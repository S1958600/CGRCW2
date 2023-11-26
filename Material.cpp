#include "Material.h"

Material::Material() {
    ks = 0.0f;
    kd = 0.0f;
    specularexponent = 0.0001f;
    diffusecolor = Vec3(0.0f, 0.0f, 0.0f);
    specularcolor = Vec3(0.0f, 0.0f, 0.0f);
    isreflective = false;
    reflectivity = 0.0f;
    isrefractive = false;
    refractiveindex = 0.0f;
    texture_ = nullptr;
}


// Constructor definition
Material::Material(double ks, double kd, double specexp,
                   const Vec3& diffcolor,
                   const Vec3& speccolor,
                   bool isrefl, double reflcoeff,
                   bool isrefr, double refrindex)
    : ks(ks), kd(kd), specularexponent(specexp),
      diffusecolor(diffcolor), specularcolor(speccolor),
      isreflective(isrefl), reflectivity(reflcoeff),
      isrefractive(isrefr), refractiveindex(refrindex) {
          texture_ = nullptr;
      }
