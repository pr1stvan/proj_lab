#include "ProjLabShaderProgramProvider.h"

std::string ProjLabShaderProgramProvider::getTextFromFile(const char* filePath)
{
    std::string text;
    std::ifstream stream(filePath, std::ios::in);
    
    if (stream.is_open())
    {
        std::stringstream sstr;
        sstr << stream.rdbuf();
        text = sstr.str();
        stream.close();
    }
    else
    {
        printf("cannot open file");
        return 0;
    }
    
    return text;
}

unsigned int ProjLabShaderProgramProvider::getSimpleShaderProgram()
{
    std::string simpleVertexShader =
            getTextFromFile("shaders/simplevertex.vert");
    std::string simpleFragmentShader =
            getTextFromFile("shaders/simplefragment.frag");

    // Create objects by setting up their vertex data on the GPU
    return loader.createShaderProgramFromSource(
                simpleVertexShader.c_str(),
                simpleFragmentShader.c_str());
}

unsigned int ProjLabShaderProgramProvider::getParticleDrawShaderProgram()
{
    std::string particleVertexShader =
            getTextFromFile("shaders/particlevertex.vert");
    std::string particleFragmentShader =
            getTextFromFile("shaders/particlefragment.frag");

    return loader.createShaderProgramFromSource(
                particleVertexShader.c_str(),
                particleFragmentShader.c_str());
}

unsigned int ProjLabShaderProgramProvider::getTriangleDrawShaderProgram()
{
    return 0;
}
