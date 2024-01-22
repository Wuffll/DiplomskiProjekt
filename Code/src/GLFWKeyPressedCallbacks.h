#pragma once

#include <GLFW/glfw3.h>

Camera* pCallbackCamera = nullptr;

Shader* pCallbackShader = nullptr;

unsigned int currentSelectedBone = 0;

unsigned int* pCallbackCurrentSelectedAnimation = nullptr;

MeshV2* pCallbackActiveMesh = nullptr;

float* pCallbackBlendFactor = nullptr;
float blendFactorMultiplier = 1.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        currentSelectedBone++;
        if (currentSelectedBone > 65)
            currentSelectedBone = 0;

        printf("Currently selected bone index: %u\n", currentSelectedBone);

        if (pCallbackShader)
            pCallbackShader->SetUniform1i("uDisplayBoneIndex", currentSelectedBone);
    }
    else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        printf("Selecting next animation!\n");
        
        if(pCallbackActiveMesh)
            pCallbackActiveMesh->SelectNextAnimation();
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        pCallbackActiveMesh->GetTransform().Rotate({ 0.0f, 30.0f, 0.0f });
    }
    else if (key == GLFW_KEY_I && action == GLFW_PRESS)
    {
        pCallbackCamera->Move({ 0.0f, 0.0f, 0.25f });
    }
    else if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        pCallbackCamera->Move({ 0.0f, 0.0f, -0.25f });
    }
    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        pCallbackCamera->Move({ 0.0f, -0.2f, 0.0f });
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        pCallbackCamera->Move({ 0.0f, 0.2f, 0.0f });
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        pCallbackCamera->Move({ 0.2f, 0.0f, 0.0f });
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        pCallbackCamera->Move({ -0.2f, 0.0f, 0.0f });
    }
    else if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        *pCallbackBlendFactor += blendFactorMultiplier * 0.1f;

        if (*pCallbackBlendFactor >= 1.0f)
        {
            *pCallbackBlendFactor = 1.0f;
            blendFactorMultiplier *= -1.0f;
        } else if (*pCallbackBlendFactor <= 0.0f)
        {
            *pCallbackBlendFactor = 0.0f;
            blendFactorMultiplier *= -1.0f;
        }
    }
    else if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        (*pCallbackCurrentSelectedAnimation)++;

        if (*pCallbackCurrentSelectedAnimation > 2)
            *pCallbackCurrentSelectedAnimation = 0;
    }
}