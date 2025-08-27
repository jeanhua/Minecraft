#version 460 core

in vec3 aPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transform;

void main() {
    vec4 worldPos = transform * vec4(aPosition, 1.0);
    gl_Position = projectionMatrix * viewMatrix * worldPos;
}