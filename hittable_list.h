#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable{
    public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list(){}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void add(shared_ptr<hittable> object) { objects.push_back(object); }
    void clear() { objects.clear(); }

    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
};

bool hittable_list::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
    hit_record tempRec;
    bool hitAnything = false;
    auto closestSoFar = tmax;

    for (const auto& object : objects){
        if (object->hit(r, tmin, closestSoFar, tempRec)){
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }
    return hitAnything;
}

#endif