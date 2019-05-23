#include "Particle.h"

void Particle::move(float t)
{
	pos +=  v;


	//Visszapattan
	/*if (glm::length(pos) >= r)
	{
		pos = r*glm::normalize(pos);
		v = glm::length(v)*glm::reflect(glm::normalize(v), glm::normalize(-1.0f*pos));
	}*/



}
