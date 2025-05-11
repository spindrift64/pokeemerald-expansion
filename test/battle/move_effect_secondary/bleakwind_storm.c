#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
#if B_UPDATED_MOVE_DATA >= GEN_9
    ASSUME(MoveHasAdditionalEffect(MOVE_BLEAKWIND_STORM, MOVE_EFFECT_SPD_MINUS_1));
#else
    ASSUME(MoveHasAdditionalEffect(MOVE_BLEAKWIND_STORM, MOVE_EFFECT_FROSTBITE));
#endif
}

#if B_UPDATED_MOVE_DATA >= GEN_9

SINGLE_BATTLE_TEST("Bleakwind Storm may lower Speed (Gen 9+)")
{
    u32 ability;
    u32 chance;

    PARAMETRIZE { ability = ABILITY_HUSTLE; chance = 30; }
    PARAMETRIZE { ability = ABILITY_SERENE_GRACE; chance = 60; }

    PASSES_RANDOMLY(chance, 100, RNG_SECONDARY_EFFECT);

    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_BLEAKWIND_STORM); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BLEAKWIND_STORM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("The opposing Wobbuffet's Speed fell!");
    }
}

#else

SINGLE_BATTLE_TEST("Bleakwind Storm may cause frostbite (Pre-Gen 9)")
{
    u32 ability;
    u32 chance;

    PARAMETRIZE { ability = ABILITY_HUSTLE; chance = 30; }
    PARAMETRIZE { ability = ABILITY_SERENE_GRACE; chance = 60; }

    PASSES_RANDOMLY(chance, 100, RNG_SECONDARY_EFFECT);

    GIVEN {
        PLAYER(SPECIES_GLACEON) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_BLEAKWIND_STORM); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BLEAKWIND_STORM, player);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_FRZ, opponent); // Frostbite uses freeze anim
        STATUS_ICON(opponent, frostbite: TRUE);
    }
}

#endif
