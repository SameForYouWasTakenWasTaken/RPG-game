#include "ConsoleUI.hpp"

#include "imgui.h"

#include "Engine/Engine.hpp"

namespace Game::UI
{

    void ConsoleUI::OnUpdate(float dt)
    {
    }

    void ConsoleUI::OnRender()
    {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

        // Begin the main Console window
        if (!ImGui::Begin("Developer Console"))
        {
            ImGui::End();
            return;
        }

        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::Selectable("Clear")) { backend::Console::Clear(); }
                ImGui::EndPopup();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

            // Go through each output
            backend::Console::GetOutputAsString([](const auto& value, auto& str)
            {
                if (str == "\n") return; // ignore new lines
                ImGui::Text("[CMD] > %s", str.c_str());
            });

            // Auto-scroll to bottom on new items
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            {
                ImGui::SetScrollHereY(1.0f);
            }

            ImGui::PopStyleVar();
        }
        ImGui::EndChild();

        ImGui::Separator();

        // 3. Command Input Bar
        bool reclaim_focus = false;

        // Input text triggers when the user hits Enter
        ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue;

        ImGui::PushItemWidth(-1); // Stretch input box to fill window width
        if (ImGui::InputText("##Input", m_InputBuffer, IM_ARRAYSIZE(m_InputBuffer), input_flags))
        {
            std::string command(m_InputBuffer);

            // Strip whitespace/trim check
            command.erase(0, command.find_first_not_of(" \t\n\r"));
            command.erase(command.find_last_not_of(" \t\n\r") + 1);

            if (!command.empty())
            {
                // Log the command visually in your backend console tracking list
                backend::Console::Println(command);

                // Basic Command Router
                if (command == "clear") {
                    backend::Console::Clear();
                }
                else if (command == "help") {
                    backend::Console::Println("Available Commands: help, clear, ping");
                }
                else if (command == "ping") {
                    backend::Console::Println("pong!");
                }
                else {
                    backend::Console::Println("[ERROR] Unknown command: ", command);
                }
            }

            // Clear the text entry box and set focus back
            m_InputBuffer[0] = '\0';
            reclaim_focus = true;
        }
        ImGui::PopItemWidth();

        // Auto-focus the input box when the window opens or when a command finishes
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
        {
            ImGui::SetKeyboardFocusHere(-1); // -1 refers to the previous item
        }

        ImGui::End();
    }
}
