#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
#if B_UPDATED_MOVE_DATA >= GEN_9
    ASSUME(MoveHasAdditionalEffect(MOVE_BITTER_MALICE, MOVE_EFFECT_ATK_MINUS_1));
#else
    #if B_USE_FROSTBITE == TRUE
        ASSUME(MoveHasAdditionalEffect(MOVE_BITTER_MALICE, MOVE_EFFECT_FROSTBITE));
    #else
        ASSUME(MoveHasAdditionalEffect(MOVE_BITTER_MALICE, MOVE_EFFECT_FREEZE));
    #endif
    ASSUME(GetMoveEffect(MOVE_BITTER_MALICE) == EFFECT_DOUBLE_POWER_ON_ARG_STATUS);
    ASSUME(GetMoveEffectArg_Status(MOVE_BITTER_MALICE) == STATUS1_ANY);
#endif
}

#if B_UPDATED_MOVE_DATA >= GEN_9

SINGLE_BATTLE_TEST("Bitter Malice lowers the target's Attack")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_BITTER_MALICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("The opposing Wobbuffet's Attack fell!");
    }
}

SINGLE_BATTLE_TEST("Bitter Malice does not double power when the target is statused (Gen 9+)", s16 damage)
{
    u32 status1;
    PARAMETRIZE { status1 = 0; }
    PARAMETRIZE { status1 = STATUS1_POISON; }
    PARAMETRIZE { status1 = STATUS1_BURN; }
    PARAMETRIZE { status1 = STATUS1_PARALYSIS; }
    PARAMETRIZE { status1 = STATUS1_FREEZE; }
    PARAMETRIZE { status1 = STATUS1_SLEEP; }
    PARAMETRIZE { status1 = STATUS1_TOXIC_POISON; }
    PARAMETRIZE { status1 = STATUS1_FROSTBITE; }

    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Status1(status1); }
    } WHEN {
        TURN { MOVE(player, MOVE_BITTER_MALICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BITTER_MALICE, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        for (int j = 1; j < 8; j++) {
            EXPECT_EQ(results[j].damage, results[0].damage);
        }
    }
}

#else

SINGLE_BATTLE_TEST("Bitter Malice may cause frostbite (Gen 8)")
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
        TURN { MOVE(player, MOVE_BITTER_MALICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BITTER_MALICE, player);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_FRZ, opponent);
    }
}

SINGLE_BATTLE_TEST("Bitter Malice's power doubles if the target is statused", s16 damage)
{
    u32 status1;
    PARAMETRIZE { status1 = 0; }
    PARAMETRIZE { status1 = STATUS1_POISON; }
    PARAMETRIZE { status1 = STATUS1_BURN; }
    PARAMETRIZE { status1 = STATUS1_PARALYSIS; }
    PARAMETRIZE { status1 = STATUS1_FREEZE; }
    PARAMETRIZE { status1 = STATUS1_SLEEP; }
    PARAMETRIZE { status1 = STATUS1_TOXIC_POISON; }
    PARAMETRIZE { status1 = STATUS1_FROSTBITE; }

    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Status1(status1); }
    } WHEN {
        TURN { MOVE(player, MOVE_BITTER_MALICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BITTER_MALICE, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.0), results[0].damage);
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[1].damage);
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[2].damage);
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[3].damage);
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[4].damage);
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[5].damage);
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[6].damage);
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[7].damage);
    }
}

#endif
