#include "Camera.h"

void Camera::Animate(float t) 
{
	if (freemode)
	{
		glm::vec3 velocityOrientation = glm::vec3(0,0,0);
		if (w == true)
		{
			velocityOrientation += orientation;
		}
		if (a == true)
		{
			velocityOrientation -= right;
		}
		if (s == true)
		{
			velocityOrientation -= orientation;
		}
		if (d == true)
		{
			velocityOrientation += right;
		}
		glm::vec3 v = velocityOrientation == glm::vec3(0, 0, 0) ? glm::vec3(0, 0, 0) : movingSpeed*glm::normalize(velocityOrientation);
		position +=v*t;
	}
	else if (!freemode)
	{
		glm::vec3 v(0, 0, 0);
		if (w == true)
		{
			v += movingSpeed *orientation;
		}
		if (a == true)
		{
			position = position - targetPoint;

			glm::vec3 lookAtPoint = position + orientation;
			if (!arrived) //The direction need to be rotated too, because the rotation around the center could slow the turning
			{
				lookAtPoint = glm::rotate(lookAtPoint, -rotationSpeed*t, glm::vec3(0, 1, 0));
			}
			position = glm::rotate(position, -rotationSpeed*t, glm::vec3(0, 1, 0));
			if (!arrived) //The direction need to be rotated too, because the rotation around the center could slow the turning
			{
				orientation = glm::normalize(lookAtPoint - position);
			}
			position = position + targetPoint;
		}
		if (s == true)
		{
			v -= movingSpeed *orientation;
		}
		if (d == true)
		{
			position = position - targetPoint;
			glm::vec3 lookPoint = position + orientation;
			if (!arrived) //The direction need to be rotated too, because the rotation around the center could slow the turning
			{
				lookPoint = glm::rotate(lookPoint, rotationSpeed*t, glm::vec3(0, 1, 0));
			}
			position = glm::rotate(position, rotationSpeed*t, glm::vec3(0, 1, 0));

			if (!arrived) //The direction need to be rotated too, because the rotation around the center could slow the turning
			{
				orientation = glm::normalize(lookPoint - position);
			}
			position += targetPoint;
		}
		position += v*t;
		
		if (arrived) //if the turning has ended
		{
			orientation = glm::normalize(targetPoint - position); //The camera is looking toward the center point
		}
		else
		{
			glm::vec3 rotationAxis = glm::cross(orientation, targetPoint - position);

			if (rotationAxis.x == 0.0f && rotationAxis.y == 0.0f && rotationAxis.z == 0.0f)// The axis vector could be (0,0,0), even if it's rare
			{
				arrived = true; //The (0,0,0) axis indicates, that the angle is 0
				orientation = glm::normalize(targetPoint - position);
			}
			else
			{
				glm::vec3 nextorientation = glm::rotate(orientation, lookAtpointSpeed*t, rotationAxis);
				glm::vec3 lookAtVector = targetPoint - position;

				//If it "outswings" the target point, the new rotationAxis will be pointing in the opposite direction
				if (glm::dot(rotationAxis, glm::cross(nextorientation, lookAtVector)) <= 0.0f)
				{
					arrived = true;
					orientation = glm::normalize(lookAtVector);
				}
				else
				{
					orientation = glm::normalize(nextorientation);
				}
			}
		}
		right = glm::normalize(glm::cross(orientation, glm::vec3(0,1,0)));
	}
}

void Camera::setOrientation(float x,float y,float z)
{
	if (!freemode) {
		glm::vec3 orientationNotNormalized(x, y, z);
		position = targetPoint + getDistance()* glm::normalize(-orientationNotNormalized);
		orientation = glm::normalize(orientationNotNormalized);
	}
}

void Camera::keyDown(unsigned char key)
{
	switch (key)
	{
	case 'w':
		w = true;
		break;
	case 'a':
		a = true;
		break;
	case 's':
		s = true;
		break;
	case 'd':
		d = true;
		break;
	case 'e':
		e = true;
		break;
	case 'q':
		q = true;
		break;
	case ' ':
		space = true;
		break;
	}
}
void Camera::keyUp(unsigned char key)
{
	switch (key)
	{
	case 'w':
		w = false;
		break;
	case 'a':
		a = false;
		break;
	case 's':
		s = false;
		break;
	case 'd':
		d = false;
		break;
	case 'e':
		e = false;
		break;
	case 'q':
		q = false;
		break;
	case ' ':
		space = false;
		break;
	}
}

void Camera::mouseMotion(int pX, int pY)
{
	float x = (float)pX;
	float y = (float)pY;
	float mouseMotionSpeed = viewAngle*(0.25f / 45.0f);

	float dx = x* mouseMotionSpeed;
	float dy = y* mouseMotionSpeed;

	if (freemode)
	{
		rotate(dx, dy);
	}
}

void Camera::setDistance(float dist)
{
	if(!freemode)
	{
		position=targetPoint+dist*glm::normalize(position - targetPoint);
	}
}

void Camera::rotate(float dx, float dy)
{
	orientation = glm::normalize(glm::rotate(orientation, glm::radians(-dx), glm::vec3(0, 1, 0)));
	right = glm::normalize(glm::rotate(right, glm::radians(-dx), glm::vec3(0, 1, 0)));

	if (angleY + dy >= 180.0f)
	{
		angleY = 180.0f;
	}
	else if (angleY + dy <= 0.0f)
	{
		angleY = 0.0f;
	}
	else
	{
		angleY = angleY + dy;
		orientation = glm::normalize(glm::rotate(orientation, glm::radians(-dy), right));
	}
}

void Camera::setFreeMode(bool value) 
{
	if (value == false && freemode == true) 
	{
		arrived = false;
	}
	else if (value == true && freemode == false) 
	{
		//The program doesn't allow vertical angle greater than 90 or lesser than 0
		//At the change from not free to free mode, it have to calculate the actual vertical angle
		angleY = glm::degrees(glm::angle(glm::normalize(orientation), glm::vec3(0,1,0)));
	}
	freemode = value;
}
glm::vec3 Camera::getOrientation() 
{
	return orientation;
}

float Camera::getDistance() 
{
	return glm::length(position - targetPoint);
}

glm::vec3 Camera::getPosition() 
{
	return position;
}

float Camera::getViewAngle() 
{
	return viewAngle;
}
