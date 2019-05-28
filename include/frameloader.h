#pragma once

#include <string>
#include <vector>

#include "particle.h"

class FrameLoader 
{
protected:
    std::vector<std::string> fileNames;
	int particleCountPerFrame;
public:
	int getParticleCountPerFrame(int index);
	virtual ~FrameLoader();
	char* getFileFolder();

	std::string getFileNameWithoutNumberFromFolderPath(char *folderPath);
	std::string createFilePath(char *folderPath, std::string fileName, int fileNumber);

    virtual bool getFrame(unsigned int index, std::vector<Particle>& particleArray) = 0;
    virtual bool loadFiles(const std::vector<std::string>& fileNames) = 0;
};
