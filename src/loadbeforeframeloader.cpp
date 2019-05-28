#include "LoadBeforeFrameLoader.h"

LoadBeforeFrameLoader::~LoadBeforeFrameLoader()
{
	deleteFrames();
}
bool LoadBeforeFrameLoader::getFrame(unsigned int index, std::vector<Particle>& particleArray)
{
	if (index >= frames.size())
	{
		particleArray = std::vector<Particle>(1);
		particleArray[0].color = glm::vec3(1, 0, 0);
		particleArray[0].pos = glm::vec3(0, 0, 0);
		return false;
	}
	particleArray = frames[index];
	return true;
}

void LoadBeforeFrameLoader::calculateVelocities()
{
	if (frames.size() > 0)
	{
		std::vector<Particle>& particles = frames[frames.size() - 1];
        for (unsigned int i = 0; i < particles.size(); i++)
		{
			particles[i].v = glm::vec3(0, 0, 0);
		}
	}
    for (unsigned int i = 0; i <frames.size() -1; i++)
	{
		std::vector<Particle>& particles = frames[i];
		std::vector<Particle>& particlesNextFile = frames[i+1];
		
        for (unsigned int j = 0; j<particles.size() && j<particlesNextFile.size(); j++)
		{
			particles[j].v = particlesNextFile[j].pos - particles[j].pos;
		}
	}
}

bool LoadBeforeFrameLoader::loadFiles(const std::vector<std::string>& files)
{
    this->fileNames = files;
    if(fileNames.size() < 1)
    {
        return false; //Can't load nothing
    }
	printf("\nLoadBefore::load\n");

    deleteFrames();

    if(!ParticleArrayLoader::getParticleCountFromFirstFile(std::string(fileNames[0]), particleCountPerFrame))
    {
        return false;
    }

    for(unsigned i = 0; i < fileNames.size(); i++)
    {
        std::vector<Particle> particleArray;
        bool succes = ParticleArrayLoader::loadParticlesFromFile(particleArray, fileNames[i]);
        if(succes)
        {
            printf("Number of particles: %d\n", particleArray.size());
            frames.push_back(particleArray);
        }
    }
	printf("\n");
	calculateVelocities();
	return true;
}

void LoadBeforeFrameLoader::deleteFrame(Particle* array)
{
	//Do nothing
}

void LoadBeforeFrameLoader::deleteFrames()
{
	std::vector<std::vector<Particle>> temp(0);
	frames.clear();
	frames.swap(temp);
}
