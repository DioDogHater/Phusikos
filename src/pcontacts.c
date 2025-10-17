#include "../include/pcontacts.h"

static void resolve_velocity(ParticleContact*,real);
static void resolve_interpenetration(ParticleContact*,real);

void ParticleContact_resolve(ParticleContact* contact, real duration){
    resolve_velocity(contact, duration);
    resolve_interpenetration(contact, duration);
}

real ParticleContact_calculate_Vs(ParticleContact* contact){
    Vec3 relative_velocity = contact->a->velocity;
    if(contact->b)
        relative_velocity = Vec3_sub(relative_velocity, contact->b->velocity);
    return Vec3_dot(relative_velocity, contact->contact_normal);
}

static void resolve_velocity(ParticleContact* contact, real duration){
    real Vs = ParticleContact_calculate_Vs(contact);
    if(Vs > 0)
        return;
    real new_Vs = -contact->restitution * Vs;

    Vec3 acc_caused_velocity = contact->a->acceleration;
    if(contact->b)
        acc_caused_velocity = Vec3_sub(acc_caused_velocity, contact->b->acceleration);

    real acc_caused_Vs = Vec3_dot(acc_caused_velocity, contact->contact_normal) * duration;
    if(acc_caused_Vs < 0){
        new_Vs += contact->restitution * acc_caused_Vs;
        if(new_Vs < 0)
            new_Vs = 0.f;
    }

    real delta_v = new_Vs - Vs;
    real total_inverse_mass = contact->a->_inv_mass;
    if(contact->b)
        total_inverse_mass += contact->b->_inv_mass;

    if(total_inverse_mass <= 0)
        return;

    Vec3 impulse_per_imass = Vec3_scale(contact->contact_normal, delta_v / total_inverse_mass);

    contact->a->velocity = Vec3_add_scaled(contact->a->velocity, contact->a->_inv_mass, impulse_per_imass);
    if(contact->b)
        contact->b->velocity = Vec3_add_scaled(contact->b->velocity, -contact->b->_inv_mass, impulse_per_imass);
}

static void resolve_interpenetration(ParticleContact* contact, real duration){
    if(contact->penetration <= 0)
        return;

    real total_inverse_mass = contact->a->_inv_mass;
    if(contact->b)
        total_inverse_mass += contact->b->_inv_mass;

    if(total_inverse_mass <= 0)
        return;

    Vec3 move_per_imass = Vec3_scale(contact->contact_normal, (-contact->penetration) / total_inverse_mass);

    contact->a->position = Vec3_add_scaled(contact->a->position, contact->a->_inv_mass, move_per_imass);
    if(contact->b)
        contact->b->position = Vec3_add_scaled(contact->b->position, -contact->b->_inv_mass, move_per_imass);
}


void PCResolver_resolve(PCResolver* resolver,
                        ParticleContact* contacts,
                        size_t contact_count,
                        real duration){
    resolver->iterations_used = 0;
    while(resolver->iterations_used < resolver->iterations){
        real max = 0.f;
        size_t max_index = contact_count;
        for(size_t i = 0; i < contact_count; i++){
            real Vs = ParticleContact_calculate_Vs(&contacts[i]);
            if(Vs > max){
                max = Vs;
                max_index = i;
            }
        }
        if(max_index == contact_count)
            break;
        ParticleContact_resolve(&contacts[max_index], duration);
        resolver->iterations_used++;
    }
}
