#version 460 core

in vec3 aPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transform;

uniform float fogStart = 300.0;
uniform float fogEnd = 1000.0;

out float fogFactor;

vec3 getCameraPos(mat4 view) {
    mat4 invView = inverse(view);
    return vec3(invView[3]);
}

void main() {
    vec4 worldPos = transform * vec4(aPosition, 1.0);
    vec3 fragPos = worldPos.xyz;
    vec3 viewPos = getCameraPos(viewMatrix);
    float dst = distance(viewPos, fragPos);
    fogFactor = 1.0 - clamp((dst - fogStart) / (fogEnd - fogStart), 0.0, 1.0);
    gl_Position = projectionMatrix * viewMatrix * worldPos;
}