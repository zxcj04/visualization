#include "./WindowManagement.hpp"

#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    WindowManagement* window_management = new WindowManagement();

    bool err = !window_management -> init("ROV");

    if(err)
    {
        exit(1);
    }

    window_management->mainloop();

    return 0;

    // /*----Associate callback func's whith events------*/
    // glutDisplayFunc(display);
    // /*  glutIdleFunc(display); */
    // glutReshapeFunc(reshape);

    // glutIgnoreKeyRepeat(true);

    // glutMouseFunc(mouse);

    // glutKeyboardFunc(keyboard);
    // glutKeyboardUpFunc(keyboard_up);

    // glutSpecialFunc(special_keyboard);
    // glutSpecialUpFunc(special_keyboard_up);

    // glutIdleFunc(idle_func);

    // if(!gladLoadGL())
    // {
    //     printf("Something went wrong!\n");
    //     exit(-1);
    // }

    // #ifdef GLAD_DEBUG
    //     // before every opengl call call pre_gl_call
    //     glad_set_pre_callback(pre_gl_call);
    //     // don't use the callback for glClear
    //     // (glClear could be replaced with your own function)
    //     glad_debug_glClear = glad_glClear;
    // #endif

    // printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

    // if (GLVersion.major < 2)
    // {
    //     printf("Your system doesn't support OpenGL >= 2!\n");
    //     return -1;
    // }

    // printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
    //        glGetString(GL_SHADING_LANGUAGE_VERSION));

    // init();      /*---Initialize other state varibales----*/

    // glutMainLoop();

    // return 0;
}