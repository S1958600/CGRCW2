#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vec3.h"
#include "Texture.h"

class Material {
public:
    double ks;                 // Specular reflection coefficient
    double kd;                 // Diffuse reflection coefficient
    double specularexponent;   // Specular reflection exponent
    Vec3 diffusecolor;         // Diffuse color
    Vec3 specularcolor;        // Specular color
    bool isreflective;         // Indicates if the material is reflective
    double reflectivity;       // Reflectivity coefficient
    bool isrefractive;         // Indicates if the material is refractive
    double refractiveindex;    // Refractive index
    texture* texture_;         // Texture

    // Default constructor
    Material();

    void setTexture(texture* texture) {
        texture_ = texture;
    }

    Vec3 getDiffuseColor(double u, double v, Vec3& p) const {
        if (texture_ != nullptr) {
            return texture_->value(u, v, p);
        }
        return diffusecolor;
    }


    // Constructor
    Material(double ks, double kd, double specexp,
             const Vec3& diffcolor,
             const Vec3& speccolor,
             bool isrefl, double reflcoeff,
             bool isrefr, double refrindex);
};

#endif // MATERIAL_H
