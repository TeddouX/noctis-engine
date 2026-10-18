#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <stdbool.h>

#include "common.hpp"


namespace NoctisEngine
{

template <SignedRealNumber Ty_>
class TypedVec4;

using Vec4 = TypedVec4<float>;
using IVec4 = TypedVec4<std::int32_t>;
using DVec4 = TypedVec4<double>;

template <SignedRealNumber Ty_>
struct TypedVec4
{
    static inline constexpr std::size_t MAX_VALUES = 4;

    union
    {
        struct { Ty_ x, y, z, w; };
        Ty_ values[MAX_VALUES];
    };


    constexpr TypedVec4() = default;

    template <RealNumber RealNumber_>
    constexpr TypedVec4(RealNumber_ x, RealNumber_ y, RealNumber_ z, RealNumber_ w)
        : x{static_cast<Ty_>(x)}
        , y{static_cast<Ty_>(y)}
        , z{static_cast<Ty_>(z)}
        , w{static_cast<Ty_>(w)}
    {}

    template <RealNumber RealNumber_>
    constexpr TypedVec4(RealNumber_ scalar)
        : x{static_cast<Ty_>(scalar)}
        , y{static_cast<Ty_>(scalar)}
        , z{static_cast<Ty_>(scalar)}
        , w{static_cast<Ty_>(scalar)}
    {}

    inline static const TypedVec4<Ty_> UP{static_cast<Ty_>(0), static_cast<Ty_>(1), static_cast<Ty_>(0), static_cast<Ty_>(0)};
    inline static const TypedVec4<Ty_> DOWN{static_cast<Ty_>(0), static_cast<Ty_>(-1), static_cast<Ty_>(0), static_cast<Ty_>(0)};
    inline static const TypedVec4<Ty_> LEFT{static_cast<Ty_>(-1), static_cast<Ty_>(0), static_cast<Ty_>(0), static_cast<Ty_>(0)};
    inline static const TypedVec4<Ty_> RIGHT{static_cast<Ty_>(1), static_cast<Ty_>(0), static_cast<Ty_>(0), static_cast<Ty_>(0)};
    inline static const TypedVec4<Ty_> FORWARDS{static_cast<Ty_>(0), static_cast<Ty_>(0), static_cast<Ty_>(1), static_cast<Ty_>(0)};
    inline static const TypedVec4<Ty_> BACKWARDS{static_cast<Ty_>(0), static_cast<Ty_>(0), static_cast<Ty_>(-1), static_cast<Ty_>(0)};
    inline static const TypedVec4<Ty_> W_POS{static_cast<Ty_>(0), static_cast<Ty_>(0), static_cast<Ty_>(0), static_cast<Ty_>(1)};
    inline static const TypedVec4<Ty_> W_NEG{static_cast<Ty_>(0), static_cast<Ty_>(0), static_cast<Ty_>(0), static_cast<Ty_>(-1)};


    constexpr auto operator[](std::size_t i) -> Ty_ &
    {
        assert(i < MAX_VALUES && "Index out of range, (>= 4)");
        return values[i];
    }

    constexpr auto operator[](std::size_t i) const -> const Ty_ &
    {
        assert(i < MAX_VALUES && "Index out of range, (>= 4)");
        return values[i];
    }

    constexpr auto operator-() const -> TypedVec4<Ty_>
    {
        return TypedVec4<Ty_>{-this->x, -this->y, -this->z, -this->w};
    }

    // Binary operators (non const)

    constexpr auto operator+(const TypedVec4<Ty_> &other) const -> TypedVec4<Ty_>
    {
        return TypedVec4<Ty_>{
            other.x + this->x, 
            other.y + this->y, 
            other.z + this->z,
            other.w + this->w
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator+(RealNumber_ scalar) const -> TypedVec4<Ty_>
    {
        return TypedVec4<Ty_>{
            scalar + this->x, 
            scalar + this->y, 
            scalar + this->z,
            scalar + this->w
        };
    }

    constexpr auto operator-(const TypedVec4<Ty_> &other) const -> TypedVec4<Ty_>
    {
        return TypedVec4<Ty_>{
            other.x - this->x, 
            other.y - this->y, 
            other.z - this->z,
            other.w - this->w
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator-(RealNumber_ scalar) const -> TypedVec4<Ty_>
    {
        return TypedVec4<Ty_>{
            scalar - this->x, 
            scalar - this->y, 
            scalar - this->z, 
            scalar - this->w
        };
    }

    constexpr auto operator*(const TypedVec4<Ty_> &other) const -> TypedVec4<Ty_>
    {
        return TypedVec4<Ty_>{
            other.x * this->x, 
            other.y * this->y, 
            other.z * this->z,
            other.w * this->w
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator*(RealNumber_ scalar) const -> TypedVec4<Ty_>
    {
        return TypedVec4<Ty_>{
            scalar * this->x, 
            scalar * this->y, 
            scalar * this->z, 
            scalar * this->w
        };
    }

    constexpr auto operator/(const TypedVec4<Ty_> &other) const -> TypedVec4<float>
    {
        return TypedVec4<float>{
            static_cast<float>(other.x) / static_cast<float>(this->x), 
            static_cast<float>(other.y) / static_cast<float>(this->y), 
            static_cast<float>(other.z) / static_cast<float>(this->z),
            static_cast<float>(other.w) / static_cast<float>(this->w)
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator/(RealNumber_ scalar) const -> TypedVec4<float>
    {
        return TypedVec4<float>{
            static_cast<float>(scalar) / static_cast<float>(this->x), 
            static_cast<float>(scalar) / static_cast<float>(this->y), 
            static_cast<float>(scalar) / static_cast<float>(this->z), 
            static_cast<float>(scalar) / static_cast<float>(this->w)
        };
    }

        // Binary operators (const)

    constexpr auto operator+(const TypedVec4<Ty_> &other) -> TypedVec4<Ty_>
    {
        return TypedVec4<Ty_>{
            other.x + this->x, 
            other.y + this->y, 
            other.z + this->z,
            other.w + this->w
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator+(RealNumber_ scalar) -> TypedVec4<Ty_>
    {
        return TypedVec4<Ty_>{
            scalar + this->x, 
            scalar + this->y, 
            scalar + this->z,
            scalar + this->w
        };
    }

    constexpr auto operator-(const TypedVec4<Ty_> &other) -> TypedVec4<Ty_>
    {
        return TypedVec4<Ty_>{
            other.x - this->x, 
            other.y - this->y, 
            other.z - this->z,
            other.w - this->w
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator-(RealNumber_ scalar) -> TypedVec4<Ty_>
    {
        return TypedVec4<Ty_>{
            scalar - this->x, 
            scalar - this->y, 
            scalar - this->z, 
            scalar - this->w
        };
    }

    constexpr auto operator*(const TypedVec4<Ty_> &other) -> TypedVec4<Ty_>
    {
        return TypedVec4<Ty_>{
            other.x * this->x, 
            other.y * this->y, 
            other.z * this->z,
            other.w * this->w
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator*(RealNumber_ scalar) -> TypedVec4<Ty_>
    {
        return TypedVec4<Ty_>{
            scalar * this->x, 
            scalar * this->y, 
            scalar * this->z, 
            scalar * this->w
        };
    }

    constexpr auto operator/(const TypedVec4<Ty_> &other) -> TypedVec4<float>
    {
        return TypedVec4<float>{
            static_cast<float>(other.x) / static_cast<float>(this->x), 
            static_cast<float>(other.y) / static_cast<float>(this->y), 
            static_cast<float>(other.z) / static_cast<float>(this->z),
            static_cast<float>(other.w) / static_cast<float>(this->w)
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator/(RealNumber_ scalar) -> TypedVec4<float>
    {
        return TypedVec4<float>{
            static_cast<float>(scalar) / static_cast<float>(this->x), 
            static_cast<float>(scalar) / static_cast<float>(this->y), 
            static_cast<float>(scalar) / static_cast<float>(this->z), 
            static_cast<float>(scalar) / static_cast<float>(this->w)
        };
    }

    // Assignment operators

    constexpr auto operator+=(const TypedVec4<Ty_> &other) -> TypedVec4<Ty_> &
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        this->w += other.w;
        return *this;
    }

    template <RealNumber RealNumber_>
    constexpr auto operator+=(RealNumber_ scalar) -> TypedVec4<Ty_>
    {
        this->x += scalar;
        this->y += scalar;
        this->z += scalar;
        this->w += scalar;
        return *this;
    }

    constexpr auto operator-=(const TypedVec4<Ty_> &other) -> TypedVec4<Ty_> &
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        this->w -= other.w;
        return *this;
    }

    template <RealNumber RealNumber_>
    constexpr auto operator-=(RealNumber_ scalar) -> TypedVec4<Ty_>
    {
        this->x -= scalar;
        this->y -= scalar;
        this->z -= scalar;
        this->w -= scalar;
        return *this;
    }

    constexpr auto operator*=(const TypedVec4<Ty_> &other) -> TypedVec4<Ty_> &
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;
        this->w *= other.w;
        return *this;
    }

    template <RealNumber RealNumber_>
    constexpr auto operator*=(RealNumber_ scalar) -> TypedVec4<Ty_>
    {
        this->x *= scalar;
        this->y *= scalar;
        this->z *= scalar;
        this->w *= scalar;
        return *this;
    }

    constexpr auto operator/=(const TypedVec4<Ty_> &other) -> TypedVec4<Ty_> &
    {
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;
        this->w /= other.w;
        return *this;
    }

    template <RealNumber RealNumber_>
    constexpr auto operator/=(RealNumber_ scalar) -> TypedVec4<Ty_>
    {
        this->x /= scalar;
        this->y /= scalar;
        this->z /= scalar;
        this->w /= scalar;
        return *this;
    }

    // Boolean operators

    constexpr auto operator==(const TypedVec4<Ty_> &other) -> bool
    {
        return this->x == other.x 
            && this->y == other.y
            && this->z == other.z
            && this->w == other.w;
    }

    constexpr auto operator!=(const TypedVec4<Ty_> &other) -> bool
    {
        return !(*this == other);
    }
};

/// @brief Dot product between two 4D vectors
template <typename Ty_>
inline constexpr auto dot(const TypedVec4<Ty_> &x, const TypedVec4<Ty_> &y) -> float
{
    return x.x * y.x + x.y * y.y + x.z * y.z + x.w * y.w; 
}

/// @brief Angle between two 4D vectors
template <typename Ty_>
inline constexpr auto angle(const TypedVec4<Ty_> &x, const TypedVec4<Ty_> &y) -> float
{
    return std::acos(std::clamp(dot(x, y), static_cast<Ty_>(-1), static_cast<Ty_>(1)));
}

template <typename Ty_>
inline constexpr auto distance2(const TypedVec4<Ty_> &x, const TypedVec4<Ty_> &y) -> float
{
    return std::pow(y.x - x.x, 2) 
        + std::pow(y.y - x.y, 2) 
        + std::pow(y.z - x.z, 2) 
        + std::pow(y.w - x.w, 2);
}

template <typename Ty_>
inline constexpr auto distance(const TypedVec4<Ty_> &x, const TypedVec4<Ty_> &y) -> float
{
    return std::sqrt(distance2(x, y));
}

} // namespace NoctisEngine