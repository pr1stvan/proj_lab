#pragma once

#include <vector>
#include "Particle.h"
#include "FrameLoader.h"
#include "ParticleArrayLoader.h"

class LoadBeforeFrameLoader : public FrameLoader
{
private:
	std::vector<std::vector<Particle>> frames;//elore betolteshez
public:
	int loadMode = 0;

	void deleteFrames();
	~LoadBeforeFrameLoader();
	void deleteFrame(Particle* array);
	char* getFileFolder();
    bool getFrame(unsigned int index, std::vector<Particle>& particleArray);
	bool load(char *fileFolder);
	void calculateVelocities();
};
