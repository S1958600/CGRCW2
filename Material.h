#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>

class Material {
public:
    float ks;                 // Specular reflection coefficient
    float kd;                 // Diffuse reflection coefficient
    float specularexponent;   // Specular reflection exponent
    std::vector<float> diffusecolor;   // Diffuse color
    std::vector<float> specularcolor;  // Specular color
    bool isreflective;        // Indicates if the material is reflective
    float reflectivity;       // Reflectivity coefficient
    bool isrefractive;        // Indicates if the material is refractive
    float refractiveindex;    // Refractive index

    Material();

    // Constructor
    Material(float ks, float kd, float specexp,
             const std::vector<float>& diffcolor,
             const std::vector<float>& speccolor,
             bool isrefl, float reflcoeff,
             bool isrefr, float refrindex);
};

#endif // MATERIAL_H
