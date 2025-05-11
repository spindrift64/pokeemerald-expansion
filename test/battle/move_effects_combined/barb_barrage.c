#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(GetMoveEffect(MOVE_BARB_BARRAGE) == EFFECT_DOUBLE_POWER_ON_ARG_STATUS);
    ASSUME(MoveHasAdditionalEffect(MOVE_BARB_BARRAGE, MOVE_EFFECT_POISON) == TRUE);
}

SINGLE_BATTLE_TEST("Barb Barrage inflicts poison")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_BARB_BARRAGE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BARB_BARRAGE, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
        STATUS_ICON(opponent, poison: TRUE);
    }
}

#if B_UPDATED_MOVE_DATA >= GEN_9

SINGLE_BATTLE_TEST("Barb Barrage's power doubles if the target is poisoned", s16 damage)
{
    ASSUME(GetMoveEffectArg_Status(MOVE_BARB_BARRAGE) == STATUS1_PSN_ANY);

    u32 status1;
    PARAMETRIZE { status1 = 0; }
    PARAMETRIZE { status1 = STATUS1_POISON; }
    PARAMETRIZE { status1 = STATUS1_TOXIC_POISON; }

    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Status1(status1); }
    } WHEN {
        TURN { MOVE(player, MOVE_BARB_BARRAGE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BARB_BARRAGE, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.0), results[0].damage); // no status
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[1].damage); // poison
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[2].damage); // toxic
    }
}

#else

SINGLE_BATTLE_TEST("Barb Barrage's power doubles if the target is statused", s16 damage)
{
    ASSUME(GetMoveEffectArg_Status(MOVE_BARB_BARRAGE) == STATUS1_ANY);

    u32 status1;
    PARAMETRIZE { status1 = 0; }
    PARAMETRIZE { status1 = STATUS1_POISON; }
    PARAMETRIZE { status1 = STATUS1_TOXIC_POISON; }
    PARAMETRIZE { status1 = STATUS1_BURN; }
    PARAMETRIZE { status1 = STATUS1_PARALYSIS; }
    PARAMETRIZE { status1 = STATUS1_SLEEP; }
    PARAMETRIZE { status1 = STATUS1_FREEZE; }
    PARAMETRIZE { status1 = STATUS1_FROSTBITE; }

    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Status1(status1); }
    } WHEN {
        TURN { MOVE(player, MOVE_BARB_BARRAGE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BARB_BARRAGE, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.0), results[0].damage); // no status
        for (int j = 1; j < 8; j++) {
            EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[j].damage); // all statuses
        }
    }
}

#endif
