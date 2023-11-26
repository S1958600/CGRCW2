#ifndef BVH_H
#define BVH_H

#include "Shape.h"
#include "RayStructures.h"
#include "Vec3.h"
#include "BoundingBox.h"
#include <vector>
#include <algorithm>


struct BVHNode{
    BoundingBox box;
    Shape* shape;
    bool hasShape;
    bool isEmpty;
    BVHNode* left;
    BVHNode* right;
};



class BVH{
public:
    BVHNode* root;

    BVH();
    BVH(std::vector<Shape *> shapes);
    BVHNode* BuildBVHTree(std::vector<Shape *> shapes);
    Hit getIntersect(Ray ray, BVHNode* node);
    bool checkBounding(Ray ray, BoundingBox box);
};


#endif // BVH_H