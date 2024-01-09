#pragma once

#include <GLFW/glfw3.h>

bool toggleModel = true, toggleRotation = false;

void ToggleModel()
{
    toggleModel = !toggleModel;
}

void ToggleRotationMethod()
{
    toggleRotation = !toggleRotation;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        ToggleModel();
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        ToggleRotationMethod();
    }
}