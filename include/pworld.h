#ifndef PHUSIKOS_PARTICLE_WORLD_H
#define PHUSIKOS_PARTICLE_WORLD_H

#include "core.h"
#include "particle.h"
#include "pfgen.h"
#include "pcontacts.h"
#include "plinks.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct ParticleEntry {
    Particle* particle;
    struct ParticleEntry* next;
} ParticleEntry;

typedef struct PCGenEntry {
    PCGen* generator;
    struct PCGenEntry* next;
} PCGenEntry;

typedef struct{
    PFGenRegistry registry;
    PCResolver resolver;
    bool calculate_iterations;

    ParticleEntry* first_particle;
    PCGenEntry* first_contact_gen;

    ParticleContact* contacts;
    size_t max_contacts;
} ParticleWorld;

void ParticleWorld_setup(ParticleWorld*,size_t,unsigned int);
size_t ParticleWorld_generate_contacts(ParticleWorld*);
void ParticleWorld_integrate(ParticleWorld*,real);
void ParticleWorld_run_physics(ParticleWorld*,real);
void ParticleWorld_add_particle(ParticleWorld*,ParticleEntry*);
void ParticleWorld_destroy(ParticleWorld*);

#endif
