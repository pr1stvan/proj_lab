#ifndef SPHERE_H_INCLUDED
#define SPHERE_H_INCLUDED

#include <vector>

#include "glm.hpp"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

struct Sphere
{
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<unsigned int> indexes;
};

Sphere createSphereOpenGl(int verticalSubdivision, int horizontalSubdivision, float r);

#endif // SPHERE_H_INCLUDED
