#include "Material.h"

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
