#version 460 core

in vec2 vertexUV;
in float fogFactor;

uniform sampler2D sampler;
uniform vec3 fogColor=vec3(1.0f,1.0f,1.0f);

out vec4 FragColor;

void main() {
    vec4 texColor = texture(sampler, vertexUV);
    vec3 finalColor = mix(fogColor, texColor.rgb, fogFactor);
    FragColor = vec4(finalColor, texColor.a);
}
