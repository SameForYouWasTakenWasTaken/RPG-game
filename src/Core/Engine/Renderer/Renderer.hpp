#pragma once

#include <unordered_map>
#include <vector>

#include "Global/Helpers.hpp"
#include "Global/Types.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Graphics/VertexBuffer.hpp"
#include "Types.hpp"
#include "Engine/Window/Window.hpp"



namespace Core::Rendering
{
    class Renderer
    {
        
        Window* m_Window;
        std::unordered_map<uint32_t, Batch> m_Batches;

        bool m_ValidWindow = false;

        void RenderVertexArray(const sf::VertexArray& va, const sf::RenderStates& states);
        void RenderVertexBuffer(const sf::VertexBuffer& vb, const sf::RenderStates& states);
    public:
        Renderer(Renderer&& renderer) = delete;
        Renderer(const Renderer& renderer) = delete;
        Renderer& operator=(const Renderer&) = delete;

        Renderer(Window* window) : m_Window(window) {};

        void Submit(const RenderObject& RenderObject);
        void Begin();
        void End();

        void Render(const RenderObject& RenderObject);
        void SetWindow(Window* window);
    };
}