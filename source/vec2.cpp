#include "vec2.h"

#include <cfloat>
#include <cmath>

const aVec2 aVec2Zero(0.f, 0.f);

aVec2::aVec2(float x, float y) : x(x), y(y) {}

void aVec2::setZero() {
    x = 0.0f;
    y = 0.0f;
}

void aVec2::set(float x, float y) {
    this->x = x;
    this->y = y;
};

aVec2 aVec2::operator-() const { return aVec2(-x, -y); }

float aVec2::operator()(int32_t i) const { return (&x)[i]; }
float& aVec2::operator()(int32_t i) { return (&x)[i]; }

/// Add a vector to this vector.
void aVec2::operator+=(const aVec2& v) {
    x += v.x;
    y += v.y;
}

/// Subtract a vector from this vector.
void aVec2::operator-=(const aVec2& v) {
    x -= v.x;
    y -= v.y;
}

/// Multiply this vector by a scalar.
void aVec2::operator*=(float a) {
    x *= a;
    y *= a;
}

/// Divide this vector by a scalar
void aVec2::operator/=(float a) {
    x /= a;
    y /= a;
}

/// Get the length of this vector (the norm).
float aVec2::length() { return std::sqrt(this->lengthSquared()); }

/// Get the length squared. For performance, use this instead of
/// aVec2::length (if possible).
float aVec2::lengthSquared() { return x * x + y * y; }

/// Convert this vector into a unit vector. Returns the length.
float aVec2::Normalize() {
    float length = this->length();
    if (length < FLT_EPSILON) {
        return 0.0f;
    }
    float invLength = 1.0f / length;
    x *= invLength;
    y *= invLength;

    return length;
}
float x, y;

/// Get the skew vector such that dot(skew_vec, other) == cross(vec, other)
aVec2 aVec2::skew() const { return aVec2(-y, x); }
