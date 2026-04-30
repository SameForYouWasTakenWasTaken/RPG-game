#include "Scene.hpp"
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

    void Scene::OnUpdate(float dt)
    {
        eventBus.Update();
        
        for (auto& layer : m_Layers)
            layer->OnUpdate(dt);
    
        m_SystemsScheduler.RunAll([dt](Core::Systems::ISystem& system){
            system.OnUpdate(dt);
        });
    }

    void Scene::OnFixed(float step)
    {
        for (auto& layer : m_Layers)
            layer->OnFixed(step);

        m_SystemsScheduler.RunAll([step](Core::Systems::ISystem& system){
            system.OnFixed(step);
        });
    }

    void Scene::OnRender()
    {
        for (auto& layer : m_Layers)
            layer->OnRender();
    }

    void Scene::OnDetach()
    {
        for (auto& layer : m_Layers)
            layer->OnDetach();
    }

    void Scene::OnAttach()
    {
        m_SystemsScheduler.Add(hierarchy);
        m_SystemsScheduler.Add(Movement);
        m_SystemsScheduler.Add(AI);
        m_SystemsScheduler.Add(Combat);
        m_SystemsScheduler.Add(Pickup);
        m_SystemsScheduler.Add(Inventory);
    }
}