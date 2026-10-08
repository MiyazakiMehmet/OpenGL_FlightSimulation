#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"
#include <string>
#include <vector>
#include <cstdio>

Texture::Texture()
{
	textureID = 0;
	texturePath = "";
	width = 0;
	height = 0;
	nrChannels = 0;
	textureData = nullptr;
}

Texture::Texture(const char* texPath) {
	textureID = 0;
	texturePath = texPath;
	width = 0;
	height = 0;
	nrChannels = 0;
	textureData = nullptr;
}

// Loads texture from file (returning bool for model loading instead of void) 
bool Texture::LoadTexture() {
	stbi_set_flip_vertically_on_load(true);
	// try several likely relative locations so running from the exe folder still finds the asset
	std::string base = texturePath;
	textureData = stbi_load(base.c_str(), &width, &height, &nrChannels, 0);

	if (!textureData) {
		std::cout << "Failed to find: " << (texturePath.empty() ? std::string("(null)") : texturePath) << std::endl;
		return false;
	}

	// leave uploading to CompileTexture
	return true;
}

void Texture::CompileTexture()
{
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (!textureData) {
		std::cout << "Failed to load texture data before compile" << std::endl;
		return;
	}

	GLenum format;
	if (nrChannels == 1) format = GL_RED;
	else if (nrChannels == 3) format = GL_RGB;
	else if (nrChannels == 4) format = GL_RGBA;
	else format = GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);

	std::cout << "Loaded texture: " << texturePath << " with " << nrChannels << " channels." << std::endl;

	stbi_image_free(textureData);
	textureData = nullptr;
	glBindTexture(GL_TEXTURE_2D, 0); //Unbind
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	nrChannels = 0;
	texturePath = "";
}