#version 460 core

in vec3 aPosition;
in vec2 aUV;

out vec2 vertexUV;

void main() {
    gl_Position = vec4(aPosition.xyz,1.0f);
    //vertexUV = aUV;
}