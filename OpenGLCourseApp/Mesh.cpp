#include "Mesh.h"

Mesh::Mesh() : VAO{0}, VBO{0}, IBO{0}, indexCount{0} {}

/* Setup Mesh of Vertices:
*  1. Generate vertex array object (VAO) name and bind as active
*  2. Generate index buffer object (IBO) name and bind as active
*  3. Store IBO data
*  4. Generate vertex buffer object (VBO) name and bind as active
*  5. Store VBO data
*  6. Define specific attributes inside VBO (position, texture mapping, vertex normal)
*  7. Unbind all objects
*/

void Mesh::CreateMesh(GLfloat *vertices,		// pointer to vertices array
					unsigned int *indices,      // pointer to indices array
					unsigned int numOfVertices, // total number elements in vertex array
					unsigned int numOfIndices)  // total number of elements in index array
{
	indexCount = numOfIndices; // initialize member variable

	// Note: indent formatting indicates object binding 

	// Generate vertex array object (VAO) name(s) to be used as a reference outside of GPU
	glGenVertexArrays(1,	// number of VAO names to generate
					&VAO);	// location in which to store VAO name(s)

	glBindVertexArray(VAO); // bind VAO as active 

		// Generate index buffer object (IBO) name(s) to be used as a reference outside of GPU 
		glGenBuffers(1,		// number of IBO names to generate 
					&IBO);	// location in which to store IBO name(s)

		// bind IBO as active, can be used for different object types
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // Specify type of buffer object (vertex array indices), and name					 

			// Create and initialize bound object's (IBO) data store
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,			// type of buffer object (IBO aka vertex array indices)   
						sizeof(indices[0]) * numOfIndices,  // size of data, in bytes
						indices,							// pointer to data
						GL_STATIC_DRAW);					// expected usage (modified once & used many times, used for drawing)

			// Generate vertex buffer object (VBO) name(s) to be used as a reference outside of GPU
			glGenBuffers(1,		// number of VBO names to generate 
						&VBO);	// location in which to store VBO names

			// bind VBO as active
			glBindBuffer(GL_ARRAY_BUFFER, VBO); // Specify type of buffer object (vertex array indices), and name 
			
				// Create and initialize bound object's (VBO) data store
				glBufferData(GL_ARRAY_BUFFER,					 // type of buffer object (VBO aka vertex buffer indices)
							sizeof(vertices[0]) * numOfVertices, // size of data, in bytes
							vertices,							 // pointer to data
							GL_STATIC_DRAW);					 // expected usage (modified once & used many times, used for drawing)

				// Define specific attributes inside VBO vertices array (positions, texture mapping, vertex normals)
				// x, y, z positions
				glVertexAttribPointer(0,					 // index of attribute (0, first attribute in vertices array) referenced in shader.vert
									3,						 // number of components in attribute (3 for x, y, z)
									GL_FLOAT,				 // type of attribute 
									GL_FALSE,				 // should be normalize?
									sizeof(vertices[0]) * 8, // "stride", offset between next position attribute (textures and normals are between)
									0);						 // offset of attribute from beginning of array
				glEnableVertexAttribArray(0);

				// texture u, v values
		 		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
				glEnableVertexAttribArray(1);

				// vertices normals
		 		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
				glEnableVertexAttribArray(2);

			// Unbind all bound objects (reset to default '0' name)
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

// Renders mesh to screen
void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glDrawElements(GL_TRIANGLES,			// type of primitive to render
				indexCount,							// number of elements to be rendered (per mesh) 
				GL_UNSIGNED_INT,					// value type in indices (following parameter)
				0);									// pointer to location of indices (start of array)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}


Mesh::~Mesh()
{
	ClearMesh();
}
