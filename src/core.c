#include "../include/core.h"

Vec3 Vec3_add(Vec3 a, Vec3 b){
    return VEC3(a.x+b.x, a.y+b.y, a.z+b.z);
}

Vec3 Vec3_add_scaled(Vec3 a, real s, Vec3 b){
    return VEC3(a.x + s * b.x, a.y + s * b.y, a.z + s * b.z);
}

Vec3 Vec3_sub(Vec3 a, Vec3 b){
    return VEC3(a.x-b.x, a.y-b.y, a.z-b.z);
}

Vec3 Vec3_invert(Vec3 v){
    return VEC3(-v.x, -v.y, -v.z);
}

Vec3 Vec3_scale(Vec3 v, real s){
    return VEC3(v.x * s, v.y * s, v.z * s);
}

Vec3 Vec3_product(Vec3 a, Vec3 b){
    return VEC3(a.x * b.x, a.y * b.y, a.z * b.z);
}

real Vec3_dot(Vec3 a, Vec3 b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 Vec3_cross(Vec3 a, Vec3 b){
    return VEC3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y-a.y*b.x);
}

real Vec3_magnitude(Vec3 v){
    real m = Vec3_dot(v,v);
    if(m == 0)
        return m;
    return real_sqrt(m);
}

Vec3 Vec3_normalized(Vec3 v){
    real m = Vec3_magnitude(v);
    if(m != 0)
        return VEC3(v.x / m, v.y / m, v.z / m);
    return VEC3_ZERO;
}
