#include "RenderLayer.hpp"
#include "Engine/Engine.hpp"
#include "tracy/Tracy.hpp"
#include "Engine/Components/Transform.hpp"
#include "Components/Sprite.hpp"
#include "Systems/ResourceManager.hpp"

namespace Game::Layers
{
    void RenderLayer::OnRender()
    {
        ZoneScoped;
        Core::Rendering::Renderer *renderer = Core::Engine::Get().GetRenderer();

        auto &registry = m_Scene->registry;
        auto view = registry.view<Game::Components::Sprite, Core::Components::Transform,
                        Core::Components::Geometry>();

        renderer->Begin();

        for (auto entity : view) {
            auto &sprite = registry.get<Game::Components::Sprite>(entity);
            auto &transform = registry.get<Core::Components::Transform>(entity);
            auto &geometry = registry.get<Core::Components::Geometry>(entity);

            if (!sprite.TextureHandle.IsValid())
                continue;

            if (m_Scene->Inventory.IsItem(entity))
              if (!m_Scene->Inventory.IsOwnedBy(entt::null,entity)) // Don't draw owned items
                    continue;

            const sf::Texture &texture =
                Systems::ResourceManager::GetTexture(sprite.TextureHandle);

            Core::Rendering::RenderObject obj{sprite.TextureHandle, &texture,
                                              &transform, &geometry};
            obj.zIndex = sprite.zIndex;
            renderer->Submit(obj);
      }

      renderer->End();
    }
}