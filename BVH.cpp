#include "BVH.h"


BVH::BVH() {
    root = nullptr;
}

BVH::BVH(std::vector<Shape *> shapes){
    root = BuildBVHTree(shapes);
}

BVHNode* BVH::BuildBVHTree(std::vector<Shape *> shapes){
    BVHNode* node = new BVHNode;
    node->shape = nullptr;
    node->hasShape = false;
    node->isEmpty = false;
    node->left = nullptr;
    node->right = nullptr;
    if(shapes.size() == 0){
        node->isEmpty = true;
        return node;
    }
    if(shapes.size() == 1){
        Shape* shape = shapes[0];
        node->shape = shape;
        node->box.min = shape->bbox.min;
        node->box.max = shape->bbox.max;
        node->hasShape = true;
        return node;
    }

    // Split the shapes into two parts
    std::vector<Shape *> left;
    std::vector<Shape *> right;
    // Skip the complex splitting process if only two shapes
    if(shapes.size() == 2){
        left.push_back(shapes.at(0));
        right.push_back(shapes.at(1));
    }else{ 
        // Find the left most shape and right most shape;
        Shape* left_most = shapes.at(0);
        Shape* right_most = shapes.at(0);

        // Try splitting by x
        for(auto it = shapes.begin(); it != shapes.end(); ++it){
            Shape* currShape = *it;
            if(currShape->getCenter().x() < left_most->getCenter().x()){
                left_most = currShape;
            }
            if(currShape->getCenter().x() > right_most->getCenter().x()){
                right_most = currShape;
            }
        }
        
        // Split again by y if all shapes have same x
        if(left_most->getCenter().x() == right_most->getCenter().x()){
            for(auto it = shapes.begin(); it != shapes.end(); ++it){
                Shape* currShape = *it;
                if(currShape->getCenter().y() < left_most->getCenter().y()){
                    left_most = currShape;
                }
                if(currShape->getCenter().y() > right_most->getCenter().y()){
                    right_most = currShape;
                }
            }
        }

        // If all shapes has same x and y, split again by z
        if(left_most->getCenter().y() == right_most->getCenter().y()){
            for(auto it = shapes.begin(); it != shapes.end(); ++it){
                Shape* currShape = *it;
                if(currShape->getCenter().z() < left_most->getCenter().z()){
                    left_most = currShape;
                }
                if(currShape->getCenter().z() > right_most->getCenter().z()){
                    right_most = currShape;
                }
            }
        }

        // In the worst case that two shapes have the same center, let right be next
        if(left_most->getCenter() == right_most->getCenter()){
            right_most = shapes.at(1);
        }

        // Split the shapes into two groups
        bool toggle = false;
        for(auto it = shapes.begin(); it != shapes.end(); ++it){
            Shape* currShape = *it;
            if(currShape == left_most || currShape == right_most){
                continue;
            }
            if(currShape->getCenter().x() < right_most->getCenter().x()){
                left.push_back(currShape);
            }else if(currShape->getCenter().x() > right_most->getCenter().x()){
                right.push_back(currShape);
            }else{
                // If the shape's center is the same as the right_most's center,
                // distribute it to the left and right nodes alternately
                if(toggle){
                    left.push_back(currShape);
                }else{
                    right.push_back(currShape);
                }
                toggle = !toggle;
            }
        }

        // Make sure left and right both have at least one shape
        if(left.size() == 0){
            left.push_back(right_most);
            right.pop_back();
        }else if(right.size() == 0){
            right.push_back(left_most);
            left.pop_back();
        }
    }

    // Recursively build the BVH tree
    node->left = BuildBVHTree(left);
    node->right = BuildBVHTree(right);

    // Compute the bounding box of this node
    node->box.min = Vec3::min(node->left->box.min, node->right->box.min);
    node->box.max = Vec3::max(node->left->box.max, node->right->box.max);

    return node;
    
}

Hit BVH::getIntersect(Ray ray, BVHNode* node){
    Hit noHit;

    if (node == nullptr || !checkBounding(ray, node->box)) {
        return noHit;
    }

    if (node->hasShape) {
        Hit hit;
        node->shape->intersect(ray, hit, nullptr);
        if (hit.t() >= 0) {
            return hit;
        } else {
            return noHit;
        }
    }

    Hit leftHit = getIntersect(ray, node->left);
    Hit rightHit = getIntersect(ray, node->right);

    if (leftHit.t() < 0) return rightHit;
    if (rightHit.t() < 0) return leftHit;

    if (leftHit.t() < rightHit.t()) return leftHit;
    else return rightHit;
}

bool BVH::checkBounding(Ray ray, BoundingBox box) {
    // Compute the intersection times of the ray with the x-slab of the bounding box
    double tmin = (box.min.x() - ray.getOrigin().x()) / ray.getDirection().x();
    double tmax = (box.max.x() - ray.getOrigin().x()) / ray.getDirection().x();

    // If tmin > tmax, swap them
    if (tmin > tmax) std::swap(tmin, tmax);

    // Compute the intersection times of the ray with the y-slab of the bounding box
    double tymin = (box.min.y() - ray.getOrigin().y()) / ray.getDirection().y();
    double tymax = (box.max.y() - ray.getOrigin().y()) / ray.getDirection().y();

    // If tymin > tymax, swap them
    if (tymin > tymax) std::swap(tymin, tymax);

    // If the intervals [tmin, tmax] and [tymin, tymax] do not overlap, return false
    if ((tmin > tymax) || (tymin > tmax))
        return false;

    // Update tmin and tmax to the overlap of the intervals [tmin, tmax] and [tymin, tymax]
    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    // Compute the intersection times of the ray with the z-slab of the bounding box
    double tzmin = (box.min.z() - ray.getOrigin().z()) / ray.getDirection().z();
    double tzmax = (box.max.z() - ray.getOrigin().z()) / ray.getDirection().z();

    // If tzmin > tzmax, swap them
    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    // If the intervals [tmin, tmax] and [tzmin, tzmax] do not overlap, return false
    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    // If we reach this point, the ray intersects the bounding box
    return true;
}