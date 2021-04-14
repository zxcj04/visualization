#include "WindowManagement.hpp"

#define GLFW_MINOR_VERSION 6

WindowManagement::WindowManagement()
{
    cout << "WindowManagement init" << endl;
}

WindowManagement::~WindowManagement()
{
	glfwDestroyWindow(this->window);

    cout << "terminate" << endl;

    glfwTerminate();
}

void WindowManagement::error_callback(int error, const char * description)
{
	cout << "Error: " << description << endl;
}

bool WindowManagement::init(string window_name)
{
    glfwSetErrorCallback(WindowManagement::error_callback);

    GLuint err = !glfwInit();
    if (err)
    {
        cout << "glfwInit ERROR" << endl;

        return false;
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifndef __APPLE__
        // dedug context
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    #endif
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    width = 1200;
    height = 900;

    this->last_x = width/2;
    this->last_y = height/2;

    this->window = glfwCreateWindow(width, height, window_name.c_str(), NULL, NULL);

    GLFWmonitor* primary = glfwGetPrimaryMonitor();

    const GLFWvidmode *mode = glfwGetVideoMode(primary);
    if (!mode)
        return 0;

    int monitorX, monitorY;
    glfwGetMonitorPos(primary, &monitorX, &monitorY);

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    glfwSetWindowPos(window,
                     monitorX + (mode->width - windowWidth) / 2,
                     monitorY + (mode->height - windowHeight) / 2);

    if(!this->window)
    {
        cout << "glfwCreateWindow ERROR" << endl;

        glfwTerminate();

        return false;
    }
    else
    {
        cout << "Create window Success" << endl;
    }

    glfwMakeContextCurrent(this->window);

    glfwSwapInterval(1);

    // glfwSetWindowAspectRatio(window, 1, 1);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "Failed to initialize GLAD" << endl;

        return false;
    }

    cout << "OpenGL shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    set_callback_functions();

    // IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    system_init();

    this->shader = Shader("./src/shaders/tri.vert", "./src/shaders/tri.frag");

    cout << this->shader.ID << endl;

    this->camera = Camera();

    this->light_color = glm::vec3(1.0f, 1.0f, 1.0f);
    this->clear_color = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);

    this->clip_x = 50;
    this->clip_y = 50;
    this->clip_z = 50;
    this->clip = 50;

    this->showing_last = false;

    this->enable_section = false;

    this->base_color = glm::vec3(0.7, 0.5, 0.5);

    // -----------------------------------------

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // -----------------------------------------

    generate_combo();

    return true;
}

void WindowManagement::generate_combo()
{
    // generate methods combo
    this->methods["Iso Surface"] = METHODS::ISO_SURFACE;

    // generate filenames combo
    DIR *dp;
    dirent *dirp;

    if((dp = opendir("./Data/Scalar/")) != NULL)
    {
        while((dirp = readdir(dp)) != NULL)
        {
            string temp = dirp->d_name;
            int index_inf = temp.find(".inf");
            int index_raw = temp.find(".raw");

            if(index_inf != string::npos) this->scalar_infs.push_back(temp.substr(0, index_inf));
            if(index_raw != string::npos) this->scalar_raws.push_back(temp.substr(0, index_raw));
        }
    }
    closedir(dp);

    sort(this->scalar_infs.begin(), this->scalar_infs.end());
    sort(this->scalar_raws.begin(), this->scalar_raws.end());

    // for(auto it: this->scalar_filenames)
    // {
    //     Volume tmp("./Data/Scalar/" + it + ".inf", "");
    // }
    //     cout << it << endl;

}

void WindowManagement::set_callback_functions()
{
    glfwSetWindowUserPointer(this->window, this);

    auto keyboardCb = [](GLFWwindow * w, int key, int scan, int act, int mod){
        static_cast<WindowManagement*>(glfwGetWindowUserPointer(w))->keyboard_callback(w, key, scan, act, mod);
    };
    auto mouseCb = [](GLFWwindow * w, int button, int action, int mods){
        static_cast<WindowManagement*>(glfwGetWindowUserPointer(w))->mouse_callback(w, button, action, mods);
    };
    auto scrollCb = [](GLFWwindow * w, double x_offset, double y_offset){
        static_cast<WindowManagement*>(glfwGetWindowUserPointer(w))->scroll_callback(w, x_offset, y_offset);
    };
    auto cursorPosCb = [](GLFWwindow * w, double x_pos, double y_pos){
        static_cast<WindowManagement*>(glfwGetWindowUserPointer(w))->cursor_callback(w, x_pos, y_pos);
    };
    auto viewportCb = [](GLFWwindow * w, int width, int height){
        static_cast<WindowManagement*>(glfwGetWindowUserPointer(w))->framebuffer_callback(w, width, height);
    };

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetKeyCallback(this->window, keyboardCb);
    glfwSetMouseButtonCallback(window, mouseCb);
    glfwSetScrollCallback(this->window, scrollCb);
    glfwSetCursorPosCallback(this->window, cursorPosCb);
    glfwSetFramebufferSizeCallback(this->window, viewportCb);
}

bool WindowManagement::system_init()
{
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    // glShadeModel(GL_SMOOTH);

    // glEnable(GL_NORMALIZE);

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_PROGRAM_POINT_SIZE);

    texture_init();

    float colors[4][3] = {
                            {0.08, 0.21, 0.17},
                            {0.16, 0.39, 0.4},
                            {0.25, 0.40, 0.32},
                            {0.12, 0.31, 0.26}
                          };

    light_init();

    // glCullFace(GL_BACK);
    // glEnable(GL_CULL_FACE);

    static float fog_color[]={0.15, 0.20, 0.20, 0.50};

    // glEnable(GL_FOG);                /*enable fog fade */
    // glFogi(GL_FOG_MODE, GL_LINEAR);  /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
    // glFogf(GL_FOG_DENSITY, 0.15);    /*fog opacity(density)= 0.25*/
    // glFogf(GL_FOG_START, 250.0);       /*Setup two ends for GL_LINEAR*/
    // glFogf(GL_FOG_END, 400.0);
    // glFogfv(GL_FOG_COLOR, fog_color);/*set the fog color */

    return true;
}

bool WindowManagement::light_init()
{
    float  lit_diffuse[] = {0.75, 0.75, 0.75, 1.0};
    float  lit_specular[] = {0.7, 0.7, 0.7, 1.0};

    // float  lit1_diffuse[] = {0.075, 0.10, 0.075, 1.0};
    float  lit1_ambient[] = {0.0, 0.0, 0.0, 0.0};

    float  lit2_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    float  lit2_ambient[] = {0.0, 0.0, 0.0, 0.0};

    // glEnable(GL_LIGHTING);
    // // glEnable(GL_COLOR_MATERIAL);

    // glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    // glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    // glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); /*global ambient*/

    // glEnable(GL_LIGHT0);

    // glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
    // glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);

    // glEnable(GL_LIGHT1);

    // glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
    // glLightfv(GL_LIGHT1, GL_SPECULAR, lit_specular);
    // glLightfv(GL_LIGHT1, GL_AMBIENT, lit1_ambient);
    // glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0001);
    // glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0);
    // glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0);

    // glEnable(GL_LIGHT2);

    // glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
    // glLightfv(GL_LIGHT2, GL_SPECULAR, lit_specular);
    // glLightfv(GL_LIGHT2, GL_AMBIENT, lit2_ambient);
    // glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit2_cutoff);
    // glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lit2_exponent);
    // glLightfv(GL_LIGHT2, GL_AMBIENT, lit2_ambient);
    // glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0);
    // glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.001);
    // glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0);

    return true;
}

void WindowManagement::display()
{
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    glfwGetFramebufferSize(window, &(this->width), &(this->height));

    glViewport(0, 0, this->width, this->height);

    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);

    render_scene();
}

void WindowManagement::mainloop()
{
    while (!glfwWindowShouldClose(this->window))
    {
        this->display();

        this->check_keyboard_pressing();

        imgui();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if(ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
        {
            // this->light_color = glm::vec3(0.5f, 0.5f, 0.5f);
            this->clear_color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
        }
        else
        {
            // this->light_color = glm::vec3(1.0f, 1.0f, 1.0f);
            this->clear_color = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
        }


        /* Swap front and back buffers */
        glfwSwapBuffers(this->window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}


//----------------------------------------------------------------



template <typename T>
void WindowManagement::load_image(T &image, string filename, int width, int height)
{
    if(height == 0)
        height = width;

    ifstream file(filename.c_str(), ios::in | ios::binary);

    unsigned char ch;

    // unsigned char ***image = new unsigned char**[256];

    // for(int i = 0; i < 256; i++)
    // {
    //     image[i] = new unsigned char*[256];

    //     for(int j = 0; j < 256; j++)
    //         image[i][j] = new unsigned char[4];
    // }

    for(int i = 0; i < 138; i++)
        file.get();

    for(int i = 0; i < 256*256*4 ; i++)
    {
        ch = file.get();

        int k =  i % 4;

        if(k == 0)
            k = 2;
        else if(k == 2)
            k = 0;

        // cout << (int) ch << " ";

        // if(k == 3)
        //     cout << endl;

        image[i / 4 / 256][i / 4 % 256][k] = ch;
    }

    file.close();
}

bool WindowManagement::texture_init()
{
    // load_image(sand, "./assets/sand.bmp", 256);
    // load_image(seaweed, "./assets/seaweed.bmp", 256);
    // load_image(bubble, "./assets/bubble.bmp", 256);

    glGenTextures(5, textName);

    // make_check_pattern();

    // glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
    // glBindTexture(GL_TEXTURE_2D, textName[0]);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
    //              GL_UNSIGNED_BYTE, checkboard);

    // make_brick_pattern();

    // glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
    // glBindTexture(GL_TEXTURE_2D, textName[1]);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
    //              GL_UNSIGNED_BYTE, brick);

    // glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
    // glBindTexture(GL_TEXTURE_2D, textName[2]);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA,
    //              GL_UNSIGNED_BYTE, sand);

    // glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
    // glBindTexture(GL_TEXTURE_2D, textName[3]);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA,
    //              GL_UNSIGNED_BYTE, seaweed);

    // glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
    // glBindTexture(GL_TEXTURE_2D, textName[4]);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA,
    //              GL_UNSIGNED_BYTE, bubble);

    // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    return true;
}

//-----------------------------------------

void WindowManagement::imgui()
{
    // cout << clip << endl;
    static bool is_load = false;
    static bool is_show = false;
    static string selected_inf = "engine";
    static string selected_raw = "engine";
    static int iso_value = 80;
    static float g_max = 160.0f;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(250, 425), ImGuiCond_Once);

    ImGui::Begin("Is that a bird?");
    {
        ImGui::Text("File");

        if (ImGui::BeginCombo(".inf", selected_inf.c_str()))
        {
            for (int i = 0; i < scalar_infs.size(); i++)
            {
                if (ImGui::Selectable(scalar_infs[i].c_str()))
                {
                    selected_inf = scalar_infs[i];
                    selected_raw = scalar_infs[i];
                    is_load = false;
                    is_show = false;
                }
            }

            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo(".raw", selected_raw.c_str()))
        {
            for (int i = 0; i < scalar_raws.size(); i++)
            {
                if (ImGui::Selectable(scalar_raws[i].c_str()))
                {
                    selected_raw = scalar_raws[i];
                    is_load = false;
                    is_show = false;
                }
            }

            ImGui::EndCombo();
        }

        ImGui::InputInt("Iso Value", &iso_value);

        if (ImGui::Button("Load", ImVec2(100.0f, 30.0f)))
        {
            if(!this->showing_last && this->volumes.size() > 0)
                this->volumes.pop_back();

            is_load = true;

            // if(this->test_volume != NULL)
            //     delete this->test_volume;

            this->volumes.push_back(Volume("./Data/Scalar/" + selected_inf + ".inf", "./Data/Scalar/" + selected_raw + ".raw", iso_value, pow(2, g_max / 20)));

            cout << "Load: " << this->volumes.back().vao.count << endl;

            this->showing_last = false;
        }

        if(this->volumes.size() > 0)
            ImGui::SameLine();

        if (this->volumes.size() > 0 && ImGui::Button("Clear", ImVec2(100.0f, 30.0f)))
        {
            this->volumes.clear();

            is_load = false;

            cout << "Clear" << endl;
        }

        if (is_load && ImGui::Button("Show", ImVec2(100.0f, 30.0f)))
        {
            this->showing_last = true;

            Volume tmp = this->volumes.back();

            this->volumes.clear();

            this->volumes.push_back(tmp);

            cout << "Show" << endl;

            is_load = false;
        }

        if(is_load)
            ImGui::SameLine();

        if (is_load && this->volumes.size() > 1 && ImGui::Button("Blend", ImVec2(100.0f, 30.0f)))
        {
            this->showing_last = true;

            cout << "Blend" << endl;

            is_load = false;
        }

        if(!is_load || this->volumes.size() <= 1)
            ImGui::NewLine();

        ImGui::Text("Slicing Plane");

        ImGui::SliderFloat("x", &(this->clip_x), -1.0f, 1.0f);
        ImGui::SliderFloat("y", &(this->clip_y), -1.0f, 1.0f);
        ImGui::SliderFloat("z", &(this->clip_z), -1.0f, 1.0f);
        ImGui::SliderFloat("clip", &(this->clip), -200.0f, 200.0f);
        {
            glm::vec3 tmp = glm::normalize(glm::vec3(this->clip_x, this->clip_y, this->clip_z));

            this->clip_x = tmp.x;
            this->clip_y = tmp.y;
            this->clip_z = tmp.z;
        }

        ImGui::Checkbox("Section", &enable_section);

        ImGui::Text("Base Color");

        ImGui::SliderFloat("red", &base_color.x, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("green", &base_color.y, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("blue", &base_color.z, 0.0f, 1.0f, "%.2f");
    }
    ImGui::End();

    if(this->volumes.size() > 0)
    {
        ImGui::SetNextWindowPos(ImVec2(this->width - 475, this->height - 425), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(450, 400), ImGuiCond_Once);

        ImGui::Begin("Histogram");
        {
            // ImGui::PlotLines("Frame Times", this->volumes.back().histogram.data(), IM_ARRAYSIZE(this->volumes.back().histogram.data()));
            // const float g[5] = { 0.0f, 50.0f, 150.0f, 100.0f, 45.0f };
            // ImVec2 size = ImGui::GetItemRectSize();
            // ImGui::PlotHistogram(
            //     "Test",
            //     g,
            //     IM_ARRAYSIZE(g),
            //     0,
            //     NULL,
            //     0,
            //     255,
            //     size
            // );

            ImGui::PlotHistogram(
                "voxel value",
                this->volumes.back().histogram.data(),
                this->volumes.back().histogram.size(),
                0,
                NULL,
                0,
                this->volumes.back().histogram_max_value,
                ImVec2(350, 350)
            );


            // ImPlot::SetNextPlotLimits(0.0, 256.0, 0.0, this->volumes.back().histogram_max_value, ImGuiCond_Always);
            // if (ImPlot::BeginPlot("My Plot", "", "", ImVec2(-1, 0))) {
            //     ImPlot::PlotBars(
            //         this->volumes.back().raw_filename.c_str(),
            //         this->volumes.back().histogram.data(),
            //         (int)(this->volumes.back().max_value - this->volumes.back().min_value)
            //     );
            //     ImPlot::EndPlot();
            // }

        }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(25, this->height - 425), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(450, 400), ImGuiCond_Once);
        ImPlot::SetNextPlotLimits(0.0, 256.0, 0.0, g_max, ImGuiCond_Always);

        ImGui::Begin("M-K Table");
        {
            ImGui::InputFloat("gMax", &g_max, 1.0f, 16.0f);

            if(ImGui::Button("recalc mk table"))
            {
                this->volumes.back().calc_mk_table(pow(2, g_max / 20));

                // ImPlot::SetNextPlotLimits(0.0, 256.0, 0.0, g_max, ImGuiCond_Always);
            }

            if (ImPlot::BeginPlot("m-k table"))
            {
                for (int m = 0; m < this->volumes.back().mk_table.size(); m++)
                {
                    for(int k = 0; k < this->volumes.back().mk_table[0].size(); k++)
                    {
                        ImVec2 rect_start = ImPlot::PlotToPixels(ImPlotPoint(m, k+1));
                        ImVec2 rect_end = ImPlot::PlotToPixels(ImPlotPoint(m+1, k));

                        int value = glm::clamp((int)this->volumes.back().mk_table[m][k], 0, 255);

                        ImPlot::PushPlotClipRect();
                        ImGui::GetWindowDrawList()->AddRectFilled(rect_start, rect_end, IM_COL32(value, value, value, 255));
                        ImPlot::PopPlotClipRect();
                    }
                }

                ImPlot::EndPlot();
            }
        }
        ImGui::End();
    }
}



void WindowManagement::render_scene()
{
    this->shader.use();

    glm::mat4 projection;

    if(width > height)
    {
        projection = glm::ortho(
            this->camera.left                              , this->camera.right,
            this->camera.bottom * ((float) height / width) , this->camera.top * ((float) height / width),
            this->camera.near                              , this->camera.far
        );
    }
    else
    {
        projection = glm::ortho(
            this->camera.left * ((float) width /height)  , this->camera.right * ((float) width /height),
            this->camera.bottom                          , this->camera.top,
            this->camera.near                            , this->camera.far
        );
    }

    // projection = glm::perspective(90.0f, (float) width / height, 0.1f, 10000.0f);

    shader.set_uniform("clip", glm::vec4(this->clip_x, this->clip_y, this->clip_z, this->clip));
    shader.set_uniform("projection", projection);
    shader.set_uniform("light_color", light_color);
    shader.set_uniform("enable_section", enable_section);
    shader.set_uniform("base_color", base_color);
    camera.use(shader);

    glm::mat4 model;

    for(int i = 0; i < this->volumes.size(); i++)
    {
        if(i == this->volumes.size() - 1 && !this->showing_last)
            continue;

        model = glm::mat4(1.0f);

        model = glm::translate(model, -glm::vec3(this->volumes[i].resolution) / 2.0f * this->volumes[i].voxelsize);
        shader.set_uniform("model", model);

        this->volumes[i].draw();
    }

    glBindVertexArray(0);
}

//-------------------------------------------------



void WindowManagement::keyboard_down(int key)
{
    // cout << (char) tolower(key) << endl;

    static double x, y;

    switch(key)
    {
        case GLFW_KEY_ESCAPE:  // ESC
            exit(0);

        case GLFW_KEY_R:
            this->shader.reload();

            break;
    }
}

void WindowManagement::check_keyboard_pressing()
{
    if(ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
        return;

    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        this->clip_x += 0.02;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        this->clip_x -= 0.02;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        this->clip_y += 0.02;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        this->clip_y -= 0.02;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        this->clip_z += 0.02;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        this->clip_z -= 0.02;
    }


    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
    {
        this->clip++;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
    {
        this->clip--;
    }

    // if(clip_x > 100)
    //     clip_x = 100;
    // if(clip_x < 0)
    //     clip_x = 0;

    // if(clip_y > 100)
    //     clip_y = 100;
    // if(clip_y < 0)
    //     clip_y = 0;

    // if(clip_z > 100)
    //     clip_z = 100;
    // if(clip_z < 0)
    //     clip_z = 0;

    glm::vec3 tmp = glm::normalize(glm::vec3(this->clip_x, this->clip_y, this->clip_z));

    this->clip_x = tmp.x;
    this->clip_y = tmp.y;
    this->clip_z = tmp.z;

    if(this->clip > 200)
        this->clip = 200;
    if(this->clip < -200)
        this->clip = -200;
}

void WindowManagement::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {

    }

}

void WindowManagement::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
        return;
    // cout << xoffset << " " << yoffset << endl;

    this->camera.zoom(yoffset);
}

void WindowManagement::cursor_callback(GLFWwindow * window, double x, double y)
{
    float x_offset = x - this->last_x;
    float y_offset = y - this->last_y;

    this->last_x = x;
    this->last_y = y;

    if(ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
        return;

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        this->camera.update_yaw(x_offset);
        this->camera.update_pitch(y_offset);
    }
}

void WindowManagement::keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
        return;

    if(action == GLFW_PRESS)
        keyboard_down(key);
}

void WindowManagement::framebuffer_callback(GLFWwindow * w, int new_w, int new_h)
{
    this->width = new_w;
    this->height = new_h;
}