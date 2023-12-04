#version 330 core

in vec3 fragPos;
in vec3 Nor;
in vec2 TexCoord;
out vec4 fragColor;

uniform vec4 color;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform float materialSH;
uniform sampler2D outTexture;

void main(){
	vec3 normalVector = normalize(Nor);
	vec3 lightVector = normalize(lightPos - fragPos);

	float ambientLight = 0.2;
	vec3 ambient = ambientLight * vec3(1,1,1);

	float diffuseLight = max(dot(normalVector,lightVector),0.0);
	vec3 diffuse = diffuseLight * lightColor;

	fragColor = texture(outTexture,TexCoord) * vec4(ambient + diffuse, 1) * color;
}