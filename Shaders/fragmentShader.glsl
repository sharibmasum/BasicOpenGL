#version 330 core
out vec4 FragColor;

in vec4 vCol;
in vec2 texCoord0;

struct directionalLight {
    vec3 colour;
    float ambientIntensity;
};

uniform sampler2D theTexture;
uniform directionalLight directLight;

void main () {
    vec4 ambientColour = vec4(directLight.colour, 1.0f) * directLight.ambientIntensity;

    FragColor = texture(theTexture, texCoord0) * ambientColour;

}