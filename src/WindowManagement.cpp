#include "WindowManagement.hpp"

#define GLFW_MINOR_VERSION 6

WindowManagement::WindowManagement()
{
    cout << "WindowManagement init" << endl;

    this->horizon_distance = 20;

    this->pressing = {};

    this->enable_cursor = true;

    this->show_shadow_points = false;

    this->enable_lit0 = true;
    this->enable_lit1 = true;
    this->enable_lit2 = true;

    this->global_ambient[0] = 0.2;
    this->global_ambient[1] = 0.2;
    this->global_ambient[2] = 0.2;
    this->global_ambient[3] = 1.0;

    this->lit_position[0] = 0.0;
    this->lit_position[1] = 1.0;
    this->lit_position[2] = 0.0;
    this->lit_position[3] = 0.0;

    this->lit1_position[0] = 0.0;
    this->lit1_position[1] = 50.0;
    this->lit1_position[2] = 0.0;
    this->lit1_position[3] = 1.0;

    this->lit2_position[0] = 0.0;
    this->lit2_position[1] = 50.0;
    this->lit2_position[2] = 0.0;
    this->lit2_position[3] = 1.0;

    this->lit2_direction[0] = 0.0;
    this->lit2_direction[1] = 50.0;
    this->lit2_direction[2] = 0.0;
    this->lit2_direction[3] = 1.0;

    this->lit2_angle = 0.0;
    this->lit2_angle_y = 0.0;
    this->lit2_cutoff = 60.0;
    this->lit2_exponent = 8.0;

    this->lit1_diffuse[0] = 0.075;
    this->lit1_diffuse[1] = 0.10;
    this->lit1_diffuse[2] = 0.075;
    this->lit1_diffuse[3] = 1.0;

    this->black[0] = 0;
    this->black[1] = 0;
    this->black[2] = 0;
    this->black[3] = 1;

    this->white[0] = 1;
    this->white[1] = 1;
    this->white[2] = 1;
    this->white[3] = 0;
}

WindowManagement::~WindowManagement()
{
    // Destroy glText
    gltTerminate();

	glfwDestroyWindow(this->window);

    cout << "terminate" << endl;

    glfwTerminate();
}

bool WindowManagement::init(string window_name)
{
    GLuint err = !glfwInit();
    if (err)
    {
        cout << "glfwInit ERROR" << endl;

        return false;
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

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

    info_width = width / 2;
    info_height = height / 2;

    this->window = glfwCreateWindow(width, height, window_name.c_str(), NULL, NULL);

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

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "Failed to initialize GLAD" << endl;

        return false;
    }

    cout << "OpenGL shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    set_callback_functions();

    system_init();

    this->shader = Shader("./src/shaders/test.vert", "./src/shaders/test.frag");

    cout << this->shader.ID << endl;

    return true;
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_PROGRAM_POINT_SIZE);

    glClearColor(0.15, 0.15, 0.5, 1.0);

    texture_init();

    float colors[4][3] = {
                            {0.08, 0.21, 0.17},
                            {0.16, 0.39, 0.4},
                            {0.25, 0.40, 0.32},
                            {0.12, 0.31, 0.26}
                          };

    random_device rd;
    // default_random_engine gen = default_random_engine(rd());
    mt19937 gen(random_device{}());

    uniform_int_distribution<int> dis(0,3);

    for(int i=-200;i<201;i++)
        for(int j=-200;j<201;j++)
        {
            int r = dis(gen);

            floor_color[i + 200][j + 200][0] = colors[r][0];
            floor_color[i + 200][j + 200][1] = colors[r][1];
            floor_color[i + 200][j + 200][2] = colors[r][2];
        }

    light_init();
    font_init();

    // glCullFace(GL_BACK);
    // glEnable(GL_CULL_FACE);

    static float fog_color[]={0.15, 0.20, 0.20, 0.50};

    glEnable(GL_FOG);                /*enable fog fade */
    glFogi(GL_FOG_MODE, GL_LINEAR);  /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
    glFogf(GL_FOG_DENSITY, 0.15);    /*fog opacity(density)= 0.25*/
    glFogf(GL_FOG_START, 250.0);       /*Setup two ends for GL_LINEAR*/
    glFogf(GL_FOG_END, 400.0);
    glFogfv(GL_FOG_COLOR, fog_color);/*set the fog color */

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

    glEnable(GL_LIGHTING);
    // glEnable(GL_COLOR_MATERIAL);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); /*global ambient*/

    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);

    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lit_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lit1_ambient);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0001);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0);

    glEnable(GL_LIGHT2);

    glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, lit_specular);
    glLightfv(GL_LIGHT2, GL_AMBIENT, lit2_ambient);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit2_cutoff);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lit2_exponent);
    glLightfv(GL_LIGHT2, GL_AMBIENT, lit2_ambient);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.001);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0);

    return true;
}

bool WindowManagement::font_init()
{
    if (!gltInit())
	{
		fprintf(stderr, "Failed to initialize glText\n");
		glfwTerminate();
		return false;
	}

    return true;
}

void WindowManagement::display()
{
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    glfwGetFramebufferSize(window, &(this->width), &(this->height));

    glViewport(0, 0, this->width, this->height);

    render_scene(FIRST);
    // render_scene(SECOND);

    render_scene(GOD);

	// glUseProgram(0);

    render_scene(THIRD);

    render_scene(INFO);
}

void draw_string(string inp, int x, int y)
{
    // Creating text
    GLTtext *text = gltCreateText();
    gltSetText(text, inp.c_str());

    // Begin text drawing (this for instance calls glUseProgram)
    gltBeginDraw();

        // Draw any amount of text between begin and end
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText2D(text, x, y, 1.15f);

    // Finish drawing text
    gltEndDraw();
	glUseProgram(0);

    // Deleting text
    gltDeleteText(text);
}

void WindowManagement::show_info()
{
    // glViewport(width - info_width, 0, info_width, info_height);
    gltViewport(info_width, info_height);

    glColor3f(1.0, 1.0, 1.0);

    vector<string> text = {
        // "Position: (" + to_string((int)this->entity_handler->rov->position[0]) + ", " + to_string((int)this->entity_handler->rov->position[1]) + ", " + to_string((int)this->entity_handler->rov->position[2]) + ")",
        // "Facing  : (" + to_string(this->entity_handler->rov->facing[0]) + ", " + to_string(this->entity_handler->rov->facing[1]) + ", " + to_string(this->entity_handler->rov->facing[2]) + ")",
        // "Angel   :  " + to_string((int)this->entity_handler->rov->angle) ,
        // "Speed   :"
    };

    int origin_y = info_height - 15;

    int y = 15;

    for(auto &s: text)
    {
        draw_string(s, 9, y);

        y += 30;
        origin_y -= 30;
    }

    vector<string> which = {"",
                            "",
                            ""};

    for(int i = 0; i < 3 ; i++)
    {
        // draw_string(which[i], 9, y);

        // draw_speed_bar(origin_y, fabs(this->entity_handler->rov->speed[i] / 4), info_width);

        origin_y -= 32;
    }
}

void WindowManagement::mainloop()
{
    while (!glfwWindowShouldClose(this->window))
    {
        this->display();

        /* Swap front and back buffers */
        glfwSwapBuffers(this->window);

        /* Poll for and process events */
        glfwPollEvents();

        check_keyboard_pressing();
    }
}