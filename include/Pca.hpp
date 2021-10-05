#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
// #include <Eigen/Dense>
#include <Eigen/Core>
#include <Eigen/Eigenvalues>

#include "VAO.hpp"

using namespace Eigen;
using namespace std;

class Pca
{
    public:

        Pca(float threshold);
        ~Pca();

        void normalize_data();

        void calc_vertex(float threshold);

        void setup_vao();

        int vao_count();

        void calc_1dtexture();
        void load_texture1d();

        void draw();

        void reload(float threshold);

    private:

        MatrixXd data;

        vector<double> vertex;

        VAO vao;

        unsigned char (*texture_data_1d)[4];
        unsigned int texture_1d;

};