#ifndef PHUSIKOS_PARTICLE_CONTACTS_H
#define PHUSIKOS_PARTICLE_CONTACTS_H

#include "core.h"
#include "particle.h"

#include <stdlib.h>

// Particle Contact Data
typedef struct{
    Particle* a;
    Particle* b;
    Vec3 contact_normal;
    real restitution;
    real penetration;
} ParticleContact;

real ParticleContact_calculate_Vs(ParticleContact*);
void ParticleContact_resolve(ParticleContact*,real);

// Particle Contact Resolver
typedef struct{
    unsigned int iterations;
    unsigned int iterations_used;
} PCResolver;
#define PC_RESOLVER(iter) (PCResolver){(iter), 0}

void PCResolver_resolve(PCResolver*,ParticleContact*,size_t,real);

// Particle Contact Generator
#define PCGEN_BASE struct{ size_t (*add_contact)(void*,ParticleContact*,size_t); }
typedef PCGEN_BASE PCGen;

#endif
