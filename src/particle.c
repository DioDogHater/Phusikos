#include "../include/particle.h"

void Particle_add_force(Particle* particle, Vec3 force){ particle->force_accum = Vec3_add(particle->force_accum, force); }

void Particle_integrate(Particle* particle, real duration){
    if(duration <= 0.f)
        return;

    // Update position
    particle->position = Vec3_add_scaled(particle->position, duration, particle->velocity);

    // Resulting acceleration with forces applied
    Vec3 resulting_accel = Vec3_add_scaled(particle->acceleration, particle->_inv_mass, particle->force_accum);
    particle->force_accum = VEC3_ZERO;

    // Update velocity
    particle->position = Vec3_add_scaled(particle->position, duration, resulting_accel);

    // Dampening
    particle->velocity = Vec3_scale(particle->velocity, real_pow(particle->damping, duration));
}
