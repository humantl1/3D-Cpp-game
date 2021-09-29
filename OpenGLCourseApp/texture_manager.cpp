#include "texture_manager.h"
#include <SDL_image.h>

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = (char*)"";
}

Texture::Texture(const char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

bool Texture::LoadTexture()
{
	//unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	//if (!texData)
	//{
	//	printf("Failed to find: %s\n", fileLocation);
	//	return false;
	//}

	SDL_Surface* surface = IMG_Load(fileLocation); 
	if (!surface) {
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}

	width= surface->w;
	height= surface->h;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		// wrap top/bottom border
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);		// wrap top/bottom border
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// set mipmap minifying function to linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// set mipmap magnifying function to linear
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	//stbi_image_free(texData);
	SDL_FreeSurface(surface);

	return true; 
}

bool Texture::LoadTextureA()
{
	//unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	//if (!texData)
	//{
	//	printf("Failed to find: %s\n", fileLocation);
	//	return false;
	//}

	SDL_Surface* surface = IMG_Load(fileLocation); 
	if (!surface) {
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}

	width= surface->w;
	height= surface->h;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	//stbi_image_free(texData);
	SDL_FreeSurface(surface);

	return true; 
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE1); // set to one to avoid unused textures being assigned to 0, and thus to both texture 2D and cube maps
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = (char*)"";
}

Texture::~Texture()
{
	ClearTexture();
}
 