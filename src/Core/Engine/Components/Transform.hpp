#pragma once
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>
#include "SFML/Graphics.hpp"
#include "SFML/System/Angle.hpp"

namespace Core::Components
{
    class Transform
    {
        glm::vec2 LocalPosition = {0.f, 0.f};
        glm::vec2 LocalSize =     {1.f, 1.f};
        sf::Angle LocalRotation = sf::Angle(sf::degrees(0.f));

    public:
        glm::mat3 LocalMatrix{1.f};
        glm::mat3 WorldMatrix{1.f};

        bool Dirty = false;
        
        inline sf::Transform ToSFML() const
        {
            const auto& m = WorldMatrix;

            return sf::Transform(
                m[0][0], m[1][0], m[2][0],
                m[0][1], m[1][1], m[2][1],
                0.f,     0.f,     1.f
            );
        }

        inline glm::vec2 GetWorldPos() const
        {
            return { WorldMatrix[2][0], WorldMatrix[2][1] };
        }

        const glm::mat3& ComputeMatrix(const Transform& t)
        {
            glm::mat3 T = glm::translate(glm::mat3(1.f), t.GetLocalPosition());
            glm::mat3 R = glm::rotate(glm::mat3(1.f), t.GetLocalRotation().asRadians());
            glm::mat3 S = glm::scale(glm::mat3(1.f), t.GetLocalSize());
            
            LocalMatrix = glm::mat3(1.f);
            LocalMatrix = T * R * S;

            return LocalMatrix;
        }

        void Move(const glm::vec2 translation)
        {
            LocalPosition += translation;
            Dirty = true;
        }

        void Rotate(const sf::Angle angle)
        {
            LocalRotation += angle;
            Dirty = true;
        }

        void Scale(const glm::vec2 factor)
        {
            LocalSize *= factor;
            Dirty = true;
        }

        void SetPosition(const glm::vec2 pos)
        {
            LocalPosition = pos;
            Dirty = true;
        }

        void SetSize(const glm::vec2 size)
        {
            LocalSize = size;
            Dirty = true;
        }

        void SetRotation(const sf::Angle angle)
        {
            LocalRotation = angle;
            Dirty = true;
        }

        const glm::vec2& GetLocalPosition() const
        {
            return LocalPosition;
        }
        const glm::vec2& GetLocalSize() const
        {
            return LocalSize;
        }
        const sf::Angle& GetLocalRotation() const
        {
            return LocalRotation;
        }
    };
}