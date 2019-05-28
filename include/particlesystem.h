#pragma once

#include <vector>

#include "glm.hpp"
#include "gtx/rotate_vector.hpp"

#include "Particle.h"
#include "LoadBeforeFrameLoader.h"
#include "LoadRuntimeFrameLoader.h"
#include "Array3d.h"
#include "functions.h"

class ParticleSystem 
{
private:
	float R = 10;
	std::vector<Particle> particles;
	std::vector<Particle> generatedParticles;
	int fileNumber = 0;
	bool isFileLoaded=false;
    unsigned int genPerParticleNotAntweakbar = 0;
public:
	float cubeLength = 11.0f;
	bool pause = false;
	bool loadMode = true;
	bool onlyFileParticles = false;
	bool onlyFileParticlesRuntime = false;
	int genPerParticle = 10;
	//best name ever
	float atwR=0.5f;
    float maxFps = 1000.0f;
	glm::vec3 generatedParticleColor;
	int generatedParticleFile = 0;
	bool ignoreNullVelocity=true;
	bool followControllPoints = false;
	float randomR = 0.7f;
	float distanceMultiplier = 1.0f;
	float triggerDistance = 1.5f;
	int velocityArrayParts = 20;
	Array3d * velocityArray = NULL;
	FrameLoader *loader;
	float secs = 0;
	bool end = false;

	ParticleSystem();
	~ParticleSystem();
	float getCubeLength();
	void deleteVelocityArray();
	void generateParticles(int fileNumber);
    bool loadFiles(std::vector<std::string> fileNames);
	void initialize(); 
	void Animate(float t);
	int getFileNumber();
	int getOriginalParticleCount();
	int getGeneratedParitlceCount();
	const std::vector<Particle>& getOriginalParticles();
	const std::vector<Particle>& getGeneratedParticles();
};
