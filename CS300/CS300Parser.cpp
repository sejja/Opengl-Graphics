#include "CS300Parser.h"

#include <iostream>
#include <fstream>

float CS300Parser::ReadFloat(std::ifstream& f)
{
    std::string str;
    f >> str;
    return static_cast<float>(std::atof(str.c_str()));
}

glm::vec3 CS300Parser::ReadVec3(std::ifstream& f)
{
    float x = ReadFloat(f);
    float y = ReadFloat(f);
    float z = ReadFloat(f);

    return glm::vec3(x, y, z);
}

void CS300Parser::LoadDataFromFile(const char* filename)
{
    std::ifstream inFile(filename);

    if (!inFile.is_open())
    {
        std::cout << "Could not open input file" << std::endl;
        exit(0);
    }

    objects.clear();

    std::string str;

    while (!inFile.eof())
    {
        str = "";
        inFile >> str;
        auto comment = str.find_first_of("#");

        if (comment == 0)
        {
            std::getline(inFile, str);
            continue;
        }

        std::string id = str;

        if (id == "fovy")
        {
            fovy = ReadFloat(inFile);
        }
        else if (id == "width")
        {
            width = ReadFloat(inFile);
        }
        else if (id == "height")
        {
            height = ReadFloat(inFile);
        }
        else if (id == "near")
        {
            nearPlane = ReadFloat(inFile);
        }
        else if (id == "far")
        {
            farPlane = ReadFloat(inFile);
        }
        else if (id == "camPosition")
        {
            camPos = ReadVec3(inFile);
        }
        else if (id == "camTarget")
        {
            camTarget = ReadVec3(inFile);
        }
        else if (id == "camUp")
        {
            camUp = ReadVec3(inFile);
        }
        else if (id == "object")
        {
            Transform newObj;
            inFile >> newObj.name;
            objects.push_back(newObj);
        }
        else if (id == "translate")
        {
            glm::vec3 pos = ReadVec3(inFile);

            if (objects.size() > 0)
            {
                objects.back().pos = pos;
            }
        }
        else if (id == "rotation")
        {
            glm::vec3 rot = ReadVec3(inFile);

            if (objects.size() > 0)
            {
                objects.back().rot = rot;
            }
        }
        else if (id == "scale")
        {
            glm::vec3 sca = ReadVec3(inFile);

            if (objects.size() > 0)
            {
                objects.back().sca = sca;
            }
        }
        else if (id == "mesh")
        {
            std::string mesh;
            inFile >> mesh;
            if (objects.size() > 0)
            {
                objects.back().mesh = mesh;
            }
        }
    }
}