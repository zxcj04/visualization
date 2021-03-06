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

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "implot/implot.h"

#include "constants.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Volume.hpp"
#include "Streamline.hpp"
#include "PCA.hpp"

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
        bool set_callback_function();

        bool texture_init();
        template <typename T>
        void load_image(T &image, string filename, int width = 256, int height = 0);

        void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void keyboard_down(int key);

        void check_keyboard_pressing();

        void mouse_callback(GLFWwindow* window, int button, int action, int mods);

        void cursor_callback(GLFWwindow * window, double x, double y);

        void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

        void framebuffer_callback(GLFWwindow * w, int width, int height);

        void generate_template_combo();
        void load_color_template(string name);

        void imgui();

        void render_scene();
        void display();

        void mainloop();

    private:
        GLFWwindow *window;

        int width;
        int height;

        glm::vec4 clear_color;

        float last_x, last_y;

        METHODS method;
        vector<string> scalar_infs, scalar_raws;
        vector<string> vector_files;
        vector<string> color_template_files;

        /*-----Create image space for textures -----*/
        unsigned int   textName[5];

        Shader shader_iso_surface;
        Shader shader_volume_rendering;
        Shader shader_streamline;
        Shader shader_pca;

        Camera camera;

        bool showing_last;

        unsigned int VBO, VAO;
        unsigned int VBO2, VAO2;

        glm::vec3 light_color;
        float clip_x, clip_y, clip_z, clip;

        // Volume *test_volume;
        vector<Volume> volumes;
        vector<Streamline> streamlines;
        vector<Pca> pcas;

        glm::vec3 base_color;

        bool enable_section;

        vector<ImVector<ImVec2>> rgba_polylines;

        bool volume_rendering_shading;
        float volume_rendering_gap;
        float volume_rendering_last_decay;
        float volume_rendering_modifier;

        float tapering_modifier;

        float pca_threshold;
};