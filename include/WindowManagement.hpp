#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <set>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include <KHR/khrplatform.h>

#define GLT_IMPLEMENTATION
#define GLT_MANUAL_VIEWPORT
#include "glText/gltext.h" /* https://github.com/vallentin/glText */
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "./Shader.hpp"

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

        bool init(string);
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
        void make_view(int scene);
        void make_projection(int scene);
        void make_scene(int scene);
        void make_view_port(int scene);
        void make_light();
        void render_scene(int scene);
        void display();

        void mainloop();

    private:
        GLFWwindow *window;

        int width;
        int height;

        float last_x, last_y;

        int info_width;
        int info_height;

        int horizon_distance;

        set<int> pressing;

        float floor_color[401][401][3];

        bool show_shadow_points;

        bool enable_lit0;
        bool enable_lit1;
        bool enable_lit2;

        bool enable_cursor;

        float  global_ambient[4];

        float lit_position[4];
        float lit1_position[4];
        float lit2_position[4];
        float lit2_direction[4];

        float lit2_angle;
        float lit2_angle_y;
        float  lit2_cutoff;
        float  lit2_exponent;

        float  lit1_diffuse[4];

        /*-----Create image space for textures -----*/
        unsigned char  checkboard[TSIZE][TSIZE][4];   /* checkboard textures */
        unsigned char  brick[TSIZE][TSIZE][4];        /* brick wall textures */
        unsigned char  sand[256][256][4];
        unsigned char  seaweed[256][256][4];
        unsigned char  bubble[256][256][4];
        unsigned int   textName[5];

        float black[4];
        float white[4];

        Shader shader;

        // FT_Library ft;
        // FT_Face face;
};