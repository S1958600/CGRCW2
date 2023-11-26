#ifndef RAYTRACER_H 
#define RAYTRACER_H

#include "RayStructures.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Triangle.h"
#include "ImageWriter.h"
#include "Scene.h"
#include "Material.h"
#include "BVH.h"

enum RenderMode {
    BINARY,
    PHONG,
    PATHTRACER
};

class RayTracer {
private:
    Scene scene;
    Camera camera;
    RenderMode renderMode;
    int nbounces;
    int samplesPP;
    int maxDepth;
    BVH bvh;

public:
    RayTracer();
    RayTracer(RenderMode renderMode, Camera& camera, Scene& scene, int nbounces);
    RayTracer(RenderMode renderMode, Camera& camera, Scene& scene);

    void setScene(Scene& scene);
    void setCamera(Camera& camera);
    void setRenderMode(RenderMode renderMode);
    void setSamplesPP(int samplesPP);
    void setMaxDepth(int maxDepth);
    void setBVH(BVH bvh);

    Vec3* RenderScene();
    Vec3 color(const Ray& r, int depth, Shape* ignoreShape);
    void parseRenderWrite(const std::string& filename, RenderMode renderMode, int samplesPP, int maxDepth);

};

#endif // RAYTRACER_H