#pragma once

#include <glew.h>

#include <iostream>
#include <string>

class Texture {
private:
	unsigned int textureID;
	std::string texturePath;
	unsigned char* textureData;
	int width, height, nrChannels;
public:
	Texture();
	Texture(const char* texPath);

	bool LoadTexture();

	void CompileTexture();
	void UseTexture();

	~Texture();
};