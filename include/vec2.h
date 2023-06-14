#ifndef __VEC2_H__
#define __VEC2_H__

#include <cstdint>
struct aVec2 {
    aVec2() = default;

    aVec2(float x, float y);

    void setZero();

    void set(float x, float y);

    aVec2 operator-() const;

    float operator()(int32_t i) const;
    float& operator()(int32_t i);

    /// Add a vector to this vector.
    void operator+=(const aVec2& v);

    /// Subtract a vector from this vector.
    void operator-=(const aVec2& v);

    /// Multiply this vector by a scalar.
    void operator*=(float a);

    /// Divide this vector by a scalar.
    void operator/=(float a);

    /// Get the length of this vector (the norm).
    float length();

    /// Get the length squared. For performance, use this instead of
    /// aVec2::length (if possible).
    float lengthSquared();

    /// Convert this vector into a unit vector. Returns the length.
    float Normalize();
    float x, y;

    /// Get the skew vector such that dot(skew_vec, other) == cross(vec, other)
    aVec2 skew() const;
};

extern const aVec2 aVec2Zero;

/*
inline aVec2 aAbs(const aVec2& a);

inline float aDot(const aVec2& a, const aVec2& b);

/// Add two vectors component-wise.
inline aVec2 operator+(const aVec2& a, const aVec2& b);

/// Subtract two vectors component-wise.
inline aVec2 operator-(const aVec2& a, const aVec2& b);

inline aVec2 operator*(float s, const aVec2& a);
inline aVec2 operator*(const aVec2& a, float s);

inline bool operator==(const aVec2& a, const aVec2& b);

inline bool operator!=(const aVec2& a, const aVec2& b);

inline float aDistance(const aVec2& a, const aVec2& b);

inline float aDistanceSquared(const aVec2& a, const aVec2& b);
*/

inline aVec2 aAbs(const aVec2& a) {
    return aVec2(a.x > 0 ? a.x : -a.x, a.y > 0 ? a.y : -a.y);
}

inline float aDot(const aVec2& a, const aVec2& b) {
    return a.x * b.x + a.y * b.y;
}

/// Add two vectors component-wise.
inline aVec2 operator+(const aVec2& a, const aVec2& b) {
    return aVec2(a.x + b.x, a.y + b.y);
}

/// Subtract two vectors component-wise.
inline aVec2 operator-(const aVec2& a, const aVec2& b) {
    return aVec2(a.x - b.x, a.y - b.y);
}

inline aVec2 operator*(float s, const aVec2& a) {
    return aVec2(s * a.x, s * a.y);
}
inline aVec2 operator*(const aVec2& a, float s) {
    return aVec2(s * a.x, s * a.y);
}

inline bool operator==(const aVec2& a, const aVec2& b) {
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const aVec2& a, const aVec2& b) {
    return a.x != b.x || a.y != b.y;
}

inline float aDistance(const aVec2& a, const aVec2& b) {
    aVec2 c = a - b;
    return c.length();
}

inline float aDistanceSquared(const aVec2& a, const aVec2& b) {
    aVec2 c = a - b;
    return aDot(c, c);
}

#endif /* __VEC2_H__ */
