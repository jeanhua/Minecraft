#version 460 core

in vec3 aPosition;
in vec3 aNormal;
in vec2 aUV;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transform;

uniform float fogStart = 300.0;
uniform float fogEnd = 1000.0;

out vec2 vertexUV;
out float fogFactor;
out vec3 normal;
out vec3 fragPos;
out vec3 viewPos;

vec3 getCameraPos(mat4 view) {
    mat4 invView = inverse(view);
    return vec3(invView[3]);
}

void main() {
    vec4 worldPos = transform * vec4(aPosition, 1.0);
    fragPos = worldPos.xyz;

    // 法线矩阵
    mat3 normalMatrix = transpose(inverse(mat3(transform)));
    normal = normalize(normalMatrix * aNormal);

    // 相机位置
    viewPos = getCameraPos(viewMatrix);

    // 雾效
    float dst = distance(viewPos, fragPos);
    fogFactor = 1.0 - clamp((dst - fogStart) / (fogEnd - fogStart), 0.0, 1.0);

    gl_Position = projectionMatrix * viewMatrix * worldPos;
    vertexUV = aUV;
}