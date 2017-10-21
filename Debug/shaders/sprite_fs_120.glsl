#version 150

in vec3 varyingColor;
in vec2 varyingTexture;

uniform sampler2D uniformSampler;

out vec4 outColour;

void main( void ) {

	ivec2 texRes = textureSize( uniformSampler, 0 );
	vec2 texCoords = vec2( varyingTexture.x / float( texRes.x ), varyingTexture.y / float( texRes.y ) );
	vec4 texSample = texture2D(uniformSampler, texCoords);
	outColour = vec4( texSample.xyz * varyingColor, 1.0 );
}