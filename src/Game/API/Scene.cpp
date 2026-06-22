#include "Scene.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "Engine/Systems/ISystem.hpp"

namespace Game
{
    Scene::Scene()
    : hierarchy(registry, eventBus)
    {
        OnAttach();
    }
    Scene::~Scene()
    {
        OnDetach();
    }

    void Scene::AddLayer(std::shared_ptr<Game::Layers::ILayer> layer)
    {
        if (!layer) return;
        layer->SetParentScene(this);
        layer->OnAttach();
        m_Layers.push_back(layer);
    }

    void Scene::RemoveLayer(std::shared_ptr<Game::Layers::ILayer> layer)
    {
        if (!layer) return;
        layer->OnDetach();
        layer->SetParentScene(nullptr);
        m_Layers.erase(
            std::remove(
                m_Layers.begin(), 
                m_Layers.end(), 
                layer),
             m_Layers.end()
        );
    }

    /**
     * @brief Updates the scene for the current frame.
     *
     * Processes pending events, updates each layer, and invokes `OnUpdate(dt)` on every system
     * registered with the systems scheduler.
     *
     * @param dt Elapsed time in seconds since the previous update.
     */
    void Scene::OnUpdate(float dt)
    {
        eventBus.Update();

        for (auto& layer : m_Layers)
            layer->OnUpdate(dt);
    
        m_SystemsScheduler.RunAll([dt](Core::Systems::ISystem& system){
            system.OnUpdate(dt);
        });
    }

    /**
     * @brief Dispatches a fixed-step update to all scene layers and all registered systems.
     *
     * Invokes each layer's fixed-step handler and then runs fixed-step processing on every system
     * currently registered with the scene's systems scheduler.
     *
     * @param step Fixed time step duration in seconds.
     */
    void Scene::OnFixed(float step)
    {
        for (auto& layer : m_Layers)
            layer->OnFixed(step);

        m_SystemsScheduler.RunAll([step](Core::Systems::ISystem& system){
            system.OnFixed(step);
        });
    }

    /**
     * @brief Renders all layers attached to the scene.
     *
     * Invokes each layer's OnRender() in the order they are stored in the scene.
     */
    void Scene::OnRender()
    {
        for (auto& layer : m_Layers)
        {
            ImGuiContext* ctx = ImGui::GetCurrentContext();
            assert(ctx != nullptr && ctx->WithinFrameScope == true);
            layer->OnRender();
        }
    }

    void Scene::OnDetach()
    {
        for (auto& layer : m_Layers)
            layer->OnDetach();
    }

    /**
     * @brief Registers the scene's core systems with the systems scheduler.
     *
     * Adds the scene's hierarchy, Movement, AI, Combat, Pickup, and Inventory systems
     * to m_SystemsScheduler so they will be executed during update and fixed-step cycles.
     */
    void Scene::OnAttach()
    {
        m_SystemsScheduler.Add(hierarchy);
        m_SystemsScheduler.Add(Movement);

        m_SystemsScheduler.Add(SpatialGrid)
            .After(hierarchy)
            .After(Movement);

        m_SystemsScheduler.Add(AI);
        m_SystemsScheduler.Add(Combat);
        m_SystemsScheduler.Add(Pickup);
        m_SystemsScheduler.Add(Inventory);
        m_SystemsScheduler.Add(Progression);
        m_SystemsScheduler.Add(Reward);
    }
}