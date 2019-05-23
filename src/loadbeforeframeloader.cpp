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

bool LoadBeforeFrameLoader::load(char *fileFolder)
{
	printf("\nLoadBefore::load\n");
	sprintf(file, "Nem tudom betolteni a fajlt.");//Alapertelemezett ertek

	deleteFrames();

	std::string fileName = getFileNameWithoutNumberFromFolderPath(fileFolder);
	int fileNumber = 0;

	std::string filePath = createFilePath(fileFolder, fileName, fileNumber);

	if (ParticleArrayLoader::getParticleCountFromFirstFile(std::string(filePath), particleCountPerFrame))
	{
		sprintf(file, "%s", fileFolder);//Ha valamit sikerult betolteni, a folder path -ot mar vissza tudja adni az antweakBarnak a loader
	}
	else return false;//Nem sikeres a beolvasas, mivel meg az elso fajlt sem sikerult

	while (true)
	{
		std::vector<Particle> particleArray;
		if (!ParticleArrayLoader::loadParticlesFromFile(particleArray, filePath))
		{
			break;
		}
		printf("Number of particles: %d\n", particleArray.size());
		frames.push_back(particleArray);
		//=========================================================
		fileNumber++;
		filePath = createFilePath(fileFolder, fileName, fileNumber);
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
