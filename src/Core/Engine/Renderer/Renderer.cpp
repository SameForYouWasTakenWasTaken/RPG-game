#include "Renderer.hpp"

#include "Engine/Components/Geometry.hpp"
#include "Global/Helpers.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/Graphics/VertexBuffer.hpp"
#include "SFML/System/Vector2.hpp"
#include "Types.hpp"
#include <tracy/Tracy.hpp>

namespace Core::Rendering
{
    void Renderer::Begin()
    {
        m_Batches.clear();
        m_RenderObjectQueue.clear();
    }

    void Renderer::SortRenderObjects()
    {
        std::sort(m_RenderObjectQueue.begin(), m_RenderObjectQueue.end(), 
        [](const RenderObject& first, const RenderObject& next){
            return first.zIndex > next.zIndex;
        });

    }
    void Renderer::AssignBatches()
    {
        for (const auto& Object : m_RenderObjectQueue)
        {
            assert(Object.TextureHandle.IsValid());
            assert(Object.Texture);
            assert(Object.Geometry);
            assert(Object.Transform);

            auto batchKey = Global::Hash<uint32_t>(
                                                Object.TextureHandle.id, 
                                                Object.Geometry->vertexArray.getPrimitiveType(),
                                                Object.zIndex
                                            );

            auto [it, inserted] = m_Batches.try_emplace(batchKey);
            auto& batch = it->second;
            batch.zIndex = Object.zIndex;


            const sf::Transform transform = Object.Transform->ToSFML();
            const auto& src = Object.Geometry->vertices;

            auto& dst = batch.Geometry.vertices;

            if (dst.capacity() < dst.size() + src.size())
                dst.reserve(dst.size() + src.size());

            for (const auto& vertex : src)
            {
                sf::Vertex v = vertex; // copy
                v.position = transform.transformPoint(v.position);
                dst.push_back(v);
            }

            batch.Texture = Object.Texture;
        }
    }

    void Renderer::End()
    {
        ZoneScoped;
        if (!m_ValidWindow) return;
        
        SortRenderObjects();
        AssignBatches();

        for (auto& batch : m_Batches)
        {
            auto* texture = batch.second.Texture;
            if (!texture) continue;

            sf::RenderStates states;
            states.texture = texture;

            batch.second.Geometry.RebuildVertexArray();
            RenderVertexArray(batch.second.Geometry.vertexArray, states);
        }
    }

    void Renderer::Submit(const Core::Rendering::RenderObject& Object)
    {
        m_RenderObjectQueue.push_back(Object);
    }

    void Renderer::RenderVertexArray(const sf::VertexArray& va, const sf::RenderStates& states)
    {
        m_Window->GetRenderWindow().draw(
            va,
            states
        );
    }

    void Renderer::RenderVertexBuffer(const sf::VertexBuffer& vb, const sf::RenderStates& states)
    {
        m_Window->GetRenderWindow().draw(
            vb,
            states
        );
    }

    void Renderer::Render(const Core::Rendering::RenderObject& Object)
    {
        if (!m_ValidWindow) return;
    }

    void Renderer::SetWindow(Window* window)
    {
        m_Window = window;
        m_Window ? m_ValidWindow = true : m_ValidWindow = false;
    }
}