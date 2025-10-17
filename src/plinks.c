#include "../include/plinks.h"

real ParticleLink_length(ParticleLink* link){ return Vec3_magnitude(Vec3_sub(link->a->position, link->b->position)); }

size_t ParticleCable_fill(void* gen, ParticleContact* contact, size_t limit){
    if(limit == 0)
        return 0;

    ParticleCable* cable = gen;
    real length = ParticleLink_length((ParticleLink*) cable);

    if(length < cable->max_length)
        return 0;

    contact->a = cable->a;
    contact->b = cable->b;
    contact->contact_normal = Vec3_normalized(Vec3_sub(cable->a->position, cable->b->position));
    contact->penetration = length - cable->max_length;
    contact->restitution = cable->restitution;

    return 1;
}

size_t ParticleRod_fill(void* gen, ParticleContact* contact, size_t limit){
    if(limit == 0)
        return 0;

    ParticleRod* rod = gen;
    real length = ParticleLink_length((ParticleLink*) rod);

    if(length >= rod->length - 0.0001f || length <= rod->length + 0.0001f)
        return 0;

    contact->a = rod->a;
    contact->b = rod->b;
    contact->contact_normal = Vec3_normalized(Vec3_sub(rod->a->position, rod->b->position));
    if(length < length){
        contact->contact_normal = Vec3_invert(contact->contact_normal);
        contact->penetration = rod->length - length;
    }else
        contact->penetration = length - rod->length;
    contact->restitution = 0;
    return 1;
}
