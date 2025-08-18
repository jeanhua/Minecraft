#version 460 core

in vec3 aPosition;
in vec3 aNormal;
in vec2 aUV;

uniform mat4 vpMat;
uniform mat4 posMat;

out vec2 vertexUV;

void main(){
    gl_Position = vpMat*posMat*vec4(aPosition.xyz,1.0);
    vertexUV = aUV;
}