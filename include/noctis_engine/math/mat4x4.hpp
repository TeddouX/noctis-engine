#pragma once
#include <cassert>

#include "noctis_engine/math/vec4.hpp"


namespace NoctisEngine 
{

struct Mat4x4
{
    static inline constexpr std::size_t NUM_ROWS = 4;

    Vec4 row1;
    Vec4 row2;
    Vec4 row3;
    Vec4 row4;
    
    constexpr Mat4x4() = default;
    
    constexpr Mat4x4(Vec4 row1, Vec4 row2, Vec4 row3, Vec4 row4)
        : row1{row1}
        , row2{row2}
        , row3{row3}
        , row4{row4}
    {}
    
    static const Mat4x4 IDENTITY;

    constexpr auto operator[](std::size_t i) -> Vec4 &
    {
        assert(i < NUM_ROWS && "Index out of range (i >= 4)");
        switch (i) 
        {
            default:
            case 0: return row1;
            case 1: return row2;
            case 2: return row3;
            case 3: return row4;
        }
    }

    constexpr auto operator[](std::size_t i) const -> const Vec4 &
    {
        assert(i < NUM_ROWS && "Index out of range (i >= 4)");
        switch (i) 
        {
            default:
            case 0: return row1;
            case 1: return row2;
            case 2: return row3;
            case 3: return row4;
        }
    }
};

inline constexpr Mat4x4 Mat4x4::IDENTITY{
    Vec4{1, 0, 0, 0},
    Vec4{0, 1, 0, 0},
    Vec4{0, 0, 1, 0},
    Vec4{0, 0, 0, 1}
};


}
