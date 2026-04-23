#pragma once

namespace Game::Components
{
    struct Enemy
    {
        float PlayerDetectionDist = 800.f; // From how far can the enemy find the player?
        float PlayerHearDetectionDist = 500.f; // How far the enemy can hear the player
        float PlayerMaxDistance = 225.f; // How close the enemy can get to the player from its origin

        float AttackRange = 300.f;
        float AttackBegin = 0.2f;
        float AttackEnd = 2.f;
        float AttackCooldown = 5.f;
    };
}