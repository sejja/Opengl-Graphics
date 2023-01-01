#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 0) in vec3 aNormal;
layout(location = 0) in vec2 aUVs;

uniform mat4 uTransform;
uniform mat4 uView;
out vec2 oUVs;

void main() {
	gl_Position = uTransform * uView * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	oUVs = aUVs;
}