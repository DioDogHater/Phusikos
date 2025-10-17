#ifndef PHUSIKOS_CORE_H
#define PHUSIKOS_CORE_H

#include <math.h>

typedef float real;

#define real_sqrt sqrtf
#define real_pow powf
#define real_abs fabsf
#define real_exp expf
#define real_cos cosf
#define real_acos acosf
#define real_sin sinf
#define real_asin asinf
#define real_tan tanf
#define real_atan2 atan2f

typedef struct{
    real x, y, z;
    real __pad;
} Vec3;

#define VEC3_ZERO (Vec3){0.f,0.f,0.f}
#define VEC3(x, y, z) (Vec3){(x),(y),(z)}

Vec3 Vec3_add(Vec3,Vec3);
Vec3 Vec3_add_scaled(Vec3,real,Vec3);
Vec3 Vec3_sub(Vec3,Vec3);

Vec3 Vec3_invert(Vec3);
Vec3 Vec3_scale(Vec3,real);
Vec3 Vec3_product(Vec3,Vec3);   // Component product

real Vec3_dot(Vec3,Vec3);
Vec3 Vec3_cross(Vec3,Vec3);

real Vec3_magnitude(Vec3);
Vec3 Vec3_normalized(Vec3);

#endif
