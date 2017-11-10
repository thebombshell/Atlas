#version 120

varying vec3 varyingColor;

void main(void) {
	gl_FragColor = vec4(varyingColor, 1.0);
}