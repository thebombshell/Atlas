#version 330 core

uniform sampler2D uniformSampler;
uniform float uniformShakeFactor;
uniform float uniformTimer;
uniform float uniformGlowFactor;

out vec4 outColour;

vec3 hsv2rgb( vec3 hsv ) {
	vec4 K = vec4( 1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0 );
	vec3 p = abs( fract( hsv.xxx + K.xyz ) * 6.0 - K.www );
	return hsv.z * mix( K.xxx, clamp( p - K.xxx, 0.0, 1.0 ), hsv.y );
}

void main( void ) {

	ivec2 texRes = textureSize( uniformSampler, 0 );
	vec2 texCoords = vec2( (gl_FragCoord.x) / float( texRes.x ), (gl_FragCoord.y) / float( texRes.y ) );
	float off = cos( uniformTimer ) * uniformShakeFactor;
	float texSampleR = texture2D(uniformSampler, texCoords + vec2( 0.01, 0.0 ) * off).r;
	float texSampleG = texture2D(uniformSampler, texCoords - vec2( 0.01, 0.0 ) * off).g;
	float texSampleB = texture2D(uniformSampler, texCoords + vec2( 0.0, 0.01 ) * off).b;
	vec3 texSample = vec3( texSampleR, texSampleG, texSampleB );
	outColour = vec4( texSample.xyz * hsv2rgb( vec3( uniformTimer * 0.5f, 1.0, 1.0 ) ), 1.0 );
}