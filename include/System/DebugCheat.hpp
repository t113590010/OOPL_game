#ifndef DEBUG_CHEAT_HPP
#define DEBUG_CHEAT_HPP

class DebugCheat {
public:
    static bool CatAttackBoost;
    static bool CatSpeedBoost;

    static float GetCatAttackMultiplier()
    {
        return CatAttackBoost ? 2.0f : 1.0f;
    }

    static float GetCatSpeedMultiplier()
    {
        return CatSpeedBoost ? 2.0f : 1.0f;
    }
};

#endif