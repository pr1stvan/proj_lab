#ifndef PROJLABSHADERPROGRAMPROVIDER_H_INCLUDED
#define PROJLABSHADERPROGRAMPROVIDER_H_INCLUDED

#include <string>
#include <fstream>
#include <sstream>

#include "ShaderLoader.h"
#include "IShaderProgramProvider.h"

class ProjLabShaderProgramProvider: public IShaderProgramProvider
{
public:
    unsigned int getSimpleShaderProgram();
    unsigned int getParticleDrawShaderProgram();
    unsigned int getTriangleDrawShaderProgram();

private:
    std::string getTextFromFile(const char* filePath);
    ShaderLoader loader;
};

#endif // PROJLABSHADERPROGRAMPROVIDER_H_INCLUDED
