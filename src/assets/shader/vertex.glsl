#version 460 core

in vec3 aPosition;
in vec3 aNormal;
in vec2 aUV;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transform;

uniform float fogDensity = 0.002;

out vec2 vertexUV;
out float fogFactor;

vec3 getCameraPos(mat4 view) {
    mat4 invView = inverse(view);
    return vec3(invView[3]);
}

void main() {
    vec4 worldPos = transform * vec4(aPosition, 1.0);
    vec3 camPos = getCameraPos(viewMatrix);
    float dst = distance(camPos, worldPos.xyz);
    fogFactor = exp(-pow(fogDensity * dst, 2.0));
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    gl_Position = projectionMatrix * viewMatrix * worldPos;
    vertexUV = aUV;
}
