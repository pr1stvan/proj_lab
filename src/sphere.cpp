#include "Sphere.h"

Sphere createSphereOpenGl(int verticalSubdivision, 
    int horizontalSubdivision, 
    float r)
{
    Sphere sphere;

    float x;
    float y;
    float z;

    //Point at the top
    sphere.normals.push_back(0);
    sphere.normals.push_back(0);
    sphere.normals.push_back(1.0f);

    sphere.vertices.push_back(0.0f);
    sphere.vertices.push_back(0.0f);
    sphere.vertices.push_back(r);

    //Point at the bottom
    sphere.normals.push_back(0);
    sphere.normals.push_back(0);
    sphere.normals.push_back(-1.0f);

    sphere.vertices.push_back(0.0f);
    sphere.vertices.push_back(0.0f);
    sphere.vertices.push_back(-r);

    int actualIndex = 1;
    const int topPointIndex = 0;
    const int bottomPointIndex = 1;

    //Top triangles
    for (int i = 0; i <= horizontalSubdivision; i++)
    {
        float u = i / (float)horizontalSubdivision;
        float v = 1 / (float)verticalSubdivision;
        x = r * cos(2.0f*M_PI*u)*sin(M_PI*v);
        y = r * sin(2.0f*M_PI*u)*sin(M_PI*v);
        z = r * cos(M_PI*v);

        glm::vec3 normal = normalize(glm::vec3(x, y, z));
        sphere.normals.push_back(normal.x);
        sphere.normals.push_back(normal.y);
        sphere.normals.push_back(normal.z);

        sphere.vertices.push_back(x);
        sphere.vertices.push_back(y);
        sphere.vertices.push_back(z);

        actualIndex++;

        if (i > 0) 
        {
            sphere.indexes.push_back(topPointIndex);
            sphere.indexes.push_back(actualIndex - 1);
            sphere.indexes.push_back(actualIndex);
        }
    }

    //Bottom triangles
    for (int i = 0; i <= horizontalSubdivision; i++)
    {
        float u = i / (float)horizontalSubdivision;
        float v = (verticalSubdivision - 1) / (float)verticalSubdivision;

        x = r * cos(2.0f*M_PI*u)*sin(M_PI*v);
        y = r * sin(2.0f*M_PI*u)*sin(M_PI*v);
        z = r * cos(M_PI*v);

        glm::vec3 normal = normalize(glm::vec3(x, y, z));

        sphere.normals.push_back(normal.x);
        sphere.normals.push_back(normal.y);
        sphere.normals.push_back(normal.z);

        sphere.vertices.push_back(x);
        sphere.vertices.push_back(y);
        sphere.vertices.push_back(z);

        actualIndex++;

        if (i > 0) 
        {
            sphere.indexes.push_back(bottomPointIndex);
            sphere.indexes.push_back(actualIndex);
            sphere.indexes.push_back(actualIndex - 1);
        }
    }

    //Triangles on the other part of the sphere
    for (int i = 1; i <= verticalSubdivision - 2; i++)//sorok
    {
        for (int j = 0; j <= horizontalSubdivision; j++)
        {
            float u = j / (float)horizontalSubdivision;
            float v1 = i / (float)verticalSubdivision;

            float v2 = (i + 1) / (float)verticalSubdivision;

            float x1 = r * cos(2.0f*M_PI*u)*sin(M_PI*v1);
            float y1 = r * sin(2.0f*M_PI*u)*sin(M_PI*v1);
            float z1 = r * cos(M_PI*v1);

            float x2 = r * cos(2.0f*M_PI*u)*sin(M_PI*v2);
            float y2 = r * sin(2.0f*M_PI*u)*sin(M_PI*v2);
            float z2 = r * cos(M_PI*v2);

            glm::vec3 normal = normalize(glm::vec3(x1, y1, z1));

            sphere.normals.push_back(normal.x);
            sphere.normals.push_back(normal.y);
            sphere.normals.push_back(normal.z);

            sphere.vertices.push_back(x1);
            sphere.vertices.push_back(y1);
            sphere.vertices.push_back(z1);

            actualIndex++;

            normal = normalize(glm::vec3(x2, y2, z2));

            sphere.normals.push_back(normal.x);
            sphere.normals.push_back(normal.y);
            sphere.normals.push_back(normal.z);

            sphere.vertices.push_back(x2);
            sphere.vertices.push_back(y2);
            sphere.vertices.push_back(z2);

            actualIndex++;

            if (j > 0) 
            {
                //Adding two triangles per quad
                sphere.indexes.push_back(actualIndex - 3);
                sphere.indexes.push_back(actualIndex - 2);
                sphere.indexes.push_back(actualIndex - 1);

                sphere.indexes.push_back(actualIndex);
                sphere.indexes.push_back(actualIndex - 1);
                sphere.indexes.push_back(actualIndex - 2);
            }
        }
    }
    
    return sphere;
}
