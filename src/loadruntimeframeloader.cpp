#include "LoadRuntimeFrameLoader.h"

bool LoadRuntimeFrameLoader::load(char *fileFolder)
{
	printf("\nLoadRuntime::load\n\n");
	sprintf(file, "Nem tudom betolteni a fajlt.");//Alapertelemezett ertek

	std::string fileName = getFileNameWithoutNumberFromFolderPath(fileFolder);
	int fileNumber = 0;

	std::string filePath = createFilePath(fileFolder, fileName, fileNumber);

	if (ParticleArrayLoader::getParticleCountFromFirstFile(filePath, particleCountPerFrame))
	{
		sprintf(file, "%s", fileFolder);//Ha valamit sikerult betolteni, a folder path -ot mar vissza tudja adni az antweakBarnak a loader
	}
	else return false;//Nem sikeres a beolvasas, mivel meg az elso fajlt sem sikerult

	return true;
}

bool LoadRuntimeFrameLoader::getFrame(unsigned int index, std::vector<Particle>& particleArrayOut)
{
	std::string fileName = getFileNameWithoutNumberFromFolderPath(file);
	std::string filePath1 = createFilePath(file, fileName, index);
	std::string filePath2 = createFilePath(file, fileName, index + 1);

	std::vector<Particle> particleArrayNextFile;
	bool success = false;
	if (!ParticleArrayLoader::loadParticlesFromFile(particleArrayOut, filePath1))
	{
		particleArrayOut = std::vector<Particle>(1);
		particleArrayOut[0].color = glm::vec3(1, 0, 0);
		particleArrayOut[0].pos = glm::vec3(0, 0, 0);
		return false;
	}
	printf("Number of particles: %d\n", particleArrayOut.size());
	if (ParticleArrayLoader::loadParticlesFromFile(particleArrayNextFile, filePath2))
	{
		success = true;
	}
	//Modify velocities, because the files was corrupted
	for (int i = 0; i < particleCountPerFrame; i++)
	{
		particleArrayOut[i].v = success ? particleArrayNextFile[i].pos - particleArrayOut[i].pos : glm::vec3(0, 0, 0);
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
