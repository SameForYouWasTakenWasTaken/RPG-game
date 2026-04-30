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
        glm::vec2 LocalOrigin =     {0.f, 0.f};
        sf::Angle LocalRotation = sf::Angle(sf::degrees(0.f));

    public:
        glm::mat3 LocalMatrix{1.f};
        glm::mat3 WorldMatrix{1.f};

        bool Dirty = true;
        
        /**
         * @brief Convert the stored world 3x3 matrix into an SFML 2D affine transform.
         *
         * @return sf::Transform An sf::Transform containing the 2D affine components extracted from WorldMatrix.
         */
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

        /**
         * @brief Recomputes and stores this transform's local matrix from another transform's local properties.
         *
         * Uses the provided transform's local position, rotation, size, and origin to update this object's LocalMatrix and returns a const reference to it.
         *
         * @param t Source transform whose local position, rotation, size, and origin are used to build the matrix.
         * @return const glm::mat3& Reference to the updated LocalMatrix.
         *
         * @note The function normalizes the origin by dividing by the source transform's size; behavior is undefined if either component of `t.GetLocalSize()` is zero.
         */
        const glm::mat3& ComputeMatrix(const Transform& t)
        {
            glm::vec2 originLocal = t.GetLocalOrigin() / t.GetLocalSize();
            
            glm::mat3 T = glm::translate(glm::mat3(1.f), t.GetLocalPosition());
            glm::mat3 R = glm::rotate(glm::mat3(1.f), t.GetLocalRotation().asRadians());
            glm::mat3 S = glm::scale(glm::mat3(1.f), t.GetLocalSize());
            glm::mat3 O = glm::translate(glm::mat3(1.f),-originLocal);
            
            LocalMatrix = glm::mat3(1.f);
            LocalMatrix = T * R * S * O;

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

        /**
         * @brief Set the local rotation and mark the transform as needing recomputation.
         *
         * @param angle New local rotation angle (replaces the previous local rotation).
         */
        void SetRotation(const sf::Angle angle)
        {
            LocalRotation = angle;
            Dirty = true;
        }

        /**
         * @brief Sets the local origin for this transform.
         *
         * Updates the transform's local origin and marks the transform's matrices as needing recomputation.
         *
         * @param origin New local origin expressed in the same coordinate space as position and size.
         */
        void SetOrigin(const glm::vec2 origin)
        {
            LocalOrigin = origin;
            Dirty = true;
        }

        /**
         * @brief Accesses the transform's local position.
         *
         * @return const glm::vec2& Reference to the local position vector (position in the transform's local space).
         */
        const glm::vec2& GetLocalPosition() const
        {
            return LocalPosition;
        }

        /**
         * @brief Gets the transform's local origin.
         *
         * The local origin is the point, in local coordinate units, used as the pivot for
         * scaling and rotation when computing the transform's local matrix. Default is {0, 0}.
         *
         * @return const glm::vec2& Reference to the local origin vector.
         */
        const glm::vec2& GetLocalOrigin() const
        {
            return LocalOrigin;
        }
        /**
         * @brief Gets the transform's local size.
         *
         * @return const glm::vec2& Reference to the local size vector (width, height).
         */
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