#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <limits>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"

#include "imgui/imgui.h"

#include "constants.hpp"

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

        Volume(string, string, METHODS, int iso_value = 80, float g_max = 256.0f);
        ~Volume();

        void read_inf(string);

        template<typename T>
        void read_data(string);

        void calc_gradient();

        void calc_vertex(int);

        void calc_3dtexture();
        void calc_1dtexture();

        void calc_histogram();
        void calc_mk_table(float g_max);

        void load_texture3d();
        void load_texture1d();

        void reload_1d_texture(vector<ImVector<ImVec2>>);

        void setup_volume_vao();

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
        vector<float> histogram;
        vector<vector<float> > mk_table;

        float histogram_max_value;

        int voxel_count;
        float min_value, max_value;

        float max_gradient_len;

        vector<float> vertex;

        VAO vao;

        METHODS method;

        // vector<unsigned char[4]> texture_data_3d;
        // vector<unsigned char[4]> texture_data_1d;

        unsigned char (*texture_data_3d)[4];
        unsigned char (*texture_data_1d)[4];

        unsigned int texture_3d;
        unsigned int texture_1d;
};