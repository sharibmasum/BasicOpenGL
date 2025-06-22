#define STB_IMAGE_IMPLEMENTATION

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

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"


std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Window mainWindow;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Texture brickTexture;
Texture dirtTexture;

Light mainLight;


float triOffset = 0.0f;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformAmbientIntensity = 0, uniformAmbientColour = 0;

static const char* vShader = "Shaders/vertexShader.glsl";
static const char* fShader = "Shaders/fragmentShader.glsl";

void createObjects () {
    GLfloat vertices [] = {

            -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
            0.0f, -1.0f, 1.0f,        0.5f, 0.0f,
            1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,       0.5f, 1.0f,
    };

    unsigned int indices [] = {
            0, 3, 1,
            1, 3, 2,
            2, 3, 0,
            0, 1, 2
    };


    Mesh *obj1 = new Mesh(); // creating new mesh object
    obj1->createMesh(vertices, indices, 20, 12);  // 4 vertices, 12 indices

    meshList.push_back(obj1); // adding the object into the vector???
}

void createShader(){
    Shader *shader1 = new Shader();
    shader1->createFromFiles(vShader, fShader); //i dont know waht the arrow is for.
    shaderList.push_back(*shader1);
}

int main() {

    mainWindow = Window();
    mainWindow.initalize();
    glEnable(GL_DEPTH_TEST);


    createObjects();
    createShader();

    camera = Camera(glm::vec3(0.0f,0.0f,3.0f), glm::vec3(0.0f,1.0f,0.0f),-90.0f,0.0f,5.0f,0.05f);

    brickTexture = Texture("Textures/brick.png");
    brickTexture.loadTexture();
    dirtTexture = Texture("Textures/dirt.png");
    dirtTexture.loadTexture();

    mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f);

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth()/(GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.f);

    while (!mainWindow.getShouldClose()) {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents(); // get + handle user input events

        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // clear the window and set the color to red.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].useShader();

        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectionLocation();
        uniformView = shaderList[0].getViewLocation();
        uniformAmbientIntensity = shaderList[0].getAmbientIntensityLocation();
        uniformAmbientColour = shaderList[0].getAmbientColourLocation();

        mainLight.useLight(uniformAmbientIntensity, uniformAmbientColour);

        if (uniformAmbientColour == -1 || uniformAmbientIntensity == -1) {
            std::cout << "Error: One or more light uniforms not found!" << std::endl;
        }

        glm::mat4 model (1.0f);

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        brickTexture.useTexture();
        meshList[0]->renderMesh();

        glUseProgram(0);
        mainWindow.swapBuffers();

    }

    return 0;
}
