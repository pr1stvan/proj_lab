#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "glm.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/vector_angle.hpp"

// 2D camera
struct Camera {
	glm::vec3 position;
	glm::vec3 right;
	glm::vec3 orientation;
	bool w = false, a = false, s = false, d = false, space = false, q = false, e = false;
	float movingSpeed = 10.0f;

	//szabad vagy kotott kamera
	bool freemode = false;

	//=======================================
	//eger iranyitas
	//=======================================
	float angleY = 90;//eger forgashoz

	//=======================================
	//kotott mod
	//=======================================
public:
	float rotationSpeed;
	glm::vec3 targetPoint;
	//=======================================
	//rafordulas
	//=======================================

	float lookAtpointSpeed;//Allapotvaltaskor a rafordulas sebessege
	bool arrived = false;

	//=======================================
	//zoom funkcio
	//=======================================
	float viewAngle = 45.0f;
	void rotate(float dx, float dy);

	Camera(float x, float y, float z) : targetPoint(0, 0, 0)
	{
		lookAtpointSpeed = 230.0f*M_PI / 180.0f;
		position = glm::vec3(x, y, z);
		orientation = glm::vec3(0, 0, -1);
		right = glm::vec3(1, 0, 0);
		rotationSpeed = 150*M_PI / 180.0f;
	}
	//=====================================
	//AntTweakBar fuggvenyek
	//=====================================
	
	bool getFreeMode(bool value);
	void setDistance(float dist);
	float getDistance();
	glm::vec3 getOrientation();
	glm::vec3 getPosition();
	float getViewAngle();
	void setFreeMode(bool value);

	void setOrientation(float x, float y, float z);
	//=======================================
	//OpenGL & GLUT
	//=======================================

	void Animate(float t);
	void keyDown(unsigned char key);
	void keyUp(unsigned char key);
	void mouseMotion(int pX, int pY);
};

#endif // CAMERA_H_INCLUDED
