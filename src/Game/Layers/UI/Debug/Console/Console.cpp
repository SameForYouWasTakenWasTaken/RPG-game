#include "Console.hpp"

namespace Game::UI::backend
{
    std::string Console::GetOutputAsString()
    {
        return GetOutputAsString([](const Value& variant, auto& str)
        {

        });
    }
}
