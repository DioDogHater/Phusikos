#ifndef PHUSIKOS_PARTICLE_FORCE_GEN_H
#define PHUSIKOS_PARTICLE_FORCE_GEN_H

#include <stdlib.h>
#include "core.h"
#include "particle.h"

// Particle Force Generator
#define PFGEN_BASE struct{ void (*update)(void*,Particle*,real); }
typedef struct PFGen{
    PFGEN_BASE;
} PFGen;

// Partice Force Generator Entry
typedef struct PFGenEntry{
    Particle* particle;
    PFGen* generator;
} PFGenEntry;

// Particle Force Generator Registry
typedef struct PFGenRegistry{
    size_t size;
    size_t _memsize;
    PFGenEntry* entries;
    size_t __pad;
} PFGenRegistry;
#define PFGEN_REGISTRY_START 64
#define PFGEN_REGISTRY_GROWTH(n) (2*(n))
#define PFGEN_REGISTRY() (PFGenRegistry){0,0,NULL,0}
void PFGenRegistry_add(PFGenRegistry*,Particle*,PFGen*);
void PFGenRegistry_remove(PFGenRegistry*,Particle*,PFGen*);
void PFGenRegistry_clear(PFGenRegistry*);
void PFGenRegistry_update(PFGenRegistry*,real);

// Particle Gravity
typedef struct{
    PFGEN_BASE;
    Vec3 gravity;
} ParticleGravity;
void ParticleGravity_update(void*,Particle*,real);
#define PARTICLE_GRAVITY(g) (ParticleGravity){ParticleGravity_update,(g)}

// Particle Drag
typedef struct{
    PFGEN_BASE;
    real k1, k2;
} ParticleDrag;
void ParticleDrag_update(void*,Particle*,real);
#define PARTICLE_DRAG(k1,k2) (ParticleDrag){ParticleDrag_update,(k1),(k2)}

// Particle Spring
typedef struct{
    PFGEN_BASE;
    Particle* other;
    real spring_const, rest_length;
} ParticleSpring;
void ParticleSpring_update(void*,Particle*,real);
#define PARTICLE_SPRING(other,k,l0) (ParticleSpring){ParticleSpring_update,(other),(k),(l0)}

// Particle Anchored Spring
typedef struct{
    PFGEN_BASE;
    Vec3* anchor;
    real spring_const, rest_length;
} ParticleAnchoredSpring;
void ParticleAnchoredSpring_update(void*,Particle*,real);
#define PARTICLE_ANCHORED_SPRING(anchor,k,l0) (ParticleSpring){ParticleAnchoredSpring_update,(anchor),(k),(l0)}

// Particle Bungee
typedef struct{
    PFGEN_BASE;
    Vec3* anchor;
    real spring_const, rest_length;
} ParticleBungee;
void ParticleBungee_update(void*,Particle*,real);
#define PARTICLE_BUNGEE(anchor,k,l0) (ParticleBungee){ParticleBungee_update,(anchor),(k),(l0)}

// Particle Buoyancy
typedef struct{
    PFGEN_BASE;
    real max_depth, volume;
    real liquid_height, liquid_density;
} ParticleBuoyancy;
void ParticleBuoyancy_update(void*,Particle*,real);
#define PARTICLE_BUOYANCY(s,V,yw,density) (ParticleBuoyancy){ParticleBuoyancy_update,(s),(V),(yw),(density)}
#define WATER_DENSITY 1000.f

// Particle Fake Spring
typedef struct{
    PFGEN_BASE;
    Vec3* anchor;
    real spring_const, damping;
} ParticleFakeSpring;
void ParticleFakeSpring_update(void*,Particle*,real);
#define PARTICLE_FAKE_SPRING(anchor,k,damping) (ParticleFakeSpring){ParticleFakeSpring_update,(anchor),(k),(damping)}

#endif
