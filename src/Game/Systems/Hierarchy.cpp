#include "Hierarchy.hpp"
#include "API/Scene.hpp"
#include "Components/HierarchyComponent.hpp"
#include "Engine/Components/Transform.hpp"

namespace Game::Systems
{
    Hierarchy::Hierarchy(entt::registry& registry, Core::Events::EventBus& eventBus) 
    : m_SceneRegistry(registry), m_EventBus(eventBus)
    {

    }

    void Hierarchy::OnUpdate(float dt)
    {
        auto roots = m_SceneRegistry.view<Components::Hierarchy, Core::Components::Transform>();
        
        for (auto entity : roots)
        {
            auto& h = roots.get<Components::Hierarchy>(entity);
            if (h.Parent == entt::null)
                UpdateRecursive(entity, glm::mat3(1.f));
            
        }
    }

    void Hierarchy::UpdateRecursive(entt::entity entity, const glm::mat3& parentWorld)
    {
        assert(entity != entt::null);

        auto& hierarchy = m_SceneRegistry.get<Components::Hierarchy>(entity);
        auto& transform = m_SceneRegistry.get<Core::Components::Transform>(entity);

        // Compute local matrix if dirty
        if (transform.Dirty)
        {
            transform.LocalMatrix = transform.ComputeMatrix(transform);
            transform.Dirty = false;
        }

        // Compute world matrix
        transform.WorldMatrix = parentWorld * transform.LocalMatrix;

        // Traverse children (linked list)
        entt::entity child = hierarchy.FirstChild;

        while (child != entt::null)
        {
            auto& childHierarchy = m_SceneRegistry.get<Components::Hierarchy>(child);
            entt::entity next = childHierarchy.NextSibling;

            UpdateRecursive(child, transform.WorldMatrix);

            child = next;
        }
    }

    void Hierarchy::AddChild(entt::entity parent, entt::entity child)
    {
        assert(parent == entt::null);
        assert(child == entt::null);

        if(
            !m_SceneRegistry.all_of<Components::Hierarchy>(parent) ||
            !m_SceneRegistry.all_of<Components::Hierarchy>(parent)
        ) return;

        auto& p = m_SceneRegistry.get<Components::Hierarchy>(parent);
        auto& c = m_SceneRegistry.get<Components::Hierarchy>(child);

        // remove from previous parent if needed
        if (c.Parent != entt::null)
        {
            auto& oldParent = m_SceneRegistry.get<Components::Hierarchy>(c.Parent);

            entt::entity* it = &oldParent.FirstChild;

            while (*it != entt::null)
            {
                if (*it == child)
                {
                    *it = c.NextSibling;
                    break;
                }
                
                it = &m_SceneRegistry.get<Components::Hierarchy>(*it).NextSibling;
            }
        }

        c.Parent = parent;

        // insert at head
        c.NextSibling = p.FirstChild;
        p.FirstChild = child;
    }


    void Hierarchy::Traverse(entt::entity parent)
    {
        auto& h = m_SceneRegistry.get<Components::Hierarchy>(parent);

        for (entt::entity child = h.FirstChild;
            child != entt::null;
            )
        {
            auto& childH = m_SceneRegistry.get<Components::Hierarchy>(child);
            entt::entity next = childH.NextSibling;

            // process child here if needed

            Traverse(child);

            child = next;
        }
    }
}