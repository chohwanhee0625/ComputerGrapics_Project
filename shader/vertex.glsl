#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNor;
layout (location = 2) in vec2 vTexCoord;

out vec3 Nor;
out vec3 fragPos;
out vec2 TexCoord;

uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 normalWorld;

void main(){
	gl_Position =  proj * view * world * vec4(vPos,1.0f);
	fragPos =  (world * vec4(vPos,1.0f)).xyz;
	TexCoord = vTexCoord;
	Nor = (normalWorld * vec4(vNor,1.0f)).xyz;
}