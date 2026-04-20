#include "Scene.hpp"

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
        hierarchy.OnUpdate(dt);
        
        for (auto& layer : m_Layers)
            layer->OnUpdate(dt);
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
        hierarchy.Init();
    }
}