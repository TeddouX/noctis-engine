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
class TypedVec3;

using Vec3 = TypedVec3<float>;
using IVec3 = TypedVec3<std::int32_t>;
using DVec3 = TypedVec3<double>;

template <SignedRealNumber Ty_>
struct TypedVec3
{
    static inline constexpr std::size_t MAX_VALUES = 3;

    union
    {
        struct { Ty_ x, y, z; };
        Ty_ values[MAX_VALUES];
    };


    constexpr TypedVec3() = default;

    template <RealNumber RealNumber_>
    constexpr TypedVec3(RealNumber_ x, RealNumber_ y, RealNumber_ z)
        : x{static_cast<Ty_>(x)}
        , y{static_cast<Ty_>(y)}
        , z{static_cast<Ty_>(z)}
    {}

    template <RealNumber RealNumber_>
    constexpr TypedVec3(RealNumber_ scalar)
        : x{static_cast<Ty_>(scalar)}
        , y{static_cast<Ty_>(scalar)}
        , z{static_cast<Ty_>(scalar)}
    {}

    inline static const TypedVec3<Ty_> UP{static_cast<Ty_>(0), static_cast<Ty_>(1), static_cast<Ty_>(0)};
    inline static const TypedVec3<Ty_> DOWN{static_cast<Ty_>(0), static_cast<Ty_>(-1), static_cast<Ty_>(0)};
    inline static const TypedVec3<Ty_> LEFT{static_cast<Ty_>(-1), static_cast<Ty_>(0), static_cast<Ty_>(0)};
    inline static const TypedVec3<Ty_> RIGHT{static_cast<Ty_>(1), static_cast<Ty_>(0), static_cast<Ty_>(0)};
    inline static const TypedVec3<Ty_> FORWARDS{static_cast<Ty_>(0), static_cast<Ty_>(0), static_cast<Ty_>(1)};
    inline static const TypedVec3<Ty_> BACKWARDS{static_cast<Ty_>(0), static_cast<Ty_>(0), static_cast<Ty_>(-1)};

    constexpr auto operator[](std::size_t i) -> Ty_ &
    {
        assert(i < MAX_VALUES && "Index out of range, (>= 3)");
        return values[i];
    }

    constexpr auto operator[](std::size_t i) const -> const Ty_ &
    {
        assert(i < MAX_VALUES && "Index out of range, (>= 3)");
        return values[i];
    }

    constexpr auto operator-() const -> TypedVec3<Ty_>
    {
        return TypedVec3<Ty_>{-this->x, -this->y, -this->z};
    }

    // Binary operators (const)

    constexpr auto operator+(const TypedVec3<Ty_> &other) const -> TypedVec3<Ty_>
    {
        return TypedVec3<Ty_>{
            other.x + this->x, 
            other.y + this->y, 
            other.z + this->z
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator+(RealNumber_ scalar) const -> TypedVec3<Ty_>
    {
        return TypedVec3<Ty_>{
            scalar + this->x, 
            scalar + this->y, 
            scalar + this->z
        };
    }

    constexpr auto operator-(const TypedVec3<Ty_> &other) const -> TypedVec3<Ty_>
    {
        return TypedVec3<Ty_>{
            other.x - this->x, 
            other.y - this->y, 
            other.z - this->z
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator-(RealNumber_ scalar) const -> TypedVec3<Ty_>
    {
        return TypedVec3<Ty_>{
            scalar - this->x, 
            scalar - this->y, 
            scalar - this->z
        };
    }

    constexpr auto operator*(const TypedVec3<Ty_> &other) const -> TypedVec3<Ty_>
    {
        return TypedVec3<Ty_>{
            other.x * this->x, 
            other.y * this->y, 
            other.z * this->z
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator*(RealNumber_ scalar) const -> TypedVec3<Ty_>
    {
        return TypedVec3<Ty_>{
            scalar * this->x, 
            scalar * this->y, 
            scalar * this->z
        };
    }

    constexpr auto operator/(const TypedVec3<Ty_> &other) const -> TypedVec3<float>
    {
        return TypedVec3<float>{
            static_cast<float>(other.x) / static_cast<float>(this->x), 
            static_cast<float>(other.y) / static_cast<float>(this->y), 
            static_cast<float>(other.z) / static_cast<float>(this->z)
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator/(RealNumber_ scalar) const -> TypedVec3<float>
    {
        return TypedVec3<float>{
            static_cast<float>(scalar) / static_cast<float>(this->x), 
            static_cast<float>(scalar) / static_cast<float>(this->y), 
            static_cast<float>(scalar) / static_cast<float>(this->z)
        };
    }

    // Binary operators (non const)

    constexpr auto operator+(const TypedVec3<Ty_> &other) -> TypedVec3<Ty_>
    {
        return TypedVec3<Ty_>{
            other.x + this->x, 
            other.y + this->y, 
            other.z + this->z
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator+(RealNumber_ scalar) -> TypedVec3<Ty_>
    {
        return TypedVec3<Ty_>{
            scalar + this->x, 
            scalar + this->y, 
            scalar + this->z
        };
    }

    constexpr auto operator-(const TypedVec3<Ty_> &other) -> TypedVec3<Ty_>
    {
        return TypedVec3<Ty_>{
            other.x - this->x, 
            other.y - this->y, 
            other.z - this->z
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator-(RealNumber_ scalar) -> TypedVec3<Ty_>
    {
        return TypedVec3<Ty_>{
            scalar - this->x, 
            scalar - this->y, 
            scalar - this->z
        };
    }

    constexpr auto operator*(const TypedVec3<Ty_> &other) -> TypedVec3<Ty_>
    {
        return TypedVec3<Ty_>{
            other.x * this->x, 
            other.y * this->y, 
            other.z * this->z
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator*(RealNumber_ scalar) -> TypedVec3<Ty_>
    {
        return TypedVec3<Ty_>{
            scalar * this->x, 
            scalar * this->y, 
            scalar * this->z
        };
    }

    constexpr auto operator/(const TypedVec3<Ty_> &other) -> TypedVec3<float>
    {
        return TypedVec3<float>{
            static_cast<float>(other.x) / static_cast<float>(this->x), 
            static_cast<float>(other.y) / static_cast<float>(this->y), 
            static_cast<float>(other.z) / static_cast<float>(this->z)
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator/(RealNumber_ scalar) -> TypedVec3<float>
    {
        return TypedVec3<float>{
            static_cast<float>(scalar) / static_cast<float>(this->x), 
            static_cast<float>(scalar) / static_cast<float>(this->y), 
            static_cast<float>(scalar) / static_cast<float>(this->z)
        };
    }

    // Assignment operators

    constexpr auto operator+=(const TypedVec3<Ty_> &other) -> TypedVec3<Ty_> &
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }

    template <RealNumber RealNumber_>
    constexpr auto operator+=(RealNumber_ scalar) -> TypedVec3<Ty_>
    {
        this->x += scalar;
        this->y += scalar;
        this->z += scalar;
        return *this;
    }

    constexpr auto operator-=(const TypedVec3<Ty_> &other) -> TypedVec3<Ty_> &
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }

    template <RealNumber RealNumber_>
    constexpr auto operator-=(RealNumber_ scalar) -> TypedVec3<Ty_>
    {
        this->x -= scalar;
        this->y -= scalar;
        this->z -= scalar;
        return *this;
    }

    constexpr auto operator*=(const TypedVec3<Ty_> &other) -> TypedVec3<Ty_> &
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;
        return *this;
    }

    template <RealNumber RealNumber_>
    constexpr auto operator*=(RealNumber_ scalar) -> TypedVec3<Ty_>
    {
        this->x *= scalar;
        this->y *= scalar;
        this->z *= scalar;
        return *this;
    }

    constexpr auto operator/=(const TypedVec3<Ty_> &other) -> TypedVec3<Ty_> &
    {
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;
        return *this;
    }

    template <RealNumber RealNumber_>
    constexpr auto operator/=(RealNumber_ scalar) -> TypedVec3<Ty_>
    {
        this->x /= scalar;
        this->y /= scalar;
        this->z /= scalar;
        return *this;
    }

    // Boolean operators

    constexpr auto operator==(const TypedVec3<Ty_> &other) -> bool
    {
        return this->x == other.x 
            && this->y == other.y
            && this->z == other.z;
    }

    constexpr auto operator!=(const TypedVec3<Ty_> &other) -> bool
    {
        return !(*this == other);
    }
};

/// @brief Dot product between two 3D vectors
template <typename Ty_>
inline constexpr auto dot(const TypedVec3<Ty_> &x, const TypedVec3<Ty_> &y) -> float
{
    return x.x * y.x + x.y * y.y + x.z * y.z; 
}

/// @brief Angle between two 3D vectors
template <typename Ty_>
inline constexpr auto angle(const TypedVec3<Ty_> &x, const TypedVec3<Ty_> &y) -> float
{
    return std::acos(std::clamp(dot(x, y), static_cast<Ty_>(-1), static_cast<Ty_>(1)));
}

template <typename Ty_>
inline constexpr auto distance2(const TypedVec3<Ty_> &x, const TypedVec3<Ty_> &y) -> float
{
    return std::pow(y.x - x.x, 2) 
    + std::pow(y.y - x.y, 2) 
    + std::pow(y.z - x.z, 2);
}

template <typename Ty_>
inline constexpr auto distance(const TypedVec3<Ty_> &x, const TypedVec3<Ty_> &y) -> float
{
    return std::sqrt(distance2(x, y));
}

} // namespace NoctisEngine
