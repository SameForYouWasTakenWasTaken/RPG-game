#pragma once

#include "Engine/Systems/ISystem.hpp"
#include "Global/Helpers.hpp"
#include "Engine/Components/Transform.hpp"
#include "glm/ext/vector_float2.hpp"
#include "vendor/entt/entt.hpp"
#include <unordered_map>
namespace Game::Systems
{

    using GridKey = long long;    
    using GridEntityList = std::vector<entt::entity>;

    class SpatialGrid : public Core::Systems::ISystem
    {

        std::unordered_map<GridKey, GridEntityList> m_Grid;
        entt::registry& m_SceneRegistry;

        const float m_GridSize = 1000.f; 

        public:
        SpatialGrid(entt::registry& registry) 
            : m_SceneRegistry(registry) {}

        
        void OnUpdate(float dt) override;
        void Insert(entt::entity entity);
        void Rebuild();

        template <typename TFunc>
        GridEntityList QueryRadius(const glm::vec2& pos, float radius, TFunc&& func)
        {
           GridEntityList neighbours;

            int min_cell_x = static_cast<int>(std::floor((pos.x - radius) / m_GridSize));
            int max_cell_x = static_cast<int>(std::floor((pos.x + radius) / m_GridSize));
            int min_cell_y = static_cast<int>(std::floor((pos.y - radius) / m_GridSize));
            int max_cell_y = static_cast<int>(std::floor((pos.y + radius) / m_GridSize));

            float radiusSquared = radius * radius;

           for (int y = min_cell_y; y <= max_cell_y; y++)
            {
                for (int x = min_cell_x; x <= max_cell_x; x++)
                {
                    auto key = Global::Hash<long long>(x, y);

                    auto it = m_Grid.find(key);

                    if (it == m_Grid.end())
                        continue;

                    for (auto entity : it->second)
                    {

                        if(!func(entity))
                            continue;   
                        
                        auto& transform =
                            m_SceneRegistry.get<Core::Components::Transform>(entity);

                        glm::vec2 entityPos = transform.GetWorldPos();

                        glm::vec2 delta = entityPos - pos;

                        if (glm::dot(delta, delta) <= radiusSquared)
                                neighbours.push_back(entity);
                    }
                }
            }

            return neighbours;

        }
    };
}