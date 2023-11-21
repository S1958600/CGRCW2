#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>

class Material {
public:
    double ks;                 // Specular reflection coefficient
    double kd;                 // Diffuse reflection coefficient
    double specularexponent;   // Specular reflection exponent
    std::vector<double> diffusecolor;   // Diffuse color
    std::vector<double> specularcolor;  // Specular color
    bool isreflective;        // Indicates if the material is reflective
    double reflectivity;       // Reflectivity coefficient
    bool isrefractive;        // Indicates if the material is refractive
    double refractiveindex;    // Refractive index

    // Default constructor
    Material();

    // Constructor
    Material(double ks, double kd, double specexp,
             const std::vector<double>& diffcolor,
             const std::vector<double>& speccolor,
             bool isrefl, double reflcoeff,
             bool isrefr, double refrindex);
};

#endif // MATERIAL_H
