#version 330 core

uniform vec4 uniformColour;

out vec4 outColour;

void main( void ) {

	outColour = vec4( uniformColour );
}