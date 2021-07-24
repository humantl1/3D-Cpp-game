#version 330

layout (location = 0) in vec3 pos;  // point on skybox

out vec3 TexCoords;					// actually, same thing as pos vector. Pass to skybox.frag

uniform mat4 projection;
uniform mat4 view;

void main()
{
	TexCoords = pos; // interpolated texture coordinate
	gl_Position = projection * view * vec4(pos, 1.0); // interpolated fragment coordinate
}