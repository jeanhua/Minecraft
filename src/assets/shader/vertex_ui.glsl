#version 460 core

in vec3 aPosition;

void main() {
    gl_Position = vec4(aPosition.xyz,1.0f);
}