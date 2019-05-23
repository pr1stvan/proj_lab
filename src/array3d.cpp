#include "Array3d.h"

void Array3d::calculateAvg() 
{
    for (unsigned int i = 0; i < t.size(); i++)
	{
		if (t[i].count > 0)
		{
			t[i].vec.x /= t[i].count;
			t[i].vec.y /= t[i].count;
			t[i].vec.z /= t[i].count;
		}
	}
}
Array3d::Array3d(glm::vec3 position, float cubeLength, int splitting) 
{
	this->cubeLength = cubeLength;

	this->parts = splitting;

	this->p = glm::vec3(position.x - cubeLength / 2.0f, position.y - cubeLength / 2.0f, position.z - cubeLength / 2.0f);
	t.resize(parts*parts*parts);

	dl = (float)cubeLength / (float)splitting;
	maxDistMinus = -1.0f *glm::length(glm::vec3(1.5f, 1.5f, 1.5f));
}

inline float Array3d::distanceFunction(float distance)
{
	return (distance / maxDistMinus) + 1.0f;
}

inline glm::vec3 Array3d::getPosition(int i, int j, int k) 
{
	//Returns the center point of the voxel
	return glm::vec3(float(i), float(j), float(k));
}

void Array3d::setVelocity(glm::vec3 pos, glm::vec3 vel)
{
	pos = (pos - p)/dl; //dl = 1

	//The x,y and z indexes of the voxel that contains the particle
	int tx = int(pos.x); //Rounding the value down
	int ty = int(pos.y); //Rounding the value down
	int tz = int(pos.z); //Rounding the value down

	pos = glm::vec3(pos.x - 0.5f, pos.y - 0.5f, pos.z - 0.5f);

	//[ ][ ][ ][ ][ ][ ]
	//[ ][x][x][x][ ][ ]
	//[ ][x][p][x][ ][ ]
	//[ ][x][x][x][ ][ ]
	//[ ][ ][ ][ ][ ][ ]
	for (int k = tz - 1; k <= tz + 1; k++)
	{
		if (k<parts && k >= 0)for (int j = ty - 1; j <= ty + 1; j++)
		{
			if (j<parts && j >= 0)for (int i = tx - 1; i <= tx + 1; i++)
			{
				if (i < parts && i >= 0)
				{
					//getPosition:the center point of the actual voxel
					float distance = glm::length(pos - getPosition(i, j, k));
					float df = distanceFunction(distance);

					t[parts*parts*k + parts * j + i].vec += df * vel;
					t[parts*parts*k + parts * j + i].count += df;
				}
			}
		}
	}
}

glm::vec3 Array3d::getVelocity(glm::vec3 pos,int ignoreNullVelocity)
{
	pos = (pos - p) / dl; //dl = 1

	//The x,y and z indexes of the voxel that contains the particle
	int tx = int(pos.x); //Rounding the value down
	int ty = int(pos.y); //Rounding the value down
	int tz = int(pos.z); //Rounding the value down

	pos = glm::vec3(pos.x - 0.5f, pos.y - 0.5f, pos.z - 0.5f);
	glm::vec3 velocity(0, 0, 0);
	float countVelocities = 0.0f; //A vektorok szama atlagszamitashoz

	//[ ][ ][ ][ ][ ][ ]
	//[ ][x][x][x][ ][ ]
	//[ ][x][p][x][ ][ ]
	//[ ][x][x][x][ ][ ]
	//[ ][ ][ ][ ][ ][ ]
	for (int k = tz - 1; k <= tz + 1; k++)
	{
		if (k<parts && k >= 0)for (int j = ty - 1; j <= ty + 1; j++)
		{
			if (j<parts && j >= 0)for (int i = tx - 1; i <= tx + 1; i++)
			{
				if (i < parts && i >= 0)
				{
					if (ignoreNullVelocity)
					{
						if (t[parts*parts*k + parts * j + i].vec.x != 0 ||
							t[parts*parts*k + parts * j + i].vec.y != 0 ||
							t[parts*parts*k + parts * j + i].vec.z != 0)
						{
							//getPosition:the center point of the actual voxel
							float distance = glm::length(pos - getPosition(i, j, k));
							float df = distanceFunction(distance);

							velocity += df * t[parts*parts*k + parts * j + i].vec;
							countVelocities += df;
						}
					}
					else
					{
						//getPosition:the center point of the actual voxel
						float distance = glm::length(pos - getPosition(i, j, k));
						float df = distanceFunction(distance);

						velocity += df * t[parts*parts*k + parts * j + i].vec;
						countVelocities += df;
					}
				}
			}
		}

	}
	//Atlagszamitas
	if (countVelocities != 0.0f) {
		velocity.x /= countVelocities;
		velocity.y /= countVelocities;
		velocity.z /= countVelocities;
	}

	return velocity;
}

void Array3d::clear() 
{
    for (unsigned int i = 0; i < t.size(); i++) {
		try {
			t.at(i).vec = glm::vec3(0, 0, 0);
			t.at(i).count = 0;
		}
		catch (std::out_of_range o) {
			printf("tulindexelted");
		}
	}
}
