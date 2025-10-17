#ifndef PHUSIKOS_PARTICLE_LINKS_H
#define PHUSIKOS_PARTICLE_LINKS_H

#include "../include/core.h"
#include "../include/particle.h"
#include "../include/pcontacts.h"

#include <stdlib.h>

// A link between two particles
#define PARTICLE_LINK_BASE struct{ PCGEN_BASE; Particle *a, *b; }
typedef PARTICLE_LINK_BASE ParticleLink;
real ParticleLink_length(ParticleLink*);

// A cable connecting two particles
typedef struct{
    PARTICLE_LINK_BASE;
    real max_length;
    real restitution;
} ParticleCable;
size_t ParticleCable_fill(void*,ParticleContact*,size_t);
#define PARTICLE_CABLE(a,b,max_len,res) (ParticleCable){ParticleCable_fill,(a),(b),(max_len),(res)}

// A rod connecting two particles
typedef struct{
    PARTICLE_LINK_BASE;
    real length;
} ParticleRod;
size_t ParticleRod_fill(void*,ParticleContact*,size_t);
#define PARTICLE_ROD(a,b,len) (ParticleRod){ParticleRod_fill,(a),(b),(len)}
#define PARTICLE_ROD_LENGTH(a,b) (ParticleRod){ParticleRod_fill,(a),(b),Vec3_magnitude(Vec3_sub((a)->position,(b)->position))}

#endif
