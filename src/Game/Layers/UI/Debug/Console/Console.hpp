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
        std::string,
        const char*>;

    class Console
    {

        template <typename T>
        static void Push(const T& t);
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
    void Console::Push(const T& t)
    {
        Output.push_back(t);
    }

    template <typename ... T>
    void Console::Println(T&&... text)
    {
        Print(text..., std::string("\n"));
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
                std::ostringstream temp_ss;
                temp_ss << v;

                std::string asString = temp_ss.str();
                oss << asString;

                // returns the std::variant and the string version
                cb(value, asString);
            }, value);
        }

        return oss.str();
    }
}
