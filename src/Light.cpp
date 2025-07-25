//
// Created by sharib masum on 2025-06-21.
//

#include "Light.h"

Light::Light() {
    colour = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientIntensity = 1.0f;

    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    diffuseIntensity = 0.0f;


}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat XDirection, GLfloat YDirection,
             GLfloat ZDirection, GLfloat dIntensity) {
    colour = glm::vec3(red, green, blue);
    ambientIntensity = aIntensity;

    direction = glm::vec3(XDirection, YDirection, ZDirection);
    diffuseIntensity = dIntensity;

}

void Light::useLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation,
                     GLfloat directionLocation) {
    glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);

    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);

}

Light::~Light() {

}