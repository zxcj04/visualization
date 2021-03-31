#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>

#include "glad/glad.h"
#include "glm/glm.hpp"

using namespace std;

enum TYPE
{
    UNSIGNED_CHAR, CHAR, UNSIGNED_SHORT, SHORT, UNSIGNED_INT, INT, FLOAT
};

enum ENDIAN
{
    LITTLE, BIG
};
class VAO
{
    public:
        GLuint id;
        int count;
};

class Volume
{
    public:

        Volume(string, string, int iso_value = 80);
        ~Volume();

        void read_inf(string);

        template<typename T>
        void read_data(string);

        void calc_gradient();

        void calc_vertex(int);

        void setup_vao();
        void draw();


    // private:
        string inf_filename, raw_filename;

        glm::ivec3 resolution;
        glm::vec3 voxelsize;
        TYPE valuetype;
        ENDIAN endian;

        int bytesize;

        vector<vector<vector<float>>> data;
        vector<vector<vector<glm::vec3>>> gradient;

        vector<float> vertex;

        VAO vao;
};