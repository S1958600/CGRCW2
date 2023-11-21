#include "Material.h"

Material::Material() {
    ks = 0.0f;
    kd = 0.0f;
    specularexponent = 0.0f;
    diffusecolor = {0.0f, 0.0f, 0.0f};
    specularcolor = {0.0f, 0.0f, 0.0f};
    isreflective = false;
    reflectivity = 0.0f;
    isrefractive = false;
    refractiveindex = 0.0f;
}


// Constructor definition
Material::Material(double ks, double kd, double specexp,
                   const std::vector<double>& diffcolor,
                   const std::vector<double>& speccolor,
                   bool isrefl, double reflcoeff,
                   bool isrefr, double refrindex)
    : ks(ks), kd(kd), specularexponent(specexp),
      diffusecolor(diffcolor), specularcolor(speccolor),
      isreflective(isrefl), reflectivity(reflcoeff),
      isrefractive(isrefr), refractiveindex(refrindex) {}
