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

    glfwSwapInterval(0);

    // glfwSetWindowAspectRatio(window, 1, 1);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "Failed to initialize GLAD" << endl;

        return false;
    }

    cout << "OpenGL shader_iso_surface language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    set_callback_functions();

    // IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    system_init();

    this->shader_iso_surface = Shader("./src/shaders/iso_surface.vert", "./src/shaders/iso_surface.frag");
    this->shader_volume_rendering = Shader("./src/shaders/volume_rendering.vert", "./src/shaders/volume_rendering.frag");
    this->shader_streamline = Shader("./src/shaders/streamline.vert", "./src/shaders/streamline.frag", "./src/shaders/streamline.gs");
    this->shader_pca = Shader("./src/shaders/pca.vert", "./src/shaders/pca.frag", "./src/shaders/pca.gs");

    cout << this->shader_iso_surface.ID << endl;
    cout << this->shader_volume_rendering.ID << endl;

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

    this->rgba_polylines.assign(4, ImVector<ImVec2>());

    this->volume_rendering_shading = false;
    this->volume_rendering_last_decay = 1;
    this->volume_rendering_gap = 1;
    this->volume_rendering_modifier = 1;

    this->tapering_modifier = 0.3;

    this->pca_threshold = 10000;

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
    generate_template_combo();

    return true;
}

bool is_not_digit(char c)
{
    return !isdigit(c);
}

bool numeric_string_compare(const string& s1, const string& s2)
{
    // handle empty strings...

    string::const_iterator it1 = s1.begin(), it2 = s2.begin();

    if (isdigit(s1[0]) && isdigit(s2[0])) {
        int n1, n2;
        stringstream ss(s1);
        ss >> n1;
        ss.clear();
        ss.str(s2);
        ss >> n2;

        if (n1 != n2) return n1 < n2;

        it1 = find_if(s1.begin(), s1.end(), is_not_digit);
        it2 = find_if(s2.begin(), s2.end(), is_not_digit);
    }

    return lexicographical_compare(it1, s1.end(), it2, s2.end());
}

void WindowManagement::generate_combo()
{
    // generate methods combo
    this->method = METHODS::ISO_SURFACE;

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

    sort(this->scalar_infs.begin(), this->scalar_infs.end(), numeric_string_compare);
    sort(this->scalar_raws.begin(), this->scalar_raws.end(), numeric_string_compare);

    if((dp = opendir("./Data/Vector/")) != NULL)
    {
        while((dirp = readdir(dp)) != NULL)
        {
            string temp = dirp->d_name;
            int index_vec = temp.find(".vec");

            if(index_vec != string::npos) this->vector_files.push_back(temp.substr(0, index_vec));
        }
    }
    closedir(dp);

    sort(this->vector_files.begin(), this->vector_files.end(), numeric_string_compare);

    this->vector_files.push_back("custom");

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

bool compare(ImVec2 x, ImVec2 y)
{
    return x.x < y.x;
}

void WindowManagement::generate_template_combo()
{
    this->color_template_files.clear();

    DIR *dp;
    dirent *dirp;

    if((dp = opendir("./color_template")) != NULL)
    {
        while((dirp = readdir(dp)) != NULL)
        {
            string temp = dirp->d_name;
            int index_color = temp.find(".color");

            if(index_color != string::npos) this->color_template_files.push_back(temp.substr(0, index_color));
        }
    }
    closedir(dp);

    sort(this->color_template_files.begin(), this->color_template_files.end());
}

void WindowManagement::load_color_template(string name)
{
    if(name == "")
        return;

    cout << "load: " << name << endl;

    this->rgba_polylines.assign(4, ImVector<ImVec2>());

    ifstream color_file;

    color_file.open("color_template/" + name + ".color");

    static float inp_x, inp_y;

    for(int i = 0; i < 4; i++)
    {
        while(true)
        {
            color_file >> inp_x;

            if(inp_x < 0)
                break;

            color_file >> inp_y;

            rgba_polylines[i].push_back(ImVec2(inp_x, inp_y));
        }

        sort(rgba_polylines[i].begin(), rgba_polylines[i].end(), compare);
    }
}

void WindowManagement::imgui()
{
    // cout << clip << endl;
    static bool is_load = false;
    static bool is_show = false;
    static string selected_inf = "engine";
    static string selected_raw = "engine";
    static string selected_vec = "1";
    static int iso_value = 80;
    static float g_max = 160.0f;

    static double gap = 3.0;
    static int iter_times = 500;
    static double h = 0.3;
    static double limit = 0.9;

    static vector<string> selectable_methods = {
        "iso surface",
        "volume rendering",
        "streamline",
        "PCA",
    };
    static string selected_method = "iso surface";

    static bool switch_main = true;
    static bool switch_mk_table = false;
    static bool switch_histogram = false;
    static bool switch_canvas = true;

    static string selected_color = "";
    static char export_name[64] = "";
    static bool update_color = false;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if ((!switch_main || !switch_mk_table || !switch_histogram || !switch_canvas) && ImGui::BeginMainMenuBar())
    {
        if (!switch_main && ImGui::BeginMenu("Main"))
        {
            switch_main = true;

            ImGui::EndMenu();
        }
        if (!switch_mk_table && ImGui::BeginMenu("MK Table"))
        {
            switch_mk_table = true;

            ImGui::EndMenu();
        }
        if (!switch_histogram && ImGui::BeginMenu("Histogram"))
        {
            switch_histogram = true;

            ImGui::EndMenu();
        }
        if (!switch_canvas && ImGui::BeginMenu("Canvas"))
        {
            switch_canvas = !switch_canvas;

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    ImGui::SetNextWindowPos(ImVec2(10, 25), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(250, 450), ImGuiCond_Once);

    if(switch_main)
    {
        ImGui::Begin("Is that a bird?", &switch_main);
        {
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            if(ImGui::BeginCombo("Method", selected_method.c_str()))
            {
                for(int i = 0; i < selectable_methods.size(); i++)
                {
                    if(ImGui::Selectable(selectable_methods[i].c_str()))
                    {
                        selected_method = selectable_methods[i];

                        if(i == METHODS::ISO_SURFACE)
                            this->method = METHODS::ISO_SURFACE;
                        else if(i == METHODS::VOLUME_RENDERING)
                            this->method = METHODS::VOLUME_RENDERING;
                        else if(i == METHODS::STREAMLINE)
                            this->method = METHODS::STREAMLINE;
                        else if(i == METHODS::PCA)
                            this->method = METHODS::PCA;
                    }
                }

                ImGui::EndCombo();
            }

            ImGui::Text("File");

            if(this->method == METHODS::ISO_SURFACE || this->method == METHODS::VOLUME_RENDERING)
            {
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

                if(this->method == METHODS::ISO_SURFACE)
                {
                    ImGui::InputInt("Iso Value", &iso_value);
                }
            }
            else if(this->method == METHODS::STREAMLINE)
            {
                if (ImGui::BeginCombo(".vec", selected_vec.c_str()))
                {
                    for (int i = 0; i < vector_files.size(); i++)
                    {
                        if (ImGui::Selectable(vector_files[i].c_str()))
                        {
                            selected_vec = vector_files[i];
                            is_load = false;
                            is_show = false;
                        }
                    }

                    ImGui::EndCombo();
                }
            }

            if (ImGui::Button("Load", ImVec2(100.0f, 30.0f)))
            {
                if(!this->showing_last)
                {
                    if( (this->method == METHODS::ISO_SURFACE || this->method == METHODS::VOLUME_RENDERING) &&
                         this->volumes.size() > 0)
                        this->volumes.pop_back();

                    else if(this->method == METHODS::STREAMLINE &&
                            this->streamlines.size() > 0)
                        this->streamlines.pop_back();

                    else if(this->method == METHODS::PCA &&
                            this->pcas.size() > 0)
                        this->pcas.pop_back();
                }

                is_load = true;

                // if(this->test_volume != NULL)
                //     delete this->test_volume;
                if( (this->method == METHODS::ISO_SURFACE || this->method == METHODS::VOLUME_RENDERING))
                {
                    this->volumes.push_back(Volume("./Data/Scalar/" + selected_inf + ".inf", "./Data/Scalar/" + selected_raw + ".raw", this->method, iso_value, pow(2, g_max / 20)));

                    cout << "Load: " << this->volumes.back().vao.count << endl;
                }
                else if(this->method == METHODS::STREAMLINE)
                {
                    this->streamlines.push_back(Streamline("./Data/Vector/" + selected_vec + ".vec", gap, iter_times, h, limit));

                    cout << "Load: " << this->streamlines.back().vao_count() << endl;
                }
                else if(this->method == METHODS::PCA)
                {
                    this->pcas.push_back(Pca(pca_threshold));

                    cout << "Load: " << this->pcas.back().vao_count() << endl;
                }

                this->showing_last = false;
            }

            ImGui::SameLine();

            if (ImGui::Button("Clear", ImVec2(100.0f, 30.0f)))
            {
                this->volumes.clear();
                this->streamlines.clear();
                this->pcas.clear();

                is_load = false;

                cout << "Clear" << endl;
            }

            if (is_load && ImGui::Button("Show", ImVec2(100.0f, 30.0f)))
            {
                this->showing_last = true;

                if( (this->method == METHODS::ISO_SURFACE || this->method == METHODS::VOLUME_RENDERING))
                {
                    Volume tmp = this->volumes.back();

                    this->volumes.clear();
                    this->streamlines.clear();
                    this->pcas.clear();

                    this->volumes.push_back(tmp);
                }
                else if(this->method == METHODS::STREAMLINE)
                {
                    Streamline tmp = this->streamlines.back();

                    this->volumes.clear();
                    this->streamlines.clear();
                    this->pcas.clear();

                    this->streamlines.push_back(tmp);
                }
                else if(this->method == METHODS::PCA)
                {
                    Pca tmp = this->pcas.back();

                    this->volumes.clear();
                    this->streamlines.clear();
                    this->pcas.clear();

                    this->pcas.push_back(tmp);
                }

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

            if(this->method == METHODS::VOLUME_RENDERING)
            {
                ImGui::Checkbox("enable shading", &volume_rendering_shading);

                ImGui::NewLine();

                ImGui::InputFloat("decay", &this->volume_rendering_last_decay, 0.05, 0.2);

                if(this->volume_rendering_last_decay < 0)
                    this->volume_rendering_last_decay = 0;
                else if(this->volume_rendering_last_decay > 1)
                    this->volume_rendering_last_decay = 1;

                ImGui::NewLine();

                float old_gap = this->volume_rendering_gap;

                ImGui::InputFloat("gap", &this->volume_rendering_gap, 0.05, 1);

                if(ImGui::GetIO().Framerate < 15.0f && this->volume_rendering_gap < old_gap)
                    this->volume_rendering_gap = old_gap;

                if(this->volume_rendering_gap <= 0.1)
                    this->volume_rendering_gap = 0.1;
                else if(this->volume_rendering_gap > 10)
                    this->volume_rendering_gap = 10;

                ImGui::NewLine();

                ImGui::InputFloat("modifier", &this->volume_rendering_modifier, 0.05, 0.2);

                if(this->volume_rendering_modifier < 0)
                    this->volume_rendering_modifier = 0;
                else if(this->volume_rendering_modifier > 5)
                    this->volume_rendering_modifier = 5;
            }

            static double old_gap = gap;
            static int old_iter_times = iter_times;
            static double old_h = h;
            static double old_limit = limit;

            if(this->method == METHODS::STREAMLINE)
            {
                ImGui::InputDouble("gap##1", &gap, 0.1, 1);
                ImGui::InputInt("iteration times", &iter_times, 10, 50);
                ImGui::InputDouble("h##1", &h, 0.1, 1);
                ImGui::InputDouble("limit", &limit, 0.05, 0.1);

                ImGui::InputFloat("tapering", &this->tapering_modifier, 0.1, 0.5);
            }

            if(old_gap != gap || old_iter_times != iter_times || old_h != h || old_limit != limit)
            {
                if(gap < 0.1)
                    gap = glm::clamp(old_gap, 0.1, old_gap);
                else if(iter_times < 2)
                    iter_times = glm::clamp(old_iter_times, 2, old_iter_times);
                else if(h < 0.1)
                    h = glm::clamp(old_h, 0.1, old_h);
                else if(limit < 0.01)
                    limit = glm::clamp(old_limit, 0.01, old_limit);
                else
                {
                    old_gap = gap;
                    old_iter_times = iter_times;
                    old_h = h;
                    old_limit = limit;
                }

                if(this->streamlines.size() >= 1)
                    this->streamlines.back().reload(gap, iter_times, h, limit);
            }

            static double old_threshold = pca_threshold;

            if(this->method == METHODS::PCA)
            {
                ImGui::InputFloat("threshold##1", &this->pca_threshold, 1000, 5000);
            }

            if(old_threshold != pca_threshold)
            {
                if(pca_threshold < 0)
                    pca_threshold = 0;
                else
                {
                    old_threshold = pca_threshold;

                    if(this->pcas.size() >= 1)
                        this->pcas.back().reload(pca_threshold);
                }
            }

            tapering_modifier = glm::clamp(tapering_modifier, 0.01f, 5.0f);

            if(this->method == METHODS::ISO_SURFACE)
            {
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
            }

            if(this->method == METHODS::ISO_SURFACE)
            {
                ImGui::Text("Base Color");

                ImGui::SliderFloat("red", &base_color.x, 0.0f, 1.0f, "%.2f");
                ImGui::SliderFloat("green", &base_color.y, 0.0f, 1.0f, "%.2f");
                ImGui::SliderFloat("blue", &base_color.z, 0.0f, 1.0f, "%.2f");
            }
        }
        ImGui::End();
    }

    if(this->volumes.size() > 0)
    {
        ImGui::SetNextWindowPos(ImVec2(this->width - 475, this->height - 425), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(450, 400), ImGuiCond_Once);

        if(switch_histogram)
        {
            ImGui::Begin("Histogram", &switch_histogram);
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
        }

        if(switch_mk_table)
        {

            ImGui::SetNextWindowPos(ImVec2(25, this->height - 450), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(450, 425), ImGuiCond_Once);
            ImPlot::SetNextPlotLimits(0.0, 256.0, 0.0, g_max, ImGuiCond_Always);

            ImGui::Begin("M-K Table", &switch_mk_table);
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

    bool last_volume_is_vr = this->volumes.size() >=1 && this->volumes.back().method == METHODS::VOLUME_RENDERING;
    bool last_streamline = this->streamlines.size() >=1;

    if((last_volume_is_vr || last_streamline) && switch_canvas)
    {

        ImGui::SetNextWindowPos(ImVec2(this->width - 575, this->height - 475), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(550, 450), ImGuiCond_Once);

        ImGui::Begin("Canvas", &switch_canvas);
        {
            static int now_designing = 0;
            static int now_hovering = -1;

            static bool opt_enable_grid = true;

            if (ImGui::BeginCombo(".color##1", selected_color.c_str()))
            {
                for (int i = 0; i < color_template_files.size(); i++)
                    if (ImGui::Selectable(color_template_files[i].c_str()))
                        selected_color = color_template_files[i];

                ImGui::EndCombo();
            }

            ImGui::SameLine();

            if(ImGui::Button("Import"))
            {
                load_color_template(selected_color);

                update_color = true;
            }

            ImGui::SameLine();

            if(ImGui::Button("(R)"))
                generate_template_combo();

            ImGui::SliderInt("color", &now_designing, 0, 3);

            ImGui::Checkbox("Enable grid", &opt_enable_grid);

            if(ImGui::Button("Clear Points"))
                for(int i = 0; i < 4; i++)
                    rgba_polylines[i].clear();

            ImGui::InputText(".color##2", export_name, IM_ARRAYSIZE(export_name));

            ImGui::SameLine();

            if(ImGui::Button("Export") && string(export_name) != "")
            {
                generate_template_combo();

                if(find(color_template_files.begin(), color_template_files.end(), string(export_name)) != color_template_files.end())
                    ImGui::OpenPopup("Duplicate File Name");
                else
                {
                    ofstream color_file;
                    color_file.open ("color_template/" + string(export_name) + ".color");

                    for(int i = 0; i < 4; i++)
                    {
                        for(auto point: rgba_polylines[i])
                        {
                            color_file << point.x << " " << point.y << endl;
                        }

                        color_file << -1 << endl;
                    }

                    strcpy(export_name, "");
                }
            }

            // Always center this window when appearing
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal("Duplicate File Name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Continue?\n\n");
                ImGui::Separator();

                if (ImGui::Button("Overwrite", ImVec2(120, 0)))
                {
                    ofstream color_file;
                    color_file.open ("color_template/" + string(export_name) + ".color");

                    for(int i = 0; i < 4; i++)
                    {
                        for(auto point: rgba_polylines[i])
                        {
                            color_file << point.x << " " << point.y << endl;
                        }

                        color_file << -1 << endl;
                    }
                    strcpy(export_name, "");

                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                    ImGui::CloseCurrentPopup();

                ImGui::EndPopup();
            }

            // Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
            ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
            // ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
            ImVec2 canvas_sz = ImVec2(510, 255);
            // if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
            // if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
            ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

            // Draw border and background color
            ImGuiIO& io = ImGui::GetIO();
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
            draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

            // This will catch our interactions
            ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
            const bool is_hovered = ImGui::IsItemHovered(); // Hovered
            const bool is_active = ImGui::IsItemActive();   // Held
            const ImVec2 origin(canvas_p0.x, canvas_p0.y); // Lock scrolled origin
            const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

            static bool drag = false;
            static int dragging = -1;

            if(now_hovering < 0 && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                if(mouse_pos_in_canvas.x > 0 && mouse_pos_in_canvas.x < canvas_p1.x &&
                mouse_pos_in_canvas.y > 0 && mouse_pos_in_canvas.y < canvas_p1.y)
                    rgba_polylines[now_designing].push_back(mouse_pos_in_canvas);

                sort(rgba_polylines[now_designing].begin(), rgba_polylines[now_designing].end(), compare);
            }
            else if(ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            {
                drag = false;
            }

            if(!drag)
            {
                if(ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    drag = true;

                int cnt = 0;

                ImVec2 * i;

                for(int colors = 0; colors < 4 ; ++colors)
                {
                    for(i = rgba_polylines[colors].begin(); i != rgba_polylines[colors].end(); i++)
                    {
                        if(glm::distance(glm::vec2(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y), glm::vec2(i->x, i->y)) <= 5.5f)
                        {
                            dragging = i - rgba_polylines[colors].begin();

                            now_hovering = colors;

                            if(ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                                now_designing = colors;

                            break;
                        }
                    }

                    if(i == rgba_polylines[colors].end())
                        cnt++;
                }

                if(cnt == 4)
                {
                    dragging = -1;

                    now_hovering = -1;
                }
                else if(ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
                {
                    rgba_polylines[now_hovering].erase(rgba_polylines[now_hovering].begin() + dragging);
                }
            }
            else if(dragging >= 0)
            {
                if(dragging > 0 && mouse_pos_in_canvas.x < rgba_polylines[now_designing][dragging - 1].x)
                {
                    rgba_polylines[now_designing][dragging].x = rgba_polylines[now_designing][dragging - 1].x;

                    rgba_polylines[now_designing][dragging].y = mouse_pos_in_canvas.y;
                }
                else if(dragging < rgba_polylines[now_designing].size() - 1 && mouse_pos_in_canvas.x >= rgba_polylines[now_designing][dragging + 1].x)
                {
                    rgba_polylines[now_designing][dragging].x = rgba_polylines[now_designing][dragging + 1].x;

                    rgba_polylines[now_designing][dragging].y = mouse_pos_in_canvas.y;
                }
                else
                    rgba_polylines[now_designing][dragging] = mouse_pos_in_canvas;

                rgba_polylines[now_designing][dragging].x = glm::clamp(rgba_polylines[now_designing][dragging].x, 0.0f, canvas_sz.x);
                rgba_polylines[now_designing][dragging].y = glm::clamp(rgba_polylines[now_designing][dragging].y, 0.0f, canvas_sz.y);
            }


            // Draw grid + all lines in the canvas
            draw_list->PushClipRect(canvas_p0, canvas_p1, true);
            if (opt_enable_grid)
            {
                const float GRID_STEP = 32.0f;
                for (float x = 0; x < canvas_sz.x; x += GRID_STEP)
                    draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
                for (float y = 0; y < canvas_sz.y; y += GRID_STEP)
                    draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
            }

            static unsigned int now_color[] = {
                IM_COL32(255, 0, 0, 255),
                IM_COL32(0, 255, 0, 255),
                IM_COL32(0, 0, 255, 255),
                IM_COL32(255, 255, 255, 127),
            };

            for(int colors = 0; colors < 4 ; ++colors)
            {
                if(rgba_polylines[colors].Size >= 2)
                    for (int n = 0; n < rgba_polylines[colors].Size - 1; n += 1)
                    {
                        draw_list->AddLine(
                            ImVec2(origin.x + rgba_polylines[colors][n].x, origin.y + rgba_polylines[colors][n].y),
                            ImVec2(origin.x + rgba_polylines[colors][n + 1].x, origin.y + rgba_polylines[colors][n + 1].y),
                            now_color[colors],
                            2.0f
                        );
                    }

                for(int i = 0; i < rgba_polylines[colors].Size; i++)
                {
                    if(colors == now_hovering && i == dragging)
                        if(drag)
                            draw_list->AddCircleFilled(ImVec2(origin.x + rgba_polylines[colors][i].x, origin.y + rgba_polylines[colors][i].y), 4.5f, IM_COL32(255, 255, 255, 255));
                        else
                            draw_list->AddCircleFilled(ImVec2(origin.x + rgba_polylines[colors][i].x, origin.y + rgba_polylines[colors][i].y), 6.25f, IM_COL32(255, 255, 255, 255));
                    else
                        draw_list->AddCircleFilled(ImVec2(origin.x + rgba_polylines[colors][i].x, origin.y + rgba_polylines[colors][i].y), 5.5f, IM_COL32(255, 255, 255, 255));

                    draw_list->AddCircleFilled(ImVec2(origin.x + rgba_polylines[colors][i].x, origin.y + rgba_polylines[colors][i].y), 4.0f, now_color[colors]);
                }
            }

            draw_list->PopClipRect();

            if(!update_color && ImGui::Button("start updating"))
                update_color = true;

            if(update_color && ImGui::Button("stop updating"))
                update_color = false;

            if(update_color)
            {
                if(this->method == METHODS::VOLUME_RENDERING && this->volumes.size() >= 1)
                    this->volumes.back().reload_1d_texture(rgba_polylines);

                if(this->method == METHODS::STREAMLINE && this->streamlines.size() >= 1)
                    this->streamlines.back().reload_1d_texture(rgba_polylines);
            }

            ImGui::End();
        }
    }
}



void WindowManagement::render_scene()
{
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

    glm::mat4 model;

    for(int i = 0; i < this->volumes.size(); i++)
    {
        if(this->volumes[i].method == METHODS::ISO_SURFACE)
        {
            this->shader_iso_surface.use();

            shader_iso_surface.set_uniform("clip", glm::vec4(this->clip_x, this->clip_y, this->clip_z, this->clip));
            shader_iso_surface.set_uniform("projection", projection);
            shader_iso_surface.set_uniform("light_color", light_color);
            shader_iso_surface.set_uniform("enable_section", enable_section);
            shader_iso_surface.set_uniform("base_color", base_color);
            camera.use(shader_iso_surface);
        }
        else if(this->volumes[i].method == METHODS::VOLUME_RENDERING)
        {
            this->shader_volume_rendering.use();

            shader_volume_rendering.set_uniform("clip", glm::vec4(this->clip_x, this->clip_y, this->clip_z, this->clip));
            shader_volume_rendering.set_uniform("projection", projection);
            shader_volume_rendering.set_uniform("light_color", light_color);
            shader_volume_rendering.set_uniform("enable_section", enable_section);
            shader_volume_rendering.set_uniform("using_texture1", 1);
            shader_volume_rendering.set_uniform("using_texture2", 2);
            shader_volume_rendering.set_uniform("resolution", this->volumes[i].resolution);
            shader_volume_rendering.set_uniform("voxelsize", this->volumes[i].voxelsize);
            shader_volume_rendering.set_uniform("shade", volume_rendering_shading);
            shader_volume_rendering.set_uniform("gap", volume_rendering_gap);
            shader_volume_rendering.set_uniform("last_decay", volume_rendering_last_decay);
            shader_volume_rendering.set_uniform("modifier", volume_rendering_modifier);
            camera.use(shader_volume_rendering);
        }

        if(i == this->volumes.size() - 1 && !this->showing_last)
            continue;

        model = glm::mat4(1.0f);

        if(this->volumes[i].method == METHODS::ISO_SURFACE)
        {
            model = glm::translate(model, -glm::vec3(this->volumes[i].resolution) / 2.0f * this->volumes[i].voxelsize);
            shader_iso_surface.set_uniform("model", model);
        }
        else if(this->volumes[i].method == METHODS::VOLUME_RENDERING)
        {
            model = glm::scale(model, this->volumes[i].voxelsize);
            model = glm::translate(model, -glm::vec3(this->volumes[i].resolution) / 2.0f);
            shader_volume_rendering.set_uniform("model", model);
        }

        this->volumes[i].draw();
    }

    for(int i = 0; i < this->streamlines.size(); i++)
    {
        this->shader_streamline.use();

        shader_streamline.set_uniform("projection", projection);
        shader_streamline.set_uniform("modifier", tapering_modifier);
        shader_streamline.set_uniform("using_texture1", 1);
        shader_streamline.set_uniform("value_max", (float) this->streamlines[i].max_value());
        camera.use(shader_streamline);

        if(i == this->streamlines.size() - 1 && !this->showing_last)
            continue;

        model = glm::mat4(1.0f);
        model = glm::translate(model, -glm::vec3(this->streamlines[i].resolution() / 2.0f));
        shader_streamline.set_uniform("model", model);

        this->streamlines[i].draw();
    }

    for(int i = 0; i < this->pcas.size(); i++)
    {
        this->shader_pca.use();

        shader_pca.set_uniform("projection", projection);
        shader_pca.set_uniform("using_texture1", 1);
        camera.use(shader_pca);

        if(i == this->pcas.size() - 1 && !this->showing_last)
            continue;

        model = glm::mat4(1.0f);
        // model = glm::translate(model, -glm::vec3(this->streamlines[i].resolution() / 2.0f));
        shader_pca.set_uniform("model", model);

        this->pcas[i].draw();
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
            this->shader_iso_surface.reload();
            this->shader_volume_rendering.reload();
            this->shader_streamline.reload();
            this->shader_pca.reload();

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