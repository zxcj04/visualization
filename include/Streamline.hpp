#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "imgui/imgui.h"

#include "VAO.hpp"

using namespace std;

class Streamline
{
    public:

        Streamline(string vec_filename, double gap, int iter_times, double h, double limit);
        ~Streamline();

        void read_vec();

        glm::dvec2 interpolate_data(double x, double y);

        void calc_vertex(double gap, int iter_times, double h, double limit);

        void setup_vao();

        int vao_count();
        glm::vec3 resolution();

        void draw();

        void reload(double gap, int iter_times, double h, double limit);

        double max_value();

        void calc_1dtexture();
        void load_texture1d();

        void reload_1d_texture(vector<ImVector<ImVec2>>);

    private:
        string vec_filename;

        int width, height;

        vector<vector<glm::dvec2>> data;
        vector<vector<bool>> occupy;

        vector<double> vertex;

        VAO vao;

        double value_max;

        unsigned char (*texture_data_1d)[4];
        unsigned int texture_1d;
};