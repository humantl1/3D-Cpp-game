#version 330
// render everything relative to the directional light source


layout (location = 0) in vec3 pos; // position of a vertex

uniform mat4 model; // convert pos to worldspace
uniform mat4 directionalLightTransform; // projection * view (orthogonal)

void main()
{
	gl_Position = directionalLightTransform * model * vec4(pos, 1.0); // convert pos into worldspace relative to light
	// depth is automatically calculated. Also, the shadow map is automatically updated with the shader's depth info (via the ShadowMap object)
	
}