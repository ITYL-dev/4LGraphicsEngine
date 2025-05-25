#include <iostream>
#include <string>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "utils.h"

// handle the keyboard inputs
void processInput(GLFWwindow* window)
{
    // close on espace
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


// render operations
void render(const unsigned int shaderProgramId, const unsigned int VertexArrayObjectId)
{
    // clear the canvas
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // select the shader program and bind the VAO
    glUseProgram(shaderProgramId);
    glBindVertexArray(VertexArrayObjectId);

    // draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

    // create the vertex shader and load its code 
    std::string vertexShaderStr{ readFileToString("src/shaders/triangle.vert") };
    const char* vertexShaderSrc{ vertexShaderStr.c_str() };
    unsigned int vertexShaderId{ glCreateShader(GL_VERTEX_SHADER) };
    glShaderSource(vertexShaderId, 1, &vertexShaderSrc, NULL);

    // compile the vertex shader
    glCompileShader(vertexShaderId);
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // create the fragment shader and load its code 
    std::string fragmentShaderStr{ readFileToString("src/shaders/triangle.frag") };
    const char* fragmentShaderSrc{ fragmentShaderStr.c_str() };
    unsigned int fragmentShaderId{ glCreateShader(GL_FRAGMENT_SHADER) };
    glShaderSource(fragmentShaderId, 1, &fragmentShaderSrc, NULL);

    // compile the fragment shader
    glCompileShader(fragmentShaderId);
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // linking the shaders into a programm
    unsigned int shaderProgramId{ glCreateProgram() };
    glAttachShader(shaderProgramId, vertexShaderId);
    glAttachShader(shaderProgramId, fragmentShaderId);
    glLinkProgram(shaderProgramId);
    glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    /* END SHADER CODE */

    /* DATA */

    float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // create the VBA, VAO and EBO
    unsigned int VertexBufferObjectId,  VertexArrayObjectId, ElementBuffetObjectId;
    glGenVertexArrays(1, &VertexArrayObjectId);
    glGenBuffers(1, &VertexBufferObjectId);
    glGenBuffers(1, &ElementBuffetObjectId);

    // bind the VAO first ...
    glBindVertexArray(VertexBufferObjectId);

    //... then bind and set vertex buffer(s) ...
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObjectId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffetObjectId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // ... and then configure vertex attributes(s)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* END DATA */

    /* RENDER LOOP */

    while (!glfwWindowShouldClose(window))
    {
        // input handling
        processInput(window);

        // render operations
        render(shaderProgramId, VertexArrayObjectId);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* END RENDER LOOP */

    /* END PROGRAMM */

    glfwTerminate();
	return 0;
}