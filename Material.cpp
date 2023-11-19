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
Material::Material(float ks, float kd, float specexp,
                   const std::vector<float>& diffcolor,
                   const std::vector<float>& speccolor,
                   bool isrefl, float reflcoeff,
                   bool isrefr, float refrindex)
    : ks(ks), kd(kd), specularexponent(specexp),
      diffusecolor(diffcolor), specularcolor(speccolor),
      isreflective(isrefl), reflectivity(reflcoeff),
      isrefractive(isrefr), refractiveindex(refrindex) {}
