#ifndef VECH
#define VECH

#include <iostream>

class Vec3 {
public:
    Vec3();
    Vec3(float v0, float v1, float v2);

    inline float x() const;
    inline float y() const;
    inline float z() const;

    
    inline float r() const;
    inline float g() const;
    inline float b() const;

    inline const Vec3& operator+() const;
    inline Vec3 operator-() const;
    inline float operator[](int i) const;
    inline float& operator[](int i);

    inline Vec3& operator+=(const Vec3 &v2);
    inline Vec3& operator-=(const Vec3 &v2);
    inline Vec3& operator*=(const Vec3 &v2);
    inline Vec3& operator/=(const Vec3 &v2);
    inline Vec3& operator*=(const float t);
    inline Vec3& operator/=(const float t);
    inline Vec3& operator=(const Vec3& other);


    inline float length() const;
    inline float length_squared() const;
    inline Vec3 pointOnVector(float t) const;

    //makes unit length vector
    inline void normalise();

    inline Vec3 make_normalised() const;

    float v[3];
};

std::istream& operator>>(std::istream &is, Vec3 &t);
std::ostream& operator<<(std::ostream &os, Vec3 &t);

Vec3 operator+(const Vec3 &v1, const Vec3 &v2);
Vec3 operator-(const Vec3 &v1, const Vec3 &v2);
Vec3 operator*(const Vec3 &v1, const Vec3 &v2);
Vec3 operator/(const Vec3 &v1, const Vec3 &v2);
Vec3 operator*(float t, const Vec3 &v);
Vec3 operator*(const Vec3 &v, float t);
Vec3 operator/(float t, const Vec3 &v);
Vec3 operator/(const Vec3 &v, float t);

float dot(const Vec3 &v1, const Vec3 &v2);
Vec3 cross(const Vec3 &v1, const Vec3 &v2);

#endif // VECH
