#ifndef VECH
#define VECH

#include <cmath>
#include <istream>

class Vec3 {
public:
    double v[3];

    Vec3() {}

    Vec3(double v0, double v1, double v2) {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
    }

    double x() const { return v[0]; }
    double y() const { return v[1]; }
    double z() const { return v[2]; }

    double r() const { return v[0]; }
    double g() const { return v[1]; }
    double b() const { return v[2]; }

    const Vec3& operator+() const { return *this; }
    Vec3 operator-() const { return Vec3(-v[0], -v[1], -v[2]); }
    double operator[](int i) const { return v[i]; }
    double& operator[](int i) { return v[i]; }

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
    Vec3& operator*=(const double t);
    Vec3& operator/=(const double t);
    Vec3& operator=(const Vec3& other);


    double length() const;
    double length_squared() const;
    Vec3 pointOnVector(double t) const;

    //make current vector a unit vector
    void normalise();

    //return a unit vector in the same direction as the current vector
    Vec3 make_normalised() const;
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

inline Vec3& Vec3::operator*=(const double t) {
    v[0] *= t;
    v[1] *= t;
    v[2] *= t;
    return *this;
}

inline Vec3& Vec3::operator/=(const double t) {
    double k = 1.0f / t;

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

inline double Vec3::length() const {
    return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

inline double Vec3::length_squared() const {
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

inline Vec3 Vec3::pointOnVector(double t) const {
    return Vec3(v[0] * t, v[1] * t, v[2] * t);
}

inline void Vec3::normalise() {
    double len = length();

    v[0] /= len;
    v[1] /= len;
    v[2] /= len;
}

inline Vec3 Vec3::make_normalised() const {
    double len = length();

    if (len != 0) {
        return Vec3(v[0] / len, v[1] / len, v[2] / len);
    } else {
        return Vec3(0, 0, 0);  // or throw an exception, or handle this case in some other way
    }
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

inline Vec3 operator*(double t, const Vec3& v) {
    return Vec3(t * v.v[0], t * v.v[1], t * v.v[2]);
}

inline Vec3 operator*(const Vec3& v, double t) {
    return Vec3(t * v.v[0], t * v.v[1], t * v.v[2]);
}

inline Vec3 operator/(double t, const Vec3& v) {
    return Vec3(t / v.v[0], t / v.v[1], t / v.v[2]);
}

inline Vec3 operator/(const Vec3& v, double t) {
    return Vec3(v.v[0] / t, v.v[1] / t, v.v[2] / t);
}

inline double dot(const Vec3& v1, const Vec3& v2) {
    return v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2];
}

inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
    return Vec3((v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1]),
        (-(v1.v[0] * v2.v[2] - v1.v[2] * v2.v[0])),
        (v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0]));
}

#endif // VECH