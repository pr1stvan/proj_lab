#pragma once

#include <vector>
#include <stdexcept>
#include "glm.hpp"

struct ArrayElement {
    glm::vec3 vec;
    float count;

    ArrayElement() :vec(0, 0, 0), count(0) {

    }
};


class Array3d {
private:
    glm::vec3 p;
    std::vector<ArrayElement> t;
    float cubeLength;
    float dl;
    int parts;
    float maxDistMinus;
public:
    Array3d(glm::vec3 position, float cubeLength, int splitting);

    ~Array3d() {

    }
    glm::vec3 getPosition(int i, int j, int k);
    glm::vec3 getVelocity(glm::vec3 pos, int ignoreNullVelocity);
    float distanceFunction(float distance);
    void setVelocity(glm::vec3 pos, glm::vec3 vel);
    void clear();
    void calculateAvg();
};
