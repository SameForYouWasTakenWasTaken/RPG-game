
#include "Game.hpp"
#include "Game/GameSettings.hpp"

int main()
{
    Game::GameSettings settings;
    settings.windowHeight = 600;
    settings.windowWidth = 800;
    settings.windowTitle = "SFML Game";

    Game::Application game{settings};

    game.Run();
}