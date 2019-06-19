#include "ViewOpenGL.h"

ViewOpenGL::ViewOpenGL() :particleSmoothness(3.0f)
{

}

void ViewOpenGL::loadCube() 
{
    glm::vec3 cubeVecs[16];

    glm::vec3 point1(-0.5f, -0.5f, 0.5f);
    glm::vec3 point2(0.5f, -0.5f, 0.5f);
    glm::vec3 point3(0.5f, -0.5f, -0.5f);
    glm::vec3 point4(-0.5f, -0.5f, -0.5f);

    glm::vec3 point5(-0.5f, 0.5f, 0.5f);
    glm::vec3 point6(0.5f, 0.5f, 0.5f);
    glm::vec3 point7(0.5f, 0.5f, -0.5f);
    glm::vec3 point8(-0.5f, 0.5f, -0.5f);

    cubeVecs[0] = point1;
    cubeVecs[1] = point2;
    cubeVecs[2] = point3;
    cubeVecs[3] = point4;
    cubeVecs[4] = point1;
    cubeVecs[5] = point5;
    cubeVecs[6] = point6;
    cubeVecs[7] = point2;
    cubeVecs[8] = point6;
    cubeVecs[9] = point7;
    cubeVecs[10] = point3;
    cubeVecs[11] = point7;
    cubeVecs[12] = point8;
    cubeVecs[13] = point4;
    cubeVecs[14] = point8;
    cubeVecs[15] = point5;

    //=========================================================================
    //Cube data
    //=========================================================================

    // vertex coordinates
    glBindBuffer(GL_ARRAY_BUFFER, cubePointPositionBuffer); // make it active

    glBufferData(GL_ARRAY_BUFFER,      // copy to the GPU
        sizeof(glm::vec3) * 16, // number of the vbo in bytes
        cubeVecs,           // address of the data array on the CPU
        // copy to that part of the memory which is not modified:
        GL_DYNAMIC_DRAW);
}

void ViewOpenGL::initialize(Model *model,
                            IShaderProgramProvider &shaderProgramProvider)
{
    simpleShaderProgram =
            shaderProgramProvider.getSimpleShaderProgram();
    particleDrawShaderProgram =
            shaderProgramProvider.getParticleDrawShaderProgram();

    initializeUniformVariables();

    this->model = model;

    glGenVertexArrays(1, &vao);    // create 1 vertex array object
    glBindVertexArray(vao);        // make it active
    glGenBuffers(1, &particlePointBuffer); // Generate a vertex buffer object
    glGenBuffers(1, &particleNormalBuffer);
    glGenBuffers(1, &particleIndexBuffer);
    glGenBuffers(1, &loadedParticleBuffer);
    glGenBuffers(1, &generatedParticleBuffer);
    glGenBuffers(1, &cubePointPositionBuffer);
    glGenBuffers(1, &lightSourceVertexBuffer);
    glGenBuffers(1, &lightSourceIndexBuffer);

    //snowflakes
    Sphere sphere = createSphereOpenGl(10, 10, 1.0f);
    sphereIndexCount = sphere.indexes.size();
    // vertex coordinates
    glBindBuffer(GL_ARRAY_BUFFER, particlePointBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sphere.vertices.size() * sizeof(float),
                 &sphere.vertices[0], GL_STATIC_DRAW);

    // vertex normals
    glBindBuffer(GL_ARRAY_BUFFER, particleNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sphere.normals.size() * sizeof(float),
                 &sphere.normals[0], GL_STATIC_DRAW);

    //indexes for coordinates and normals
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particleIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sphere.indexes.size() * sizeof(unsigned int),
                 &sphere.indexes[0], GL_STATIC_DRAW);

    //Light source spheres
    sphere = createSphereOpenGl(20, 20, 1.0f);
    lightSourceIndexCount = sphere.indexes.size();
    // vertex coordinates
    glBindBuffer(GL_ARRAY_BUFFER, lightSourceVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sphere.vertices.size() * sizeof(float),
                 &sphere.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightSourceIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sphere.indexes.size() * sizeof(unsigned int),
                 &sphere.indexes[0], GL_STATIC_DRAW);

    loadCube();
}

int ViewOpenGL::getUniformLocation(unsigned int programID, const char* name)
{
    int location = glGetUniformLocation(programID, name);

    if (location < 0)
    {
        printf("uniform %s cannot found\n", name);
    }

    return location;
}
void ViewOpenGL::initializeParticleCenterBuffers(Model* model)
{
    glBindBuffer(GL_ARRAY_BUFFER, loadedParticleBuffer);
    // Initialize with empty (NULL) buffer :
    // it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, // copy to the GPU
        model->getOriginalParticleCount() * sizeof(Particle), // num of the vbo
        NULL,    // pointer to the data
        GL_STREAM_DRAW);

    // Initialize with empty (NULL) buffer :
    // it will be updated later, each frame.
    glBindBuffer(GL_ARRAY_BUFFER, generatedParticleBuffer);
    glBufferData(GL_ARRAY_BUFFER, // copy to the GPU
        model->getGeneratedParticleCount() * sizeof(Particle), // numb of vbo
        NULL,    // pointer to the data
        GL_STREAM_DRAW);
}

void ViewOpenGL::setWindowSize(unsigned int windowWidth,
                               unsigned int windowHeight)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    
    glViewport(0, 0, windowWidth, windowHeight);
}

void ViewOpenGL::initializeUniformVariables() 
{
    particleShader_r_ID = getUniformLocation(particleDrawShaderProgram, "r");
    particleShader_VP_ID = getUniformLocation(particleDrawShaderProgram, "VP");
    particleShader_V_ID = getUniformLocation(particleDrawShaderProgram, "V");
    particleShader_Smoothness_ID =
            getUniformLocation(particleDrawShaderProgram, "Smoothness");
    particleShader_AmbientPower_ID =
            getUniformLocation(particleDrawShaderProgram, "AmbientPower");
    particleShader_Light1Position_worldspace_ID =
            getUniformLocation(particleDrawShaderProgram,
                               "Light1Position_worldspace");
    particleShader_Light1Color_ID =
            getUniformLocation(particleDrawShaderProgram, "Light1Color");
    particleShader_Light1Power_ID =
            getUniformLocation(particleDrawShaderProgram, "Light1Power");
    particleShader_Light1SpecularPower_ID =
            getUniformLocation(particleDrawShaderProgram,
                               "Light1SpecularPower");
    particleShader_Light2Position_worldspace_ID =
            getUniformLocation(particleDrawShaderProgram,
                               "Light2Position_worldspace");
    particleShader_Light2Color_ID =
            getUniformLocation(particleDrawShaderProgram,
                               "Light2Color");
    particleShader_Light2Power_ID =
            getUniformLocation(particleDrawShaderProgram,
                               "Light2Power");
    particleShader_Light2SpecularPower_ID =
            getUniformLocation(particleDrawShaderProgram,
                               "Light2SpecularPower");

    simpleShader_MVP_ID = getUniformLocation(simpleShaderProgram, "MVP");
    simpleShader_transparency_ID =
            getUniformLocation(simpleShaderProgram, "transparency");
    simpleShader_vertexColor_ID =
            getUniformLocation(simpleShaderProgram, "vertexColor");
}

void ViewOpenGL::renderParticlesOpenGL() 
{
    glUseProgram(particleDrawShaderProgram);
    glEnable(GL_CULL_FACE);

    glm::mat4 V = getCameraV(model->getCameraPosition(),
                             model->getCameraOrientation());

    glm::mat4 P = getCameraP(model->getCameraViewAngle());
    glm::mat4 VPTransform = P*V;
    glm::mat4 VTransform = V;

    if (useLights) 
    {
        glUniform1f(particleShader_r_ID, particleRadius);
        glUniform1f(particleShader_AmbientPower_ID, 0.5f);
        glUniform1f(particleShader_Light1Power_ID,
                    model->lightSource1.lightPower);
        glUniform1f(particleShader_Light2Power_ID,
                    model->lightSource2.lightPower);
        glUniform1f(particleShader_Light1SpecularPower_ID,
                    model->lightSource1.specularPower);
        glUniform1f(particleShader_Light2SpecularPower_ID,
                    model->lightSource2.specularPower);
    }
    else 
    {
        glUniform1f(particleShader_r_ID, 0.01f);
        glUniform1f(particleShader_AmbientPower_ID, 1.0f);
        glUniform1f(particleShader_Light1Power_ID, 0.0f);
        glUniform1f(particleShader_Light2Power_ID, 0.0f);
        glUniform1f(particleShader_Light1SpecularPower_ID, 0.0f);
        glUniform1f(particleShader_Light2SpecularPower_ID, 0.0f);
    }

    glUniformMatrix4fv(particleShader_VP_ID, 1, GL_FALSE, &VPTransform[0][0]);
    glUniformMatrix4fv(particleShader_V_ID, 1, GL_FALSE, &VTransform[0][0]);
    glUniform1f(particleShader_Smoothness_ID, particleSmoothness);
    glUniform3fv(particleShader_Light1Position_worldspace_ID,
                 1, &model->lightSource1.position[0]);
    glUniform3fv(particleShader_Light1Color_ID,
                 1, &model->lightSource1.color[0]);
    glUniform3fv(particleShader_Light2Position_worldspace_ID,
                 1, &model->lightSource2.position[0]);
    glUniform3fv(particleShader_Light2Color_ID,
                 1, &model->lightSource2.color[0]);

    //Coordinates
    //=========================================================================

    // vertex coordinates
    glBindBuffer(GL_ARRAY_BUFFER, particlePointBuffer);
    glEnableVertexAttribArray(0);
    // Data organization of Attribute Array 0
    glVertexAttribPointer(
        0,                  // attribute location number in the shader
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    //vertex normals
    glBindBuffer(GL_ARRAY_BUFFER, particleNormalBuffer);
    glEnableVertexAttribArray(3);
    // Data organization of Attribute Array 3
    glVertexAttribPointer(
        3,                  // attribute location number in the shader
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    const std::vector<Particle>& originalParticles =
            model->getOriginalParticles();
    const std::vector<Particle> & generatedParticles =
            model->getGeneratedParticles();
    
    // center coordinates and colors
    glBindBuffer(GL_ARRAY_BUFFER, loadedParticleBuffer);
    //Buffer orphaning from opengl tutorial. Increases speed at about 0.7 fps 
    glBufferData(GL_ARRAY_BUFFER, // copy to the GPU
        originalParticles.size() * sizeof(Particle), // num of the vbo in bytes
        NULL,
        GL_STREAM_DRAW);// copy to that part of the memory which is not modified
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    originalParticles.size() * sizeof(Particle),
                    &originalParticles[0]);

    glBindBuffer(GL_ARRAY_BUFFER,
                 generatedParticleBuffer);
    //Buffer orphaning from opengl tutorial. Increases speed at about 0.7 fps
    glBufferData(GL_ARRAY_BUFFER, // copy to the GPU
        generatedParticles.size() * sizeof(Particle), //num of the vbo in bytes
        NULL,
        GL_STREAM_DRAW); // copy to that part of the memory which isn't modified
    glBufferSubData(GL_ARRAY_BUFFER,
        0,
        generatedParticles.size() * sizeof(Particle),
        &generatedParticles[0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particleIndexBuffer);

    glVertexAttribDivisor(0, 0); // vertices : always reuse the same vertices->0
    glVertexAttribDivisor(1, 1); // center positions : one per sphere -> 1
    glVertexAttribDivisor(2, 1); // color : one per sphere -> 1
    glVertexAttribDivisor(3, 0); // normals : always reuse the same normals -> 0
    
    bindParticleBuffer(loadedParticleBuffer);
    glDrawElementsInstanced(GL_TRIANGLES,
                            sphereIndexCount,
                            GL_UNSIGNED_INT,
                            0,
                            originalParticles.size());

    bindParticleBuffer(generatedParticleBuffer);
    glDrawElementsInstanced(GL_TRIANGLES,
                            sphereIndexCount,
                            GL_UNSIGNED_INT,
                            0,
                            generatedParticles.size());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisable(GL_CULL_FACE);
}

void ViewOpenGL::bindParticleBuffer(unsigned int bufferID)
{
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glEnableVertexAttribArray(1);
    // Data organization of Attribute Array 0
    glVertexAttribPointer(1,            // Attribute Array 1
        3, // components/attribute
        GL_FLOAT,  //component type
        GL_FALSE,    // normalize?
        sizeof(Particle), //stride: A particle difference between positions
        NULL);     // offset: the first element is a the position(pos,color,v)

    //colors
    //=========================================================================

    // Map Attribute Array 1 to
    // the current bound vertex buffer (particleColorBuffer)
    glEnableVertexAttribArray(2);  // Vertex color
    // Data organization of Attribute Array 1

    glVertexAttribPointer(2, // Attribute Array 2
        3, // components/attribute
        GL_FLOAT, //component type 
        GL_FALSE, //normalize? 
        sizeof(Particle), //stride: A particle difference between positions
        // offset: the second element is a the color(pos,color,v):
        (GLvoid*)(sizeof(glm::vec3)));
}

void ViewOpenGL::render() 
{
    glClearColor(0.0f, 0.0f, 0.0f, 255); // background color
    // clear the screen:
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(vao);

    if (model->getFileNumber()> 0) //Renders only, if there are frames loaded
    {
        renderParticlesOpenGL();
    }

    if (drawCube)
    {
        renderCube();
    }

    if (drawSphereForLightSource1) 
    {
        renderLightSource(model->getLightSource1());
    }

    if (drawSphereForLightSource2) 
    {
        renderLightSource(model->getLightSource2());
    }
}

void ViewOpenGL::renderLightSource(const LightSource& lightSource) 
{
    glUseProgram(simpleShaderProgram);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 M;

    glm::mat4 V = getCameraV(model->getCameraPosition(),
                             model->getCameraOrientation());
    glm::mat4 P = getCameraP(model->getCameraViewAngle());

    //Move the lightSource to its position:
    M = glm::translate(lightSource.position);
    M = glm::scale(M,
                   glm::vec3(lightSourceSphereRadius / 1.5f,
                             lightSourceSphereRadius / 1.5f,
                             lightSourceSphereRadius / 1.5f)); // model matrix
    glm::mat4 MVPTransform = P* V *M;

    glUniformMatrix4fv(simpleShader_MVP_ID, 1, GL_FALSE, &MVPTransform[0][0]);
    glUniform1f(simpleShader_transparency_ID, 1.0f);
    glUniform3fv(simpleShader_vertexColor_ID, 1, (float*)&lightSource.color);

    //Vertexes
    glBindBuffer(GL_ARRAY_BUFFER, lightSourceVertexBuffer);
    // Map Attribute Array 0 to the current bound vertex buffer (vbo[0]):
    glEnableVertexAttribArray(0);
    // Data organization of Attribute Array 0
    glVertexAttribPointer(0,            // Attribute Array 0
        3, GL_FLOAT,  // components/attribute, component type
        GL_FALSE,        // not in fixed point format, do not normalized
        0, NULL);     // stride and offset: it is tightly packed

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightSourceIndexBuffer);

    glDrawElements(GL_TRIANGLES,
                   lightSourceIndexCount,
                   GL_UNSIGNED_INT,
                   (void*)0);

    //Move the lightSource to its position:
    M = glm::translate(lightSource.position);
    M = glm::scale(M,
                   glm::vec3(lightSourceSphereRadius,
                             lightSourceSphereRadius,
                             lightSourceSphereRadius)); // model matrix
    MVPTransform = P* V *M;

    glUniformMatrix4fv(simpleShader_MVP_ID, 1, GL_FALSE, &MVPTransform[0][0]);
    glUniform1f(simpleShader_transparency_ID, 0.5f);

    glDrawElements(GL_TRIANGLES,
                   lightSourceIndexCount,
                   GL_UNSIGNED_INT,
                   (void*)0);

    glDisableVertexAttribArray(0);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
}

void ViewOpenGL::renderCube() 
{
    glUseProgram(simpleShaderProgram);

    glm::mat4 V = getCameraV(model->getCameraPosition(),
                             model->getCameraOrientation());

    glm::mat4 P = getCameraP(model->getCameraViewAngle());

    glm::mat4 M = glm::scale(glm::vec3(model->getCubeLength(),
                                       model->getCubeLength(),
                                       model->getCubeLength())); // model matrix
    glm::mat4 MVPTransform = P*V*M;

    glUniformMatrix4fv(simpleShader_MVP_ID, 1, GL_FALSE, &MVPTransform[0][0]);
    glUniform1f(simpleShader_transparency_ID, 1.0f);
    glUniform3f(simpleShader_vertexColor_ID, 1.0, 1.0, 1.0);

    //Vertexes
    glBindBuffer(GL_ARRAY_BUFFER, cubePointPositionBuffer);
    // Map Attribute Array 0 to the current bound vertex buffer (vbo[0]):
    glEnableVertexAttribArray(0);
    // Data organization of Attribute Array 0
    glVertexAttribPointer(0,            // Attribute Array 0
        3, GL_FLOAT,  // components/attribute, component type
        GL_FALSE,        // not in fixed point format, do not normalized
        0, NULL);     // stride and offset: it is tightly packed

    glDrawArrays(GL_LINE_STRIP, 0, 16);
    glDisableVertexAttribArray(0);
}
void ViewOpenGL::setUseLights(bool useLights) 
{
    this->useLights = useLights;
}
bool ViewOpenGL::getUseLights() 
{
    return useLights;
}

void ViewOpenGL::cleanUp() 
{
    // Cleanup VBO and shader
    glDeleteBuffers(1, &particlePointBuffer);
    glDeleteBuffers(1, &particleNormalBuffer);
    glDeleteBuffers(1, &particleIndexBuffer);
    glDeleteBuffers(1, &loadedParticleBuffer);
    glDeleteBuffers(1, &generatedParticleBuffer);
    glDeleteBuffers(1, &cubePointPositionBuffer);
    glDeleteBuffers(1, &lightSourceVertexBuffer);

    glDeleteVertexArrays(1, &vao);

    glDeleteProgram(simpleShaderProgram);
    glDeleteProgram(particleDrawShaderProgram);
}

// view matrix: translates the center to the origin:
glm::mat4 ViewOpenGL::getCameraV(glm::vec3 pos, glm::vec3 orientation)
{ 
    return glm::lookAt(pos, pos + orientation, glm::vec3(0, 1, 0));
}

// projection matrix: scales it to be a square of edge length 2:
glm::mat4 ViewOpenGL::getCameraP(float viewAngle)
{ 
    glm::mat4 projection = glm::perspective(
                glm::radians(viewAngle),
                (float)windowWidth / (float)windowHeight,
                0.1f,
                1000.0f);
    return projection;
}
