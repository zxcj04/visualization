#include "./Camera.hpp"

Camera::Camera()
{
    this->zoom_value = 30.0f;

    this->yaw        = 0.0f;
    this->pitch      = 0.0f;

    this->left   = -20 ;
    this->right  =  20 ;
    this->bottom = -20 ;
    this->top    =  20 ;
    this->near   = 0.1 ;
    this->far    =  50 ;
}

void Camera::calc()
{
    // cout << "\t" << direction.x << " " << direction.y << " " << direction.z << endl;
    this->direction.x = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->direction.y = sin(glm::radians(this->pitch));
    this->direction.z = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->direction = glm::normalize(this->direction);
    // cout << "\t" << direction.x << " " << direction.y << " " << direction.z << endl;

    this->position = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - this->direction);

    this->position *= zoom_value;

    glm::vec3 view_right = glm::normalize(glm::cross(this->direction, glm::vec3(0.0f, 1.0f, 0.0f))); // member consider
    glm::vec3 up = glm::normalize(glm::cross(view_right, this->direction));

    view = glm::lookAt(position, position + direction, up);
}

void Camera::use(Shader shader)
{
    calc();

    // cout << "camera: " << endl;

    // cout << position.x << " " << position.y << " " << position.z << endl;

    // cout << direction.x << " " << direction.y << " " << direction.z << endl;

    // cout << yaw << " " << pitch << endl;

    shader.set_uniform("view", view);
}

void Camera::zoom(float offset)
{
    static float sensitivity = 0.5f;

    float ratio = zoom_value;

    zoom_value -= offset * sensitivity;

    if(zoom_value < 1)
        zoom_value = 1;

    ratio /= zoom_value;

    this->left /= ratio;
    this->right /= ratio;
    this->bottom /= ratio;
    this->top /= ratio;
}

void Camera::update_yaw(float offset)
{
    static float sensitivity = 0.1f;

    yaw -= offset * sensitivity;

    if(yaw > 360.0f)
        yaw -=  360.0f;
    if(yaw <   0.0f)
        yaw +=  360.0f;
}

void Camera::update_pitch(float offset)
{
    static float sensitivity = 0.1f;

    pitch -= offset * sensitivity;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
}