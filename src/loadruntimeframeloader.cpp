#include "LoadRuntimeFrameLoader.h"

bool LoadRuntimeFrameLoader::loadFiles(const std::vector<std::string>& files)
{
    this->fileNames = files;
    if(fileNames.size() < 1)
    {
        return false;
    }
	printf("\nLoadRuntime::load\n\n");

    if (!ParticleArrayLoader::getParticleCountFromFirstFile(fileNames[0], particleCountPerFrame))
	{
        return false;
    }

	return true;
}

bool LoadRuntimeFrameLoader::getFrame(unsigned int index, std::vector<Particle>& particleArrayOut)
{
    if(index > fileNames.size() -1)
    {
        particleArrayOut = std::vector<Particle>(1);
        particleArrayOut[0].color = glm::vec3(1, 0, 0);
        particleArrayOut[0].pos = glm::vec3(0, 0, 0);
        return false;
    }
    else if(index == fileNames.size() - 1)
    {
        if(!ParticleArrayLoader::loadParticlesFromFile(particleArrayOut, fileNames[index]))
        {
            particleArrayOut = std::vector<Particle>(1);
            particleArrayOut[0].color = glm::vec3(1, 0, 0);
            particleArrayOut[0].pos = glm::vec3(0, 0, 0);
            return false;
        }
        printf("Number of particles: %d\n", particleArrayOut.size());

        for (int i = 0; i < particleCountPerFrame; i++)
        {
            particleArrayOut[i].v = glm::vec3(0, 0, 0);
        }
    }
    else
    {
        std::vector<Particle> particleArrayNextFile;

        if(!ParticleArrayLoader::loadParticlesFromFile(particleArrayOut, fileNames[index]) ||
                !ParticleArrayLoader::loadParticlesFromFile(particleArrayNextFile, fileNames[index + 1]))
        {
            particleArrayOut = std::vector<Particle>(1);
            particleArrayOut[0].color = glm::vec3(1, 0, 0);
            particleArrayOut[0].pos = glm::vec3(0, 0, 0);
            return false;
        }
        printf("Number of particles: %d\n", particleArrayOut.size());

        for (int i = 0; i < particleCountPerFrame; i++)
        {
            particleArrayOut[i].v = particleArrayNextFile[i].pos - particleArrayOut[i].pos;
        }
    }
	return true;
}

LoadRuntimeFrameLoader::~LoadRuntimeFrameLoader()
{
	//Do nothing
}

void LoadRuntimeFrameLoader::deleteFrame(Particle* array)
{
	if (array != NULL)
	{
		delete[] array;
	}
}
