#version 330

in vec4 FragPos;		// in from geometry shader. Vertex Clip Space coordinate

uniform vec3 lightPos;	// position of light source
uniform float farPlane;	// distance to far plane (limit of light rendering)

void main()
{
	float distance = length (FragPos.xyz - lightPos);	// distance between light and fragment. The "depth" of fragment relative to light source;
	distance = distance / farPlane;						// normalize distance to [1.0, 0.0] depth value
	gl_FragDepth = distance;							// override pre-existing depth buffer value
}