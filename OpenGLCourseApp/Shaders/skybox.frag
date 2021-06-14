#version 330

in vec3 TexCoords; // in from skybox.vert

out vec4 color;	

uniform samplerCube skybox;

void main ()
{
	color = texture(skybox, TexCoords); // color output is simply position of the texel on the skybox (referenced via a 3D vector)
}