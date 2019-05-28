#include "Model.h"

Model::Model() :camera(0, 0, 40)
{
	lightSource1.position = glm::vec3(4, 4, 4);
	lightSource2.position = glm::vec3(-4, 4, -4);
	lightSource1.color = glm::vec3(1, 1, 1);
	lightSource2.color = glm::vec3(1, 1, 1);
}

void Model::initialize()
{
	particleSystem = new ParticleSystem();
	particleSystem->initialize();
	//Default values
	glm::vec3 direction = glm::normalize(glm::vec3(0.35f, -0.40f, 0.85f));
	camera.setOrientation(direction.x, direction.y, direction.z);
	camera.setDistance(29.4f);
}

int Model::getOriginalParticleCount()
{
	return particleSystem->getOriginalParticleCount();
}

int Model::getGeneratedParticleCount()
{
	return particleSystem->getGeneratedParitlceCount();
}

void Model::cleanUp() 
{
	delete particleSystem;
}

void Model::animate(float dt)
{
	camera.Animate(dt);	
	particleSystem->Animate(dt);
}

glm::vec3 Model::getCameraPosition()
{
	return camera.getPosition();
}

glm::vec3 Model::getCameraOrientation()
{
	return camera.getOrientation();
}

float Model::getCameraViewAngle()
{
	return camera.getViewAngle();
}

LightSource Model::getLightSource1() 
{
	return lightSource1;
}

LightSource Model::getLightSource2() 
{
	return lightSource2;
}

const std::vector<Particle>& Model::getOriginalParticles()
{
	return particleSystem->getOriginalParticles();
}
const std::vector<Particle>& Model::getGeneratedParticles()
{
	return particleSystem->getGeneratedParticles();
}

int Model::getFileNumber()
{
	return particleSystem->getFileNumber();
}
