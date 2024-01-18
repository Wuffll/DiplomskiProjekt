#pragma once

#include <GLFW/glfw3.h>

Shader* pCallbackShader;
unsigned int currentSelectedBone = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        currentSelectedBone++;
        if (currentSelectedBone > 65)
            currentSelectedBone = 0;

        printf("Currently selected bone index: %u\n", currentSelectedBone);

        pCallbackShader->SetUniform1i("uDisplayBoneIndex", currentSelectedBone);
    }
}