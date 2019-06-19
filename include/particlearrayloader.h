#pragma once

#include <string>
#include <vector>

#include "Partio.h"

#include "Particle.h"

class ParticleArrayLoader
{
public:
    static bool loadParticlesFromFile(std::vector<Particle> &particleArray,
                                      std::string filePath);
    static bool getParticleCountFromFirstFile(std::string filePath,
                                              int& particleCount);
};
