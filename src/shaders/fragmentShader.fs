#version 430

in vec2 TexCoord;
out vec4 color;

uniform sampler2D img_output;

void main() {
	color = texture(img_output, TexCoord);
}
