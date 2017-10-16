#version 120

attribute vec3 attributePosition;
attribute vec3 attributeColor;
attribute vec2 attributeTexture;

varying vec3 varyingColor;
varying vec2 varyingTexture;

uniform mat4 uniformView;
uniform mat4 uniformProjection;

void main( void ) {

	gl_Position = uniformProjection * uniformView * vec4( attributePosition, 1.0 );
	varyingColor = attributeColor;
	varyingTexture = attributeTexture;
}