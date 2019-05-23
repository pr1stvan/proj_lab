#version 450 core
    
in vec4 color;				// variable input: interpolated color of vertex shader
out vec4 fragmentColor;		// output that goes to the raster memory as told by glBindFragDataLocation
void main() {
	fragmentColor = color; 
}