#pragma once

#include <vector>

#include "Camera.h"
#include "LightSource.h"
#include "ParticleSystem.h"

struct Model {
	LightSource lightSource1;
	LightSource lightSource2;

	Camera camera;
	ParticleSystem* particleSystem;

	Model();
	void animate(float dt);
	void initialize();
	void cleanUp();

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraOrientation();
	float getCameraViewAngle();

	LightSource getLightSource1();
	LightSource getLightSource2();

	const std::vector<Particle>& getOriginalParticles();
	const std::vector<Particle>& getGeneratedParticles();

	int getOriginalParticleCount();
	int getGeneratedParticleCount();

	int getFileNumber();

	float getCubeLength() {
		return particleSystem->cubeLength;
	}
};
