#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

class CS300Parser
{
public:
    void LoadDataFromFile(const char* filename);

    float     fovy;
    float     width;
    float     height;
    float     nearPlane;
    float     farPlane;
    glm::vec3 camPos;
    glm::vec3 camTarget;
    glm::vec3 camUp;

    struct Transform
    {
        std::string name;

        std::string mesh;

        glm::vec3 pos;
        glm::vec3 rot;
        glm::vec3 sca;
    };
    std::vector<Transform> objects;

private:
    static float     ReadFloat(std::ifstream& f);
    static glm::vec3 ReadVec3(std::ifstream& f);
};