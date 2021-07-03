#version 330
// render everything relative to the omnidirectional light source

layout (location = 0) in vec3 pos;  // position of a vertex

uniform mat4 model;					// convert pos to worldspace

void main()
{
	gl_Position = model * vec4(pos, 1.0); // simply pass on vertex position transformed to world space and stored in gl_position as clip space coordinates [-1.0, 1.0]
}
