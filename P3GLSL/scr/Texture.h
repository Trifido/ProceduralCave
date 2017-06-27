#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "auxiliar.h"
#include <iostream>
#include <vector>
#include <gl/glew.h>

enum TypeTexture
{
	COMMON_TEXTURE = 0,
	DYNAMIC_TEXTURE = 1,
	CUBEMAP_TEXTURE = 2
};

class Texture
{
	private:
		unsigned int id;
		TypeTexture type;
		char *name;
		std::vector <char*> arrayName;
		unsigned char *mymap;
		int myh, myw;
	public:
		Texture(){};
		Texture(char *filename);
		Texture(char *filename1, char *filename2, char *filename3, char *filename4, char *filename5, char *filename6);

		void WaterTexture(char *filename);
		void LoadTexture();
		void create_cube_map(const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right, GLuint* tex_cube);
		bool load_cube_map_side(GLuint texture, GLenum side_target, const char* file_name);
		inline unsigned int GetId(){ return this->id; }
		void Destroy();

		void Texture::Load(unsigned char *map, int h, int w);
		void LoadPerlinTexture();

		inline ~Texture() { Destroy(); }
};

#endif