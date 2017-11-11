#version 330

in vec3 varyingColour;

layout( location = 0 ) out vec4 outColour;

void main(void) {

	outColour = vec4( varyingColour, 1.0);
}