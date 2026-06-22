#include "Console.hpp"

namespace Game::UI::backend
{
    std::string Console::GetOutputAsString()
    {
        std::ostringstream oss;

        for (const auto& value : Output)
        {
            std::visit([&oss](const auto& v)
            {
                oss << v;
            }, value);
        }

        return oss.str();
    }
}
