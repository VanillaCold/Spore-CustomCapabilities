#pragma once
#include "stdafx.h"
#include <Spore/BasicIncludes.h>
#include <Spore\Simulator\cGetOutOfUFOToolStrategy.h>
using namespace Simulator;


virtual_detour(TakeDamageDetour, Simulator::cCombatant, Simulator::cCombatant, int(float, uint32_t, int, const Vector3&, cCombatant*)) {};

member_detour(PlayAbilityDetour, Simulator::cCreatureBase, void(int abilityIndex, Anim::AnimIndex* dstAnimIndex)) {};

virtual_detour(GetAbilityDetour, Simulator::cCreatureAnimal, Simulator::cCreatureBase, Simulator::cCreatureAbility* (int)) {};