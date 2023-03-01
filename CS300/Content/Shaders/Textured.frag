#version 460 core

out vec4 FragColor;
in vec2 oUVs;

uniform sampler2D colorsTex;

void main() {
	//FragColor = vec4(oUVs, 0, 1) + vec4(texture(colorsTex, oUVs).rgb, 1);
	FragColor = vec4(texture(colorsTex, oUVs).rgb, 1);
}