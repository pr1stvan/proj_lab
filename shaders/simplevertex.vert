#version 450 core
   
uniform mat4 MVP;			// Model-View-Projection matrix in row-major format
uniform vec3 vertexColor;
uniform float transparency;
in vec3 vertexPosition;		// variable input from Attrib Array selected by glBindAttribLocation

out vec4 color;				// output attribute
void main() {
	color = vec4(vertexColor,transparency); // copy color from input to output
	gl_Position = MVP * vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1); // transform to clipping space
}