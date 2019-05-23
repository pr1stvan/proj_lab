#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <stdio.h>

#include <glad/glad.h>

#define GLM_ENABLE_EXPERIMENTAL

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/vector_angle.hpp"

#include "glfw3.h"

//#define TW_STATIC
#define _CRT_SECURE_NO_WARNINGS

#include <time.h>

#include <vector>


#include <stdlib.h>

#include "Partio.h"
extern unsigned int windowWidth, windowHeight;

class ParticleSystem;
struct Model;
struct ViewOpenGL;

#include "functions.h"
#include "Array3d.h"
#include "Camera.h"
#include "Particle.h"
#include "ParticleArrayLoader.h"
#include "FrameLoader.h"
#include "LoadBeforeFrameLoader.h"
#include "LoadRuntimeFrameLoader.h"
#include "Sphere.h"
#include "LightSource.h"
#include "ShaderLoader.h"
#include "IShaderProgramProvider.h"
#include "ProjLabShaderProgramProvider.h"

#include "ParticleSystem.h"
#include "ViewOpenGL.h"
#include "Model.h"

#include "MyAntTweakBar.h"

#endif // HEADER_H_INCLUDED

