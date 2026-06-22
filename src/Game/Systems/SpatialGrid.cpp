#include "SpatialGrid.hpp"
#include "Engine/Components/Transform.hpp"
#include "Global/Helpers.hpp"
#include "Components/LonelyTags.hpp"



namespace Game::Systems
{
    void SpatialGrid::Insert(entt::entity entity)
    {
        auto& transform = m_SceneRegistry.get<Core::Components::Transform>(entity);
        auto pos = transform.GetWorldPos();
        int cell_x = static_cast<int>(std::floor(pos.x / m_GridSize));
        int cell_y = static_cast<int>(std::floor(pos.y / m_GridSize));

        auto key = Global::Hash<long long>(cell_x, cell_y);
        m_Grid[key].push_back(entity);
    }

    void SpatialGrid::OnUpdate(float dt)
    {
        Rebuild();
    }

    void SpatialGrid::Rebuild()
    {
        m_Grid.clear();

        auto view = m_SceneRegistry.view<Core::Components::Transform, Game::Components::LonelyTags::Spatial>();

        for (auto entity : view)
            Insert(entity);
    }
}