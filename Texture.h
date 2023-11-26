#ifndef TEXTURE_H
#define TEXTURE_H

#include "Vec3.h"

class texture {
  public:
    virtual ~texture() = default;

    virtual Vec3 value(double u, double v, const Vec3& p) const = 0;
};

class solid : public texture {
  public:
    solid(Vec3 c) : colourValue_(c) {}

    solid(double red, double green, double blue) : solid(Vec3(red,green,blue)) {}

    Vec3 value(double u, double v, const Vec3& p) const override {
        return colourValue_;
    }

  private:
    Vec3 colourValue_;
};

class checkered : public texture {
  public:
    ~checkered() {
        delete even;
        delete odd;
    }

    checkered(double _scale, texture* _even, texture* _odd)
      : inv_scale(1.0 / _scale), even(_even), odd(_odd) {}

    checkered(double _scale, Vec3 col1, Vec3 col2)
      : inv_scale(1.0 / _scale),
        even(new solid(col1)),
        odd(new solid(col2))
    {}

    Vec3 value(double u, double v, const Vec3& p) const override {
        auto xInteger = static_cast<int>(std::floor(inv_scale * p.x()));
        auto yInteger = static_cast<int>(std::floor(inv_scale * p.y()));
        auto zInteger = static_cast<int>(std::floor(inv_scale * p.z()));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

  private:
    double inv_scale;
    texture* even;
    texture* odd;
};



#endif