#pragma once
class TextureImage
{
public:
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width;
	unsigned int height;
	unsigned int imageSize;

	unsigned char* data;

	TextureImage(const char* imagePath);
	~TextureImage();
};

