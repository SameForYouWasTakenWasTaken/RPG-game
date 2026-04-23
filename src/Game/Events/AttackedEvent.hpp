#pragma once

#include "vendor/entt/entt.hpp"
#include "Engine/Events/IEvent.hpp"

namespace Game::Events
{
    class Attacked : public Core::Events::IEvent
    {

    public:
        entt::entity Attacker{entt::null};
        entt::entity Attackee{entt::null};
        bool landed = true;
        float DamageTaken = 0.f;

        Attacked(entt::entity Attacker, entt::entity Attackee, float dmg)
        : Attacker(Attacker), Attackee(Attackee), DamageTaken(dmg) {}

        EVENT_TYPE(Attacked);
    };
}