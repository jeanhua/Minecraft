#version 460 core

out vec4 FragColor;

in float fogFactor;

uniform int showFog = 1;
uniform vec3 fogColor = vec3(1.0f, 1.0f, 1.0f);

void main() {
    vec3 baseColor = vec3(0.24f, 0.325f, 0.7f);
    vec3 finalColor = baseColor + float(showFog) * (fogColor - baseColor) * (1.0f - fogFactor);
    FragColor = vec4(finalColor, 0.85f);
}