#version 460 core

in vec2 vertexUV;
in float fogFactor;
in vec3 normal;
in vec3 fragPos;
in vec3 viewPos;

// control
uniform int showFog = 1;
uniform int showSunshine = 1;

uniform sampler2D world_sampler;
uniform vec3 fogColor=vec3(1.0f,1.0f, 1.0f);

uniform vec3 lightDir=vec3(-0.50, -1.0, 0.2);
uniform vec3 lightColor=vec3(1.0f,1.0f, 1.0f);
uniform float ambientStrength = 0.8;
uniform float specularStrength = 0.9;
uniform int shininess = 32;

out vec4 FragColor;

void main() {
    // 环境光
    vec3 ambient = ambientStrength * lightColor;

    // 漫反射
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(-lightDir); // 取反，因为lightDir是太阳到物体的方向
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    // 镜面反射
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // 组合光照
    vec4 texColor = texture(world_sampler, vertexUV);
    vec3 lighting;
    if(showSunshine==1){
        lighting = (ambient + diffuse + specular) * texColor.rgb;
    }else{
        lighting = texColor.rgb;
    }

    // fog
    vec3 finalColor;
    if(showFog==1){
        finalColor = mix(fogColor, lighting, fogFactor);
    }else{
        finalColor = lighting;
    }
    FragColor = vec4(finalColor, texColor.a);
}