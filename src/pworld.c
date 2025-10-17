#include "../include/pworld.h"

void ParticleWorld_setup(ParticleWorld* world, size_t max_contacts, unsigned int iterations){
    world->first_particle = NULL;
    world->first_contact_gen = NULL;

    world->registry = PFGEN_REGISTRY();

    world->calculate_iterations = (iterations == 0);
    world->resolver = PC_RESOLVER(iterations);

    world->contacts = (ParticleContact*) malloc(sizeof(ParticleContact) * max_contacts);
    world->max_contacts = max_contacts;
}

size_t ParticleWorld_generate_contacts(ParticleWorld* world){
    if(!world || !world->contacts)
        return 0;
    size_t limit = world->max_contacts;
    ParticleContact* contact = world->contacts;
    PCGenEntry* entry = world->first_contact_gen;
    while(entry && limit > 0){
        size_t used = entry->generator->add_contact(entry->generator, contact, limit);
        limit -= used;
        contact += used;
        entry = entry->next;
    }
    return world->max_contacts - limit;
}

void ParticleWorld_integrate(ParticleWorld* world, real duration){
    if(!world)
        return;
    ParticleEntry* entry = world->first_particle;
    while(entry){
        Particle_integrate(entry->particle, duration);
        entry = entry->next;
    }
}

void ParticleWorld_run_physics(ParticleWorld* world, real duration){
    if(!world)
        return;
    PFGenRegistry_update(&world->registry, duration);
    ParticleWorld_integrate(world,duration);
    size_t used_contacts = ParticleWorld_generate_contacts(world);
    if(world->calculate_iterations)
        world->resolver.iterations = used_contacts * 2;
    PCResolver_resolve(&world->resolver, world->contacts, used_contacts, duration);
}


void ParticleWorld_add_particle(ParticleWorld* world, ParticleEntry* particle){
    if(!world || !particle)
        return;
    if(!world->first_particle){
        world->first_particle = particle;
        return;
    }
    ParticleEntry* entry = world->first_particle;
    while(entry->next) entry = entry->next;
    entry->next = particle;
}

void ParticleWorld_add_contact_gen(ParticleWorld* world, PCGenEntry* contact_gen){
    if(!world || !contact_gen)
        return;
    if(!world->first_contact_gen){
        world->first_contact_gen = contact_gen;
        return;
    }
    PCGenEntry* entry = world->first_contact_gen;
    while(entry->next) entry = entry->next;
    entry->next = contact_gen;
}

void ParticleWorld_destroy(ParticleWorld* world){
    if(!world)
        return;
    if(world->contacts)
        free(world->contacts);
    world->max_contacts = 0;
}
