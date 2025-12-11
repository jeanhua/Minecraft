#version 460 core

in vec2 vertexUV;
in float fogFactor;
in vec3 normal;
in vec3 fragPos;
in vec3 viewPos;

// control
uniform int showFog = 1;
uniform int showSunshine = 1;
uniform int rayTest = 0;

uniform sampler2D world_sampler;
uniform vec3 fogColor = vec3(1.0f, 1.0f, 1.0f);

uniform vec3 lightDir = vec3(-0.50, -1.0, 0.2);
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform float ambientStrength = 0.8;
uniform float specularStrength = 0.9;
uniform int shininess = 32;

uniform vec3 rayColor = vec3(1.0f, 1.0f, 1.0f);

out vec4 FragColor;

void main() {
    float f_showSunshine = float(showSunshine);
    float f_showFog = float(showFog);
    float f_rayTest = float(rayTest);

    vec4 texColor = texture(world_sampler, vertexUV);

    // ===== 光照计算 =====
    // 环境光
    vec3 ambient = ambientStrength * lightColor;

    // 漫反射
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(-lightDir);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    // 镜面反射
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), float(shininess));
    vec3 specular = specularStrength * spec * lightColor;

    vec3 litResult = (ambient + diffuse + specular) * texColor.rgb;
    vec3 lighting = mix(texColor.rgb, litResult, f_showSunshine);

    // ===== 雾效 =====
    vec3 finalColor = lighting + (fogColor - lighting) * (1.0 - fogFactor) * f_showFog;

    // ===== rayTest =====
    vec4 normalOutput = vec4(finalColor, texColor.a);
    vec4 rayOutput = vec4(rayColor, 1.0);
    FragColor = mix(normalOutput, rayOutput, f_rayTest);
}