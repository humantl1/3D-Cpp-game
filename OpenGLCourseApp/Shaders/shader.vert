#version 330																
																			
// vertex data, locations defined in Mesh.CreateMesh()
layout (location = 0) in vec3 pos;											
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
																			
// uniforms (data in from cpp program)
uniform mat4 model;														    
uniform mat4 projection;												    
uniform mat4 view;
uniform mat4 directionalLightTransform; // projection * view (orthogonal)

// data out to fragment shader
out vec4 vColor;					// vertex color											
out vec2 TexCoord;					// corresponding texture coordinate
out vec3 Normal;					// surface normal at vertex
out vec3 FragPos;					// position of fragments in world space (used for light calcs)
out vec4 DirectionalLightSpacePos;  // position of fragment relative to light
																			
void main()																	
{																  
	// gl_Position is stored as clip space coordinates [-1.0, 1.0]			
	gl_Position = projection * view * model * vec4(pos, 1.0);

	DirectionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.0);

    // artifact from testing color interpolation based off of position						
	vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	
	// pass texture mapping coordinate to fragment shader
	TexCoord = tex;

	// corrects normals for non-uniform scaling
	Normal = mat3(transpose(inverse(model))) * norm;

	// pass position of fragment to simulate light relative to it
	FragPos = (model * vec4(pos, 1.0)).xyz;
}