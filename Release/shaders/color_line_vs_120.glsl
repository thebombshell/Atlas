#version 330

in vec3 attributePosition;
in vec3 attributeColor;

out vec3 varyingColour;

uniform mat4 uniformView;
uniform mat4 uniformProjection;

void main(void) {
	gl_Position = uniformProjection * uniformView * vec4(attributePosition, 1.0);
	varyingColour = attributeColor;
}