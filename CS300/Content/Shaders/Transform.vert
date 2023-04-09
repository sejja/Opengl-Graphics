#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 0) in vec3 aNormal;
layout(location = 0) in vec2 aUVs;

uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uModel;
out vec2 oUVs;
out vec3 oNormal;
out vec3 oPosition;

void main() {
	oUVs = aUVs;
	oNormal = aNormal;
	oPosition = vec3(uModel * vec4(aPos, 1));
	gl_Position = uTransform * uView * uModel * vec4(aPos, 1.0);
}