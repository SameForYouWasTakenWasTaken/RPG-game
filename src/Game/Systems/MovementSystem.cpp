#include "MovementSystem.hpp"
#include "Components/Humanoid.hpp"
#include "Components/States/Movement.hpp"
#include "Engine/Components/Transform.hpp"
#include "glm/geometric.hpp"

namespace Game::Systems
{
    void Movement::LinearGoTo(Core::Components::Transform& Transform, const glm::vec2& targetPos, float speed)
    {

        auto distance = glm::distance(targetPos, Transform.GetWorldPos());
        if (distance < 0.0001f)
            return; 

        auto dir = glm::normalize(targetPos - Transform.GetWorldPos());
        Transform.Move(dir * speed);
    }

    void Movement::OnFixed(float step)
    {
    }
}