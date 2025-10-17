#ifndef PHUSIKOS_PARTICLE_H
#define PHUSIKOS_PARTICLE_H

#include "core.h"

typedef struct{
    Vec3 position;
    Vec3 velocity;
    Vec3 acceleration;
    Vec3 force_accum;
    real damping;
    real _inv_mass;
} Particle;

#define PARTICLE(mass, pos, vel, accel, damp) (Particle){(pos),(vel),(accel),VEC3_ZERO,(damp),1.f/(mass)}

#define SET_MASS(particle, mass) do{ (particle)._inv_mass = 1.f/(mass); } while(0)
#define SET_INV_MASS(particle, inv_mass) do{ (particle)._inv_mass = (mass); } while(0)

void Particle_add_force(Particle*,Vec3);

void Particle_integrate(Particle*,real);

#endif
