#pragma once
#include <cassert>

#include "noctis_engine/math/vec4.hpp"


namespace NoctisEngine 
{

struct Mat4x4
{
    static inline constexpr std::size_t NUM_COLUMNS = 4;

    Vec4 column1;
    Vec4 column2;
    Vec4 column3;
    Vec4 column4;
    
    constexpr Mat4x4() = default;
    
    constexpr Mat4x4(Vec4 column1, Vec4 column2, Vec4 column3, Vec4 column4)
        : column1{column1}
        , column2{column2}
        , column3{column3}
        , column4{column4}
    {}
    
    static const Mat4x4 IDENTITY;

    constexpr auto operator[](std::size_t i) -> Vec4 &
    {
        assert(i < NUM_COLUMNS && "Index out of range (i >= 4)");
        switch (i) 
        {
            default:
            case 0: return column1;
            case 1: return column2;
            case 2: return column3;
            case 3: return column4;
        }
    }

    constexpr auto operator[](std::size_t i) const -> const Vec4 &
    {
        assert(i < NUM_COLUMNS && "Index out of range (i >= 4)");
        switch (i) 
        {
            default:
            case 0: return column1;
            case 1: return column2;
            case 2: return column3;
            case 3: return column4;
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
