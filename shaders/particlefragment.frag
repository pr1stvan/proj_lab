#version 450 core

in vec3 fragment_color;				// variable input: interpolated color of vertex shader
out vec4 color;		// output that goes to the raster memory as told by glBindFragDataLocation

void main() {
	color = vec4(fragment_color, 1); // extend RGB to RGBA
}