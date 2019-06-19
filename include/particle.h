#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

#include "glm.hpp"

struct Particle
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec3 v;
    

    Particle() :pos(0, 0, 0), color(1.0f,1.0f,1.0f),v(0, 0, 0)
    {

    }
    Particle(glm::vec3 pos, glm::vec3 v) :pos(pos), v(v)
    {

    }
    void move(float t);

};

#endif // PARTICLE_H_INCLUDED
