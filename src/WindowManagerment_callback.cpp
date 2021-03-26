#include "WindowManagement.hpp"

void WindowManagement::keyboard_down(int key)
{
    // cout << (char) tolower(key) << endl;

    static double x, y;

    switch(key)
    {
        case GLFW_KEY_ESCAPE:  // ESC
            exit(0);

        // case GLFW_KEY_Q:
        //     this->enable_cursor = !this->enable_cursor;

        //     if(enable_cursor)
        //     {
        //         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        //         glfwGetCursorPos(window, &x, &y);

        //         this->last_x = x;
        //         this->last_y = y;
        //     }
        //     else
        //         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        //     break;
    }
}

void WindowManagement::keyboard_up(int key)
{
    switch(key)
    {

    }
}

void WindowManagement::keyboard_repeat(int key)
{
    switch(key)
    {

    }
}

void WindowManagement::check_keyboard_pressing()
{
    // if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS ||
    //     glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    // {
    //     this->lit1_diffuse[0] -= 0.005;
    // }

    // if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS ||
    //     glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    // {
    //     this->lit1_diffuse[1] -= 0.005;
    // }

    // if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS ||
    //     glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    // {
    //     this->lit1_diffuse[2] -= 0.005;
    // }

    // if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS ||
    //     glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    // {
    //     this->lit1_diffuse[0] += 0.005;
    // }

    // if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS ||
    //     glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    // {
    //     this->lit1_diffuse[1] += 0.005;
    // }

    // if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS ||
    //     glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
    // {
    //     this->lit1_diffuse[2] += 0.005;
    // }

    // if(lit2_exponent > 128)
    //     lit2_exponent = 128;
    // else if(lit2_exponent < 0)
    //     lit2_exponent = 0;

    // if(lit2_cutoff > 90)
    //     lit2_cutoff = 90;
    // else if(lit2_cutoff < 10)
    //     lit2_cutoff = 10;

    // if(lit1_diffuse[0] > 1.0)
    //     lit1_diffuse[0] = 1.0;
    // else if(lit1_diffuse[0] < 0.0)
    //     lit1_diffuse[0] = 0.0;

    // if(lit1_diffuse[1] > 1.0)
    //     lit1_diffuse[1] = 1.0;
    // else if(lit1_diffuse[1] < 0.0)
    //     lit1_diffuse[1] = 0.0;

    // if(lit1_diffuse[2] > 1.0)
    //     lit1_diffuse[2] = 1.0;
    // else if(lit1_diffuse[2] < 0.0)
    //     lit1_diffuse[2] = 0.0;

    // if(lit2_angle < 0)
    //     lit2_angle += 360;
    // else if(lit2_angle > 360)
    //     lit2_angle -= 360;

    // if(lit2_angle_y < 0)
    //     lit2_angle_y += 360;
    // else if(lit2_angle_y > 360)
    //     lit2_angle_y -= 360;
}


void WindowManagement::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {

    }

}

void WindowManagement::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // cout << xoffset << " " << yoffset << endl;

    this->camera.zoom(yoffset);
}

void WindowManagement::cursor_callback(GLFWwindow * window, double x, double y)
{
    float x_offset = x - this->last_x;
    float y_offset = y - this->last_y;

    this->last_x = x;
    this->last_y = y;

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        this->camera.update_yaw(x_offset);
        this->camera.update_pitch(y_offset);
    }

    // this->entity_handler->rov->angle -= x_offset * sensitivity;

    // if(this->entity_handler->rov->angle > 360)
    //     this->entity_handler->rov->angle -= 360;

    // if(x_offset < 0)
    // {
    //     this->entity_handler->rov->blade_angle[3] += 20;

    //     if(this->entity_handler->rov->blade_angle[3] > 360)
    //         this->entity_handler->rov->blade_angle[3] -= 360;
    // }
    // else if(x_offset > 0)
    // {
    //     this->entity_handler->rov->blade_angle[4] += 20;

    //     if(this->entity_handler->rov->blade_angle[4] > 360)
    //         this->entity_handler->rov->blade_angle[4] -= 360;
    // }

    // this->entity_handler->rov->facing[0] = sin(this->entity_handler->rov->angle * M_PI / 180);
    // this->entity_handler->rov->facing[1] = this->entity_handler->rov->facing[1];
    // this->entity_handler->rov->facing[2] = cos(this->entity_handler->rov->angle * M_PI / 180);
}

void WindowManagement::keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
        keyboard_down(key);
    // else if(action == GLFW_RELEASE)
    //     keyboard_up(key);
    // else if(action == GLFW_REPEAT)
    //     keyboard_repeat(key);
}

void WindowManagement::framebuffer_callback(GLFWwindow * w, int new_w, int new_h)
{
    this->width = new_w;
    this->height = new_h;

    this->info_width = this->width / 2;
    this->info_height = this->height / 2;
}