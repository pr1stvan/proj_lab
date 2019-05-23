#ifndef ISHADERPROGRAMPROVIDER_H_INCLUDED
#define ISHADERPROGRAMPROVIDER_H_INCLUDED

class IShaderProgramProvider {
public:
    virtual unsigned int getSimpleShaderProgram() = 0;
    virtual unsigned int getParticleDrawShaderProgram() = 0;
    virtual unsigned int getTriangleDrawShaderProgram() = 0;
};

#endif // ISHADERPROGRAMPROVIDER_H_INCLUDED
