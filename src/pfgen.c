#include "../include/pfgen.h"
#include <string.h>

void PFGenRegistry_add(PFGenRegistry* reg, Particle* particle, PFGen* gen){
    if(!reg || !particle || !gen)
        return;
    if(!reg->_memsize || !reg->entries){
        reg->entries = (PFGenEntry*) malloc(sizeof(PFGenEntry) * PFGEN_REGISTRY_START);
        reg->_memsize = PFGEN_REGISTRY_START;
        reg->size = 0;
    }else if(reg->_memsize - reg->size == 0){
        reg->_memsize = PFGEN_REGISTRY_GROWTH(reg->_memsize);
        reg->entries = (PFGenEntry*) realloc(reg->entries, sizeof(PFGenEntry) * reg->_memsize);
    }
    reg->entries[reg->size] = (PFGenEntry){particle, gen};
    reg->size++;
}

void PFGenRegistry_remove(PFGenRegistry* reg, Particle* particle, PFGen* gen){
    if(!reg || !particle || !gen)
        return;

    for(size_t i = 0; i < reg->size; i++){
        if(reg->entries[i].particle == particle && reg->entries[i].generator == gen){
            reg->size--;
            if(i < reg->size)
                memmove(&reg->entries[i],&reg->entries[i+1],(reg->size - i) * sizeof(PFGenEntry));
            return;
        }
    }
}

void PFGenRegistry_clear(PFGenRegistry* reg){
    if(!reg)
        return;

    if(reg->entries)
        free(reg->entries);

    reg->_memsize = 0;
    reg->size = 0;
}

void PFGenRegistry_update(PFGenRegistry* reg, real duration){
    if(!reg)
        return;

    for(size_t i = 0; i < reg->size; i++){
        PFGenEntry* entry = &reg->entries[i];
        entry->generator->update(entry->generator,entry->particle,duration);
    }
}

void ParticleGravity_update(void* gen, Particle* particle, real duration){
    if(particle->_inv_mass == 0.f)
        return;

    ParticleGravity* gravity_gen = gen;
    Particle_add_force(particle, Vec3_scale(gravity_gen->gravity, 1.f/particle->_inv_mass));
}

void ParticleDrag_update(void* gen, Particle* particle, real duration){
    ParticleDrag* drag_gen = gen;

    Vec3 force = particle->velocity;
    real drag_coeff = Vec3_magnitude(force);
    drag_coeff = drag_gen->k1 * drag_coeff + drag_gen->k2 * drag_coeff * drag_coeff;

    force = Vec3_scale(Vec3_normalized(force), -drag_coeff);
    Particle_add_force(particle, force);
}

static Vec3 get_spring_force(Vec3 other, Particle* particle, real spring_const, real rest_length){
    Vec3 force = particle->position;
    force = Vec3_sub(force, other);

    real magnitude = Vec3_magnitude(force);
    if(magnitude != 0.f)
        force = Vec3_scale(force, 1.f/magnitude);
    magnitude = real_abs(magnitude - rest_length) * spring_const;

    return Vec3_scale(force, -magnitude);
}

void ParticleSpring_update(void* gen, Particle* particle, real duration){
    ParticleSpring* spring_gen = gen;

    Vec3 force = get_spring_force(spring_gen->other->position, particle, spring_gen->spring_const, spring_gen->rest_length);
    Particle_add_force(particle, force);

    force = Vec3_invert(force);
    Particle_add_force(spring_gen->other, force);
}

void ParticleAnchoredSpring_update(void* gen, Particle* particle, real duration){
    ParticleAnchoredSpring* aspring_gen = gen;
    Particle_add_force(particle, get_spring_force(*aspring_gen->anchor, particle, aspring_gen->spring_const, aspring_gen->rest_length));
}

void ParticleBungee_update(void* gen, Particle* particle, real duration){
    ParticleBungee* bungee_gen = gen;

    Vec3 force = particle->position;
    force = Vec3_sub(force, *bungee_gen->anchor);

    real magnitude = Vec3_magnitude(force);
    if(magnitude <= bungee_gen->rest_length)
        return;

    force = Vec3_scale(force, 1.f/magnitude);
    magnitude = bungee_gen->spring_const * (bungee_gen->rest_length - magnitude);

    Particle_add_force(particle, Vec3_scale(force, -magnitude));
}

void ParticleBuoyancy_update(void* gen, Particle* particle, real duration){
    ParticleBuoyancy* buoy_gen = gen;

    real depth = particle->position.y;
    if(depth >= buoy_gen->liquid_height + buoy_gen->max_depth)
        return;

    Vec3 force = VEC3_ZERO;
    if(depth <= buoy_gen->liquid_height - buoy_gen->max_depth)
        force.y = buoy_gen->liquid_density * buoy_gen->volume;
    else
        force.y = (buoy_gen->liquid_density * buoy_gen->volume * (depth - buoy_gen->max_depth - buoy_gen->liquid_height)) / (2 * buoy_gen->max_depth);

    Particle_add_force(particle, force);
}

void ParticleFakeSpring_update(void* gen, Particle* particle, real duration){
    if(particle->_inv_mass == 0.f)
        return;

    ParticleFakeSpring* fspring_gen = gen;
    Vec3 position = Vec3_sub(particle->position, *fspring_gen->anchor);

    real gamma = 0.5f * real_sqrt(4 * fspring_gen->spring_const - fspring_gen->damping * fspring_gen->damping);
    if(gamma == 0.f)
        return;

    Vec3 c = Vec3_scale(position, (fspring_gen->damping / (2.f * gamma)));
    c = Vec3_add_scaled(c, 1.f/gamma, particle->velocity);

    Vec3 target = Vec3_scale(position, real_cos(gamma * duration));
    target = Vec3_add_scaled(target, real_sin(gamma * duration), c);
    target = Vec3_scale(target, real_exp(-0.5f * duration * fspring_gen->damping));

    Vec3 accel = Vec3_scale(Vec3_sub(target, position), 1.f/(duration * duration));
    accel = Vec3_add_scaled(accel, -duration, particle->velocity);

    Particle_add_force(particle, Vec3_scale(accel, 1.f/particle->_inv_mass));
}
