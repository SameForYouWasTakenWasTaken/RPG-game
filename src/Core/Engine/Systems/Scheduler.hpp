#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

namespace Core::Systems
{
    template <typename T>
    struct SystemNode
    {
        T* system;
        std::vector<SystemNode<T>*> dependencies;

        explicit SystemNode(T* sys)
            : system(sys) {}
    };

    template <typename T>
    class Scheduler;

    template <typename T>
    struct Builder
    {
        using Node = SystemNode<T>;

        Scheduler<T>& scheduler;
        Node* primaryNode;

        Builder(Scheduler<T>& sched, Node* node)
            : scheduler(sched), primaryNode(node) {}

        Builder& After(T& other)
        {
            Node* otherNode = scheduler.GetNode(other);
            primaryNode->dependencies.push_back(otherNode);
            return *this;
        }
    };

    template <typename T>
    class Scheduler
    {
        using Node = SystemNode<T>;

    public:
        Builder<T> Add(T& system)
        {
            auto node = std::make_unique<Node>(&system);
            Node* raw = node.get();

            m_Map[&system] = raw;
            m_Nodes.push_back(std::move(node));

            return Builder<T>(*this, raw);
        }

        Node* GetNode(T& system)
        {
            return m_Map.at(&system);
        }

        template <typename Func>
        void Run(Node& node, Func&& func, std::unordered_set<Node*>& visited)
        {
            if (visited.contains(&node))
                return;

            visited.insert(&node);

            for (auto* dep : node.dependencies)
                Run(*dep, func, visited);

            func(*node.system);
        }

        template <typename Func>
        void Run(Node& node, Func&& func)
        {
            std::unordered_set<Node*> visited;
            Run(node, func, visited);
        }

        template <typename Func>
        void RunAll(Func&& func)
        {
            std::unordered_set<Node*> visited;

            for (auto& node : m_Nodes)
                Run(*node, func, visited);
        }

    private:
        std::vector<std::unique_ptr<Node>> m_Nodes;
        std::unordered_map<T*, Node*> m_Map;
    };
}