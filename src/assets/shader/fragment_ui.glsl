#version 460 core

out vec4 FragColor;

in vec2 vertexUV;

uniform sampler2D world_sampler;

void main() {
    //FragColor = texture(world_sampler,vertexUV);
    FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
}