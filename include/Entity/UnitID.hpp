#ifndef UNIT_ID_HPP
#define UNIT_ID_HPP

enum class UnitID {
    NONE = 0,
    BASIC_CAT = 1,
    LONG_LEG_CAT = 2,

    // 敵人也可以編號，方便以後統一管理
    BASIC_ENEMY = 101
};

#endif