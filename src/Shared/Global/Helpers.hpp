#pragma once

#include "SFML/Graphics/Shader.hpp"
#include <functional>
#include <cstdint>

#include "vendor/entt/entt.hpp"


namespace Global
{
    using HashID = uint32_t;
    
    template <typename SeedT, typename Type>
    void HashCombine(SeedT& seed, const Type& v) {
        seed ^= std::hash<Type>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    
    template <typename T = size_t, typename ... Types>
    T Hash(const Types&... types)
    {
        T seed = 0;
        (HashCombine<T>(seed, types), ...);
        
        return seed;
    }

    template <typename TComponent>
    inline entt::entity GetFirstEntity(entt::registry& registry)
    {
        auto view = registry.view<TComponent>();
        for (auto entity : view)
            return entity;
        return entt::null;
    }

    
}