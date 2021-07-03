#version 330

// specify what type of geometry is passed in
layout (triangles) in;		// in this case, interpret three vertices passed in as a triangle 
layout (triangle_strip,		// uses previous two out of three vertices passed in as the base of a triangle formed from the next vertex 
	max_vertices = 18) out; // expect a max of 18 vertices to come out of this shader (additional outputs would be cut off)

uniform mat4 lightMatrices[6];	// there is a light matrix for each side of the cube map (projection * view)

out vec4 FragPos;				// continue passing along gl_Position from vert shader to fragment shader

void main()
{
	// for each face of the cube map
	for (int face = 0; face < 6; face++)
	{
		// in geometry language, gl_Layer is used to select a specific layer 
		// (or face and layer for cube maps) in a multi-layer framebuffer attachement
		gl_Layer = face; 
		for (int i = 0; i < 3; i++) // for each vertex in triangle
		{
			FragPos = gl_in[i].gl_Position; // gl_in is a triangle passed in from "layer (triangles) in", so is an array of 3 vertices
			gl_Position = lightMatrices[face] * FragPos; // gl_Position NOT passed on to frag shader. Triangle vertices position for specific cube map face. 
			EmitVertex(); // emits vertex into triangle strip
		}
		EndPrimitive();   // move on to next face
	}
}