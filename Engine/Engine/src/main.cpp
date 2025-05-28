#include <iostream>
#include <vector>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "utils.h"
#include "shaders/shader.h"

// handle the keyboard inputs
void processInput(GLFWwindow* window)
{
    // close on espace
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


// render operations
void render(const std::vector<Shader>& shaderPrograms, const GLuint VAOs[])
{
    // clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // first triangle
    shaderPrograms[0].use();
    glBindVertexArray(VAOs[0]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    // second triangle
    shaderPrograms[1].use();
    glBindVertexArray(VAOs[1]);
    GLfloat cyanValue{ (sin(static_cast<GLfloat>(glfwGetTime())) / 2.0f) + 0.5f };
    shaderPrograms[1].setFloat("cyanValue", cyanValue);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}


int main() {

    /* INITIALIZE GLFW and OpenGL (with GLAD) */

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // debug output, to remove on release
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for Mac OS

    // create the window and openGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Graphics Engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // load openGL functions at runtime using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // setting the debug output
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    /* END INITIALIZE */

    /* SHADER CODE */

    std::vector<Shader> shaderPrograms;
    shaderPrograms.emplace_back(Shader("src/shaders/triangle.vert", "src/shaders/triangle1.frag"));
    shaderPrograms.emplace_back(Shader("src/shaders/triangle.vert", "src/shaders/triangle2.frag"));

    /* END SHADER CODE */

    /* DATA */

    GLuint indices[] = { 0, 1, 2 };

    // first triangle
    GLfloat vertices1[] = {
        // positions          // colors
        0.0f,   -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.9f,  -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
        -0.45f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };
    // second triangle
    GLfloat vertices2[] = {
        // positions         // colors
        0.0f,  -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        0.9f,  -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
        0.45f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };

    // create arrays : VertexBufferObjectIds (VBOs), VertexArrayObjectIds (VAOs), 
    // and ElementBuffetObjectIds (EBOs)
    GLuint VBOs[2], VAOs[2], EBOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    glGenBuffers(2, EBOs);

    // bind the VAO first ...
    glBindVertexArray(VAOs[0]);

    //... then bind and set vertex buffer(s) ...
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // ... and then configure vertex attributes(s)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // bind the VAO first ...
    glBindVertexArray(VAOs[1]);

    //... then bind and set vertex buffer(s) ...
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // ... and then configure vertex attributes(s)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    

    /* END DATA */

    /* RENDER LOOP */

    while (!glfwWindowShouldClose(window))
    {
        // input handling
        processInput(window);

        // render operations
        render(shaderPrograms, VAOs);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* END RENDER LOOP */

    /* END PROGRAMM */

    glfwTerminate();
	return 0;
}