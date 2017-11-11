#version 330 core

uniform sampler2D uniformSampler;
uniform float uniformShakeFactor;
uniform float uniformTimer;

out vec4 outColour;

void main( void ) {

	ivec2 texRes = textureSize( uniformSampler, 0 );
	vec2 texCoords = vec2( (gl_FragCoord.x) / float( texRes.x ), (gl_FragCoord.y) / float( texRes.y ) );
	float off = cos( uniformTimer ) * uniformShakeFactor;
	float texSampleR = texture2D(uniformSampler, texCoords + vec2( 0.01, 0.0 ) * off).r;
	float texSampleG = texture2D(uniformSampler, texCoords - vec2( 0.01, 0.0 ) * off).g;
	float texSampleB = texture2D(uniformSampler, texCoords + vec2( 0.0, 0.01 ) * off).b;
	vec3 texSample = vec3( texSampleR, texSampleG, texSampleB );
	outColour = vec4( texSample.xyz, 1.0 );
}