#version 460 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skyboxSampler;

void main()
{
    FragColor = texture(skyboxSampler, TexCoords);
}