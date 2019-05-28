#pragma once

#include <vector>
#include "Particle.h"
#include "FrameLoader.h"
#include "ParticleArrayLoader.h"

class LoadRuntimeFrameLoader : public FrameLoader
{
public:
	int loadMode = 0;

	~LoadRuntimeFrameLoader();
	void deleteFrame(Particle* array);
	char* getFileFolder();
    bool getFrame(unsigned int index, std::vector<Particle>& particleArray);
    bool loadFiles(const std::vector<std::string>& fileNames);
};
