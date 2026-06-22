#pragma once
#include <string>
#include <vector>
#include <variant>
#include <sstream>

namespace Game::UI::backend
{
    // Supported values for the console to log stuff
    using Value = std::variant<
        int,
        float,
        double,
        std::string>;

    class Console
    {

        template <typename T>
        static void Push(T&& t);
    public:
        inline static std::vector<Value> Output{};

        template <typename... T>
        static void Println(T&&... text);

        template <typename... T>
        static void Print(T&&... text);

        static void Clear() {Output.clear();}
        static std::string GetOutputAsString();
        template <typename TCallback>
        static std::string GetOutputAsString(TCallback&& cb);
    };

    template <typename T>
    void Console::Push(T&& t)
    {
        if constexpr (std::is_same_v<std::decay_t<T>, const char*> || std::is_same_v<std::decay_t<T>, char*>)
        {
            Output.push_back(std::string(t));
        }
        else
        {
            Output.push_back(std::forward<T>(t));
        }
    }

    template <typename ... T>
    void Console::Println(T&&... text)
    {
        Print(std::forward<T>(text)..., "\n");
    }

    template <typename ... T>
    void Console::Print(T&&... text)
    {
        (Push(std::forward<T>(text)), ...);
    }

    template <typename TCallback>
    std::string Console::GetOutputAsString(TCallback&& cb)
    {
        std::ostringstream oss;

        for (const auto& value : Output)
        {
            std::visit([&oss, &cb, &value](const auto& v)
            {
                oss << v;

                // returns the std::variant and the string version
                std::ostringstream temp_ss;
                temp_ss << v;
                cb(value, temp_ss.str());
            }, value);
        }

        return oss.str();
    }
}
