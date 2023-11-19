#ifndef VECH
#define VECH

#include <cmath>
#include <istream>

class Vec3 {
public:
    float v[3];

    Vec3() {}

    Vec3(float v0, float v1, float v2) {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
    }

    float x() const { return v[0]; }
    float y() const { return v[1]; }
    float z() const { return v[2]; }

    float r() const { return v[0]; }
    float g() const { return v[1]; }
    float b() const { return v[2]; }

    const Vec3& operator+() const { return *this; }
    Vec3 operator-() const { return Vec3(-v[0], -v[1], -v[2]); }
    float operator[](int i) const { return v[i]; }
    float& operator[](int i) { return v[i]; }

    Vec3& operator+=(const Vec3& v2) {
        v[0] += v2.v[0];
        v[1] += v2.v[1];
        v[2] += v2.v[2];
        return *this;
    }

    Vec3& operator-=(const Vec3& v2) {
        v[0] -= v2.v[0];
        v[1] -= v2.v[1];
        v[2] -= v2.v[2];
        return *this;
    }

    Vec3& operator*=(const Vec3& v2);
    Vec3& operator/=(const Vec3& v2);
    Vec3& operator*=(const float t);
    Vec3& operator/=(const float t);
    Vec3& operator=(const Vec3& other);


    float length() const;
    float length_squared() const;
    Vec3 pointOnVector(float t) const;

    //make current vector a unit vector
    void normalise();

    //return a unit vector in the same direction as the current vector
    Vec3 make_normalised() const;

    



    // Add the remaining function definitions from Vec3.cpp here
};


inline Vec3& Vec3::operator*=(const Vec3& v2) {
    v[0] *= v2.v[0];
    v[1] *= v2.v[1];
    v[2] *= v2.v[2];
    return *this;
}

inline Vec3& Vec3::operator/=(const Vec3& v2) {
    v[0] /= v2.v[0];
    v[1] /= v2.v[1];
    v[2] /= v2.v[2];
    return *this;
}

inline Vec3& Vec3::operator*=(const float t) {
    v[0] *= t;
    v[1] *= t;
    v[2] *= t;
    return *this;
}

inline Vec3& Vec3::operator/=(const float t) {
    float k = 1.0f / t;

    v[0] *= k;
    v[1] *= k;
    v[2] *= k;
    return *this;
}

inline Vec3& Vec3::operator=(const Vec3& other) {
    if (this != &other) {
        v[0] = other.v[0];
        v[1] = other.v[1];
        v[2] = other.v[2];
    }
    return *this;
}

inline float Vec3::length() const {
    return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

inline float Vec3::length_squared() const {
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

inline Vec3 Vec3::pointOnVector(float t) const {
    return Vec3(v[0] * t, v[1] * t, v[2] * t);
}

inline void Vec3::normalise() {
    float len = length();

    v[0] /= len;
    v[1] /= len;
    v[2] /= len;
}

inline Vec3 Vec3::make_normalised() const {
    float len = length();

    return Vec3(v[0] / len, v[1] / len, v[2] / len);
}

inline std::istream& operator>>(std::istream& is, Vec3& t) {
    is >> t.v[0] >> t.v[1] >> t.v[2];
    return is;
}

inline std::ostream& operator<<(std::ostream& os, Vec3& t) {
    os << t.v[0] << " " << t.v[1] << " " << t.v[2];
    return os;
}

inline Vec3 operator+(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.v[0] + v2.v[0], v1.v[1] + v2.v[1], v1.v[2] + v2.v[2]);
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.v[0] - v2.v[0], v1.v[1] - v2.v[1], v1.v[2] - v2.v[2]);
}

inline Vec3 operator*(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.v[0] * v2.v[0], v1.v[1] * v2.v[1], v1.v[2] * v2.v[2]);
}

inline Vec3 operator/(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.v[0] / v2.v[0], v1.v[1] / v2.v[1], v1.v[2] / v2.v[2]);
}

inline Vec3 operator*(float t, const Vec3& v) {
    return Vec3(t * v.v[0], t * v.v[1], t * v.v[2]);
}

inline Vec3 operator*(const Vec3& v, float t) {
    return Vec3(t * v.v[0], t * v.v[1], t * v.v[2]);
}

inline Vec3 operator/(float t, const Vec3& v) {
    return Vec3(t / v.v[0], t / v.v[1], t / v.v[2]);
}

inline Vec3 operator/(const Vec3& v, float t) {
    return Vec3(v.v[0] / t, v.v[1] / t, v.v[2] / t);
}

inline float dot(const Vec3& v1, const Vec3& v2) {
    return v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2];
}

inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
    return Vec3((v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1]),
        (-(v1.v[0] * v2.v[2] - v1.v[2] * v2.v[0])),
        (v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0]));
}

#endif // VECH