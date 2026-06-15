#pragma once

#include <type_traits>
namespace Core::API
{
    template <typename T>
    requires(std::is_arithmetic_v<T>)
    struct Rect
    {
        T left{};
        T top{};
        T width{};
        T height{};
    };
}