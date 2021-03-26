#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <set>
#include <map>
#include <dirent.h>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include <KHR/khrplatform.h>

#define GLT_IMPLEMENTATION
#define GLT_MANUAL_VIEWPORT
#include "glText/gltext.h" /* https://github.com/vallentin/glText */
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "./constants.hpp"
#include "./Shader.hpp"
#include "./Camera.hpp"

#ifndef M_PI
#define M_PI acos(-1)
#endif

using namespace std;

#define   TSIZE  64   /* define texture dimension */

enum SCENE
{
    FIRST, THIRD, GOD, INFO
};

class WindowManagement
{
    public:

        WindowManagement();
        ~WindowManagement();

        static void error_callback(int, const char *);

        bool init(string);
        void generate_combo();
        void set_callback_functions();
        bool system_init();
        bool light_init();
        bool font_init();
        bool set_callback_function();

        bool texture_init();
        template <typename T>
        void load_image(T &image, string filename, int width = 256, int height = 0);
        void make_check_pattern();
        void make_brick_pattern();

        void check_keyboard_pressing();

        void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void keyboard_up(int key);
        void keyboard_down(int key);
        void keyboard_repeat(int key);

        void mouse_callback(GLFWwindow* window, int button, int action, int mods);

        void cursor_callback(GLFWwindow * window, double x, double y);

        void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

        void framebuffer_callback(GLFWwindow * w, int width, int height);

        void show_info();
        void make_light();
        void render_scene();
        void display();

        void mainloop();

    private:
        GLFWwindow *window;

        int width;
        int height;

        float last_x, last_y;

        map<string, METHODS> methods;
        vector<string> scalar_filenames, vector_filenames, high_dim_filenames;

        int info_width;
        int info_height;

        float floor_color[401][401][3];

        /*-----Create image space for textures -----*/
        unsigned char  checkboard[TSIZE][TSIZE][4];   /* checkboard textures */
        unsigned char  brick[TSIZE][TSIZE][4];        /* brick wall textures */
        unsigned char  sand[256][256][4];
        unsigned char  seaweed[256][256][4];
        unsigned char  bubble[256][256][4];
        unsigned int   textName[5];

        Shader shader;
        Camera camera;

        unsigned int VBO, VAO;
        unsigned int VBO2, VAO2;
};