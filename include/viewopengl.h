#pragma once

#include "glad/glad.h"
#include "glm.hpp"

#include "Sphere.h"
#include "Model.h"
#include "IShaderProgramProvider.h"

class ViewOpenGL {
private:
    unsigned int windowWidth, windowHeight;

    Model *model;
    //shader program id -s
    unsigned int simpleShaderProgram;
    unsigned int particleDrawShaderProgram;

    // vertex buffer objects
    unsigned int particlePointBuffer;
    unsigned int particleNormalBuffer;
    unsigned int particleIndexBuffer;
    unsigned int loadedParticleBuffer;
    unsigned int generatedParticleBuffer;

    //uniforms
    int particleShader_r_ID;
    int particleShader_VP_ID;
    int particleShader_V_ID;
    int particleShader_Smoothness_ID;
    int particleShader_AmbientPower_ID;
    int particleShader_Light1Position_worldspace_ID;
    int particleShader_Light1Color_ID;
    int particleShader_Light1Power_ID;
    int particleShader_Light1SpecularPower_ID;
    int particleShader_Light2Position_worldspace_ID;
    int particleShader_Light2Color_ID;
    int particleShader_Light2Power_ID;
    int particleShader_Light2SpecularPower_ID;

    int simpleShader_MVP_ID;
    int simpleShader_transparency_ID;
    int simpleShader_vertexColor_ID;

    bool useLights = true;

    unsigned int cubePointPositionBuffer;

    unsigned int lightSourceVertexBuffer;
    unsigned int lightSourceIndexBuffer;

    unsigned int vao; // vertex array object id

    int sphereIndexCount;
    int lightSourceIndexCount;

    float lightSourceSphereRadius = 0.4f;
    float particleRadius = 0.02f;
public:
    float particleSmoothness;
    bool drawCube = 1;
    bool drawSphereForLightSource1 = true;
    bool drawSphereForLightSource2 = true;

    ViewOpenGL();
    void initialize(Model* model,
                    IShaderProgramProvider& shaderProgramProvider);
    void initializeParticleCenterBuffers(Model* model);
    void initializeUniformVariables();
    void loadCube();
    void render();
    void renderCube();
    void renderLightSource(const LightSource& lightSource);
    void renderParticlesOpenGL();
    void cleanUp();
    void bindParticleBuffer(unsigned int bufferID);

    int getUniformLocation(unsigned int programID,const char* name);

    void setUseLights(bool useLights);
    bool getUseLights();

    glm::mat4 getCameraV(glm::vec3 pos, glm::vec3 orientation);
    glm::mat4 getCameraP(float viewAngle);

    void setWindowSize(unsigned int windowWidth, unsigned int windowHeight);
};
