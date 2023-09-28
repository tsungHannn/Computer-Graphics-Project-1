#version 410 core

layout (location = 0) in vec3 position;

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;

void main(){
	gl_Position = u_ViewProj * u_Transform * vec4(position, 1.0);
}