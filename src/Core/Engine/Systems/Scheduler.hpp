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

        /**
             * @brief Initializes the node to reference a system instance.
             *
             * @param sys Pointer to the system instance that this node will reference.
             *            The node does not take ownership of the pointed-to system.
             */
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

        /**
             * @brief Constructs a Builder bound to a scheduler and the primary node to configure.
             *
             * @param sched Reference to the Scheduler that owns and manages nodes.
             * @param node Pointer to the primary Node that will be configured via the builder.
             */
            Builder(Scheduler<T>& sched, Node* node)
            : scheduler(sched), primaryNode(node) {}

        /**
         * @brief Declare that the primary node depends on another system and must run after it.
         *
         * Appends the node corresponding to `other` to the primary node's dependency list.
         *
         * @param other System whose node will be executed before the primary node.
         * @return Builder& Reference to this Builder instance to allow method chaining.
         */
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
        /**
         * @brief Registers a system with the scheduler and returns a builder for configuring it.
         *
         * @param system The system instance to add to the scheduler; the scheduler will store and manage a node representing this instance.
         * @return Builder<T> A builder bound to the newly created node for the provided system.
         */
        Builder<T> Add(T& system)
        {
            auto node = std::make_unique<Node>(&system);
            Node* raw = node.get();

            m_Map[&system] = raw;
            m_Nodes.push_back(std::move(node));

            return Builder<T>(*this, raw);
        }

        /**
         * @brief Retrieve the node associated with a system instance.
         *
         * @param system The system instance whose node is being looked up.
         * @return Node* Pointer to the node associated with `system`.
         * @throws std::out_of_range if `system` has not been added to the scheduler.
         */
        Node* GetNode(T& system)
        {
            return m_Map.at(&system);
        }

        template <typename Func>
        /**
         * @brief Execute the provided callable for the given node and its dependencies in dependency-first order.
         *
         * Traverses the dependency graph starting from `node`, invoking `func` on each node's system after all of its dependencies
         * have been processed. Uses `visited` to avoid visiting the same node multiple times (prevents cycles and duplicate work).
         *
         * @param node The node whose subgraph should be traversed and executed.
         * @param func Callable invoked as `func(*node.system)` for each visited node.
         * @param visited Set of already-visited nodes; this function inserts nodes into the set as they are processed.
         */
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
        /**
         * @brief Executes the given callable for a node and all of its dependencies in dependency-first order.
         *
         * Starts a fresh traversal from the provided node and invokes `func` for each visited system exactly once,
         * after all of that system's dependencies have been processed.
         *
         * @tparam Func Callable type; must be invocable with a single argument of type `T&` (the system reference).
         * @param node The node to start traversal from.
         * @param func The callable to invoke for each system after its dependencies have run.
         */
        void Run(Node& node, Func&& func)
        {
            std::unordered_set<Node*> visited;
            Run(node, func, visited);
        }

        template <typename Func>
        /**
         * @brief Invokes the given callable for every registered system in dependency-first order.
         *
         * Processes all nodes stored in the scheduler and calls the provided functor once for each system
         * after all of its dependencies have been processed. Each node is visited at most once during this call.
         *
         * @tparam Func Callable type; must be invocable with a single argument of type `T&`.
         * @param func Functor invoked for each system as `func(system)`.
         */
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