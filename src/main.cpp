#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <vector>

#include "gl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "../include/Mesh.h"

std::vector<Mesh*> meshList;


const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, IBO, shader, uniformModel, uniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.01f;

const float toRadians = 3.14159265f / 108.0f;

bool sizeDirection = true;
float curlSize = 0.4f;
float curAngle = 0.0f;
float maxSize = 0.8f;
float minSize = 0.1f;

// vertex shader
static const char* vShader = " #version 330 core\n"
                             "layout (location = 0) in vec3 aPos;\n"
                             "uniform mat4 model; \n"
                             "uniform mat4 projection; \n"
                             "out vec4 vCol; \n"
                             "\n"
                             "void main() {\n"
                             "gl_Position = projection * model * vec4(aPos, 1.0); \n"
                             "vCol = vec4(clamp(aPos, 0.0f, 1.0f), 1.0f);\n"
                             "}";

static const char* fShader = "#version 330 core\n"
                             "out vec4 FragColor;\n"
                             "in vec4 vCol; \n"
                             "\n"
                             "\n"
                             "void main () {\n"
                             "FragColor = vCol; \n"
                             "}";

void createTriangle () {
    GLfloat vertices [] = {
            -1.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
    };

    unsigned int indices [] = {
            0, 3, 1,
            1,3,2,
            2,3,0,
            0,1,2
    };


    Mesh *obj1 = new Mesh(); // creating new mesh object
    obj1->createMesh(vertices, indices, 4, 12);  // 4 vertices, 12 indices

    meshList.push_back(obj1); // adding the object into the vector???

    Mesh *obj2 = new Mesh(); // creating new mesh object
    obj2->createMesh(vertices, indices, 4, 12);  // 4 vertices, 12 indices

    Mesh *obj3 = new Mesh(); // creating new mesh object
    obj3->createMesh(vertices, indices, 4, 12);  // 4 vertices, 12 indices

    meshList.push_back(obj2); // adding the object into the vector???
    meshList.push_back(obj3); // adding the object into the vector???
}


void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = {0};
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error shader program \n", eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}

void compileShaders () {
    shader = glCreateProgram();

    if (!shader) {
        printf("error shader");
        return;
    }
    addShader(shader, vShader, GL_VERTEX_SHADER);
    addShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking shader program \n", eLog);
        return;
    }

    uniformProjection = glGetUniformLocation(shader, "projection");
    uniformModel = glGetUniformLocation(shader, "model");
}


int main() {
    if (!glfwInit()) {
        printf("GLFW installation failed");
        glfwTerminate();
        return 1;

    }

    //window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // no backwards compatability
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // allows forward compatability

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Learning OpenGL", NULL, NULL);
    if (!window) {
        printf("failed to create window");
        glfwTerminate();
        return 1;
    }

    // getting buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(window); // set context for glew to use

    if (!gladLoadGL(glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    glViewport(0,0, bufferWidth, bufferHeight);

    createTriangle();
    compileShaders();

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth/(GLfloat)bufferHeight, 0.1f, 100.f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // get + handle user input events

        if (direction) {
            triOffset += triIncrement;
        } else {
            triOffset -= triIncrement;
        }

        if (abs(triOffset) >= triMaxOffset) {
            direction = !direction;
        }

        if (sizeDirection) {
            curlSize += 0.001f;
        } else {
            curlSize -= 0.001f;
        }

        if (curlSize >= maxSize || curlSize <= minSize) {
            sizeDirection = !sizeDirection;
        }

        curAngle += 0.1f;
        if (curAngle >= 360.0f) curAngle -= 360.0f;


        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // clear the window and set the color to red.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader);

        glm::mat4 model (1.0f);

        model = glm::translate(model, glm::vec3(0.0f, triOffset, -2.5f));
        //model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[0]->renderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, -triOffset, -2.5f));
        model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->renderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, -triOffset, -2.5f));
        model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[2]->renderMesh();


        glUseProgram(0);


        glfwSwapBuffers(window);
    }

    return 0;
}
