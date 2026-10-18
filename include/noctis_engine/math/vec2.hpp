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
class TypedVec2;

using Vec2 = TypedVec2<float>;
using IVec2 = TypedVec2<std::int32_t>;
using DVec2 = TypedVec2<double>;

template <SignedRealNumber Ty_>
struct TypedVec2
{
    static inline constexpr std::size_t MAX_VALUES = 2;

    union
    {
        struct { Ty_ x, y; };
        Ty_ values[MAX_VALUES];
    };


    constexpr TypedVec2() = default;

    template <RealNumber RealNumber_>
    constexpr TypedVec2(RealNumber_ x, RealNumber_ y)
        : x{static_cast<Ty_>(x)}
        , y{static_cast<Ty_>(y)}
    {}

    template <RealNumber RealNumber_>
    constexpr TypedVec2(RealNumber_ scalar)
        : x{static_cast<Ty_>(scalar)}
        , y{static_cast<Ty_>(scalar)}
    {}

    inline static const TypedVec2<Ty_> UP{static_cast<Ty_>(0), static_cast<Ty_>(1)};
    inline static const TypedVec2<Ty_> DOWN{static_cast<Ty_>(0), static_cast<Ty_>(-1)};
    inline static const TypedVec2<Ty_> LEFT{static_cast<Ty_>(-1), static_cast<Ty_>(0)};
    inline static const TypedVec2<Ty_> RIGHT{static_cast<Ty_>(1), static_cast<Ty_>(0)};

    constexpr auto operator[](std::size_t i) -> Ty_ &
    {
        assert(i < MAX_VALUES && "Index out of range, (>= 2)");
        return values[i];
    }

    constexpr auto operator[](std::size_t i) const -> const Ty_ &
    {
        assert(i < MAX_VALUES && "Index out of range, (>= 2)");
        return values[i];
    }

    constexpr auto operator-() const -> TypedVec2<Ty_>
    {
        return TypedVec2<Ty_>{-this->x, -this->y};
    }

    // Binary operators (const)

    constexpr auto operator+(const TypedVec2<Ty_> &other) const -> TypedVec2<Ty_>
    {
        return TypedVec2<Ty_>{
            other.x + this->x, 
            other.y + this->y
        };
    }
    

    template <RealNumber RealNumber_>
    constexpr auto operator+(RealNumber_ scalar) const -> TypedVec2<Ty_>
    {
        return TypedVec2<Ty_>{
            scalar + this->x,
            scalar + this->y
        };
    }

    constexpr auto operator-(const TypedVec2<Ty_> &other) const -> TypedVec2<Ty_>
    {
        return TypedVec2<Ty_>{
            this->x - other.x, 
            this->y - other.y
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator-(RealNumber_ scalar) const -> TypedVec2<Ty_>
    {
        return TypedVec2<Ty_>{
            this->x - scalar, 
            this->y - scalar
        };
    }

    constexpr auto operator*(const TypedVec2<Ty_> &other) const -> TypedVec2<Ty_>
    {
        return TypedVec2<Ty_>{
            other.x * this->x, 
            other.y * this->y
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator*(RealNumber_ scalar) const -> TypedVec2<Ty_>
    {
        return TypedVec2<Ty_>{
            scalar * this->x, 
            scalar * this->y
        };
    }

    constexpr auto operator/(const TypedVec2<Ty_> &other) const -> TypedVec2<float>
    {
        return TypedVec2<float>{
            static_cast<float>(this->x) / static_cast<float>(other.x), 
            static_cast<float>(this->y) / static_cast<float>(other.y)
        };
    }

    template <RealNumber RealNumber_>
    constexpr auto operator/(RealNumber_ scalar) const -> TypedVec2<float>
    {
        return TypedVec2<float>{
            static_cast<float>(this->x) / static_cast<float>(scalar), 
            static_cast<float>(this->y) / static_cast<float>(scalar)
        };
    }

    // Assignment operators

    constexpr auto operator+=(const TypedVec2<Ty_> &other) -> TypedVec2<Ty_> &
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }

    template <RealNumber RealNumber_>
    constexpr auto operator+=(RealNumber_ scalar) -> TypedVec2<Ty_> &
    {
        this->x += scalar;
        this->y += scalar;
        return *this;
    }

    constexpr auto operator-=(const TypedVec2<Ty_> &other) -> TypedVec2<Ty_> &
    {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }

    template <RealNumber RealNumber_>
    constexpr auto operator-=(RealNumber_ scalar) -> TypedVec2<Ty_> &
    {
        this->x -= scalar;
        this->y -= scalar;
        return *this;
    }

    constexpr auto operator*=(const TypedVec2<Ty_> &other) -> TypedVec2<Ty_> &
    {
        this->x *= other.x;
        this->y *= other.y;
        return *this;
    }

    template <RealNumber RealNumber_>
    constexpr auto operator*=(RealNumber_ scalar) -> TypedVec2<Ty_> &
    {
        this->x *= scalar;
        this->y *= scalar;
        return *this;
    }

    constexpr auto operator/=(const TypedVec2<Ty_> &other) -> TypedVec2<Ty_> &
    {
        this->x /= other.x;
        this->y /= other.y;
        return *this;
    }

    template <RealNumber RealNumber_>
    constexpr auto operator/=(RealNumber_ scalar) -> TypedVec2<Ty_> &
    {
        this->x /= scalar;
        this->y /= scalar;
        return *this;
    }

    // Boolean operators

    constexpr auto operator==(const TypedVec2<Ty_> &other) const -> bool
    {
        return this->x == other.x 
            && this->y == other.y;
    }

    constexpr auto operator==(const TypedVec2<Ty_> &other) -> bool
    {
        return this->x == other.x 
            && this->y == other.y;
    }

    constexpr auto operator!=(const TypedVec2<Ty_> &other) const -> bool
    {
        return !(*this == other);
    }

    constexpr auto operator!=(const TypedVec2<Ty_> &other) -> bool
    {
        return !(*this == other);
    }
};

/// @brief Dot product between two 2D vectors
template <typename Ty_>
inline constexpr auto dot(const TypedVec2<Ty_> &a, const TypedVec2<Ty_> &b) -> Ty_
{
    return a.x * b.x + a.y * b.y; 
}

/// @brief Angle between two 2D vectors
template <typename Ty_>
inline constexpr auto angle(const TypedVec2<Ty_> &a, const TypedVec2<Ty_> &b) -> float
{
    double denominator =
        std::sqrt(static_cast<double>(dot(a, a))) *
        std::sqrt(static_cast<double>(dot(b, b)));

    if (denominator == 0.0)
        return 0.0f;

    double cosine = static_cast<double>(dot(a, b)) / denominator;
    return static_cast<float>(
        std::acos(std::clamp(cosine, -1.0, 1.0))
    );
}

template <typename Ty_>
inline constexpr auto distance2(const TypedVec2<Ty_> &a, const TypedVec2<Ty_> &b) -> Ty_
{
    Ty_ dx = static_cast<Ty_>(b.x) - a.x;
    Ty_ dy = static_cast<Ty_>(b.y) - a.y;

    return dx * dx + dy * dy;

}

template <typename Ty_>
inline constexpr auto distance(const TypedVec2<Ty_> &a, const TypedVec2<Ty_> &b) -> Ty_
{
    return std::sqrt(distance2(a, b));
}

} // namespace NoctisEngine