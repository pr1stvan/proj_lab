#include "ParticleArrayLoader.h"

bool ParticleArrayLoader::getParticleCountFromFirstFile(std::string filePath, 
    int& particleCount)
{
    Partio::ParticlesDataMutable* particlesData = 
        Partio::read(filePath.c_str());

    if ((particlesData = Partio::read(filePath.c_str())) == NULL)
    {
        return false;
    }
    
    particleCount = particlesData->numParticles();
    particlesData->release();
    return true;
}

bool ParticleArrayLoader::loadParticlesFromFile(
    std::vector<Particle> &particleArray,
    std::string filePath)
{
    Partio::ParticlesDataMutable* particlesData;
    
    if ((particlesData = Partio::read(filePath.c_str())) == NULL)
    {
        return false;
    }

    particleArray = std::vector<Particle>(particlesData->numParticles());
    Partio::ParticleAttribute positionAttribute;
    Partio::ParticleAttribute velocityAttribute;

    if (!particlesData->attributeInfo("position", positionAttribute))
    {
        printf("Failed to find position attribute\n");
    }

    if (!particlesData->attributeInfo("velocity", velocityAttribute))
    {
        printf("Failed to find velocity attribute\n");
    }

    for (int i = 0; i < particlesData->numParticles(); i++)
    {
        const float* pos = particlesData->data<float>(positionAttribute, i);
        const float* v = particlesData->data<float>(velocityAttribute, i);
        particleArray[i].pos = glm::vec3(pos[0], pos[1], pos[2]);
        particleArray[i].v = glm::vec3(v[0], v[1], v[2]);
        particleArray[i].color = glm::vec3(1, 1, 1);
    }

    particlesData->release();
    return true;
}
