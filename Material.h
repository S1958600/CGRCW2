#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vec3.h"

class Material {
public:
    double ks;                 // Specular reflection coefficient
    double kd;                 // Diffuse reflection coefficient
    double specularexponent;   // Specular reflection exponent
    Vec3 diffusecolor;   // Diffuse color
    Vec3 specularcolor;  // Specular color
    bool isreflective;        // Indicates if the material is reflective
    double reflectivity;       // Reflectivity coefficient
    bool isrefractive;        // Indicates if the material is refractive
    double refractiveindex;    // Refractive index

    // Default constructor
    Material();

    // Constructor
    Material(double ks, double kd, double specexp,
             const Vec3& diffcolor,
             const Vec3& speccolor,
             bool isrefl, double reflcoeff,
             bool isrefr, double refrindex);
};

#endif // MATERIAL_H
