#version 460 core

out vec4 FragColor;

in float fogFactor;

uniform int showFog = 1;
uniform vec3 fogColor=vec3(1.0f,1.0f, 1.0f);

void main() {
    vec3 finalColor;
    if(showFog==1) {
        finalColor = mix(fogColor, vec3(0.24f, 0.325f, 0.7f), fogFactor);
    }else{
        finalColor = vec3(0.24f, 0.325f, 0.7f);
    }
    FragColor = vec4(finalColor.xyz,0.85f);
}