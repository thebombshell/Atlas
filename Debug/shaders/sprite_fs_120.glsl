#version 120

varying vec3 varyingColor;
varying vec2 varyingTexture;

uniform sampler2D uniformSampler;

void main( void ) {

	gl_FragColor = vec4( texture2D(uniformSampler, varyingTexture).xyz * varyingColor, 1.0 );
}