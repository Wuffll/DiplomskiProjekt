#pragma comment (lib, "glew32s.lib")
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <string>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Objekt.h"
#include "Transform.h"

#include "MeshV2.h"

#include "Spline.h"
#include "Camera.h"

#include "FpsManager.h"
#include "OpenGLDebugMessageCallback.h"
#include "GLFWKeyPressedCallbacks.h"
#include "Parser.h"
#include "Debug.h"

// change directory to yours

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

GLFWwindow* InitWindow();
glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4* from);

int main(int argc, char* argv[])
{
    std::string ExePath = argv[0];
    ExePath = ExePath.substr(0, ExePath.find_last_of('\\'));

    GLFWwindow* window = InitWindow();

    Shader shader(ExePath + "\\Shaders\\general.glsl");

    pCallbackShader = &shader;

    // Objekt obj("FirstObject", ExePath + "\\Models\\Character.fbx", shader);

    MeshV2 mesh(ExePath + "\\Models\\Character.fbx");
    
    Renderer renderer(shader);

    // renderer.AddDrawableObject(obj);

    Camera camera;
    camera.SetShader("view", &shader);

    camera.SetPosition({ 0.0f, -1.0f, -3.0f });

    Transform projection(glm::perspective(45.0f, 1.0f, 0.1f, 1000.0f));

    shader.Bind();
    shader.SetUniformMatrix4f("projection", projection.GetMatrix());

    shader.SetUniform1i("uDisplayBoneIndex", 0);

    FpsManager fpsManager(120);
    TimeControl timer;
    timer.Start();
    float deltaTime = 0.0f;

    float timeBetweenPoints = 0.016f; // in seconds

    std::vector<aiMatrix4x4> boneTransforms;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        fpsManager.Begin();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // obj.SetActive(toggleModel);

        // Time passed
        deltaTime += (float)timer.End();

        // obj.GetTransform().Rotate({ 0.0f, 0.0f, 180.0f }); // upside down model when going towards screen
 
        // Debugging
        {
            // auto& transform = obj.GetTransform();
            // std::cout << "Position = " << Debug::GlmString(transform.GetPosition()).c_str() << "; Rotation = " << Debug::GlmString(transform.GetOrientation()) << std::endl;
        }

        // renderer.Draw();

        mesh.GetBoneTransforms(boneTransforms);
        for (int i = 0; i < boneTransforms.size(); i++)
        {
            shader.SetUniformMatrix4f("uBones[" + STRING(i) + "]", aiMatrix4x4ToGlm(&boneTransforms[i]));
        }

        mesh.Draw(shader);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        timer.Start();

        do 
        {
            /* Poll for and process events */
            glfwPollEvents();
        } while (!fpsManager.TimeToGo()); // should eventually replace while loop with sleep

        // std::cout << "FPS: " << fpsManager.GetCurrentFps() << std::endl;
    }

    glfwTerminate();
    return 0;
}

inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4* from)
{
    glm::mat4 to;


    to[0][0] = (GLfloat)from->a1; to[0][1] = (GLfloat)from->b1;  to[0][2] = (GLfloat)from->c1; to[0][3] = (GLfloat)from->d1;
    to[1][0] = (GLfloat)from->a2; to[1][1] = (GLfloat)from->b2;  to[1][2] = (GLfloat)from->c2; to[1][3] = (GLfloat)from->d2;
    to[2][0] = (GLfloat)from->a3; to[2][1] = (GLfloat)from->b3;  to[2][2] = (GLfloat)from->c3; to[2][3] = (GLfloat)from->d3;
    to[3][0] = (GLfloat)from->a4; to[3][1] = (GLfloat)from->b4;  to[3][2] = (GLfloat)from->c4; to[3][3] = (GLfloat)from->d4;

    return to;
}

GLFWwindow* InitWindow()
{
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        exit(-1);

    /* Window creation hints */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        printf("ERROR: Unable to create window!\n");
        exit(-1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

    // glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR: GLEW unable to be initialized!" << std::endl;
        exit(-1);
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, NULL);

    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_DEPTH_TEST);

    return window;
}