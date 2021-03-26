#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "./Shader.hpp"

class Camera
{
    public:

        glm::vec3 position;

        float yaw;
        float pitch;

        Camera();

        void calc();
        void use(Shader);

        void zoom(float);

        void update_yaw(float);
        void update_pitch(float);

    private:
        glm::vec3 direction;
        glm::vec3 up;
        glm::vec3 right;

        glm::mat4 view;

        float zoom_value;
};