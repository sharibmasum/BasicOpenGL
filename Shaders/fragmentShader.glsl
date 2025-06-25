#version 330 core
out vec4 FragColor;

in vec4 vCol;
in vec2 texCoord0;
in vec3 Normal;


struct directionalLight {
    vec3 colour;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;

};

uniform sampler2D theTexture;
uniform directionalLight directLight;

void main () {
    vec4 ambientColour = vec4(directLight.colour, 1.0f) * directLight.ambientIntensity;

    float diffuseFactor = max(dot(normalize(Normal), normalize(directLight.direction)), 0.0f);
vec4 diffuseColour = vec4(directLight.colour, 1.0f) * directLight.diffuseIntensity * diffuseFactor;

    FragColor = texture(theTexture, texCoord0) * (ambientColour + diffuseColour);


}