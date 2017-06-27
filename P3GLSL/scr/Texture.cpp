#include "Texture.h"

Texture::Texture(char *filename)
{
	//LoadTexture(filename);
	name = filename;
	type = COMMON_TEXTURE;
}

void Texture::WaterTexture(char *filename)
{
	name = filename;
	type = DYNAMIC_TEXTURE;
}

Texture::Texture(char *filename1, char *filename2, char *filename3, char *filename4, char *filename5, char *filename6)
{
	arrayName.push_back(filename1);
	arrayName.push_back(filename2);
	arrayName.push_back(filename3);
	arrayName.push_back(filename4);
	arrayName.push_back(filename5);
	arrayName.push_back(filename6);
	type = CUBEMAP_TEXTURE;
}

void Texture::Load(unsigned char *map, int h, int w)
{
	mymap = map;
	myh = h;
	myw = w;
}

void Texture::LoadTexture()
{
	unsigned char *map;
	unsigned int w, h;

	glGenTextures(1, &this->id);

	if (type != CUBEMAP_TEXTURE)
	{
		//Primero se carga la textura desde fichero
		map = loadTexture(name, w, h);
		if (!map)
		{
			std::cout << "Error cargando el fichero: "
				<< name << std::endl;
			exit(-1);
		}

		glBindTexture(GL_TEXTURE_2D, this->id);
		//Tipo de textura a modificar, numero de midmap, formato de canal interno de la tarjeta gráfica, ancho, alto, borde, los canales de los datos que le voy a pasar,
		//tipo de canal, puntero a los datos.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)map);

		delete[] map;

		//Generar el Mipmap
		glGenerateMipmap(GL_TEXTURE_2D);

		//Indicar el acceso a la textura
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (type == COMMON_TEXTURE)
		{
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		}
		else if (type == DYNAMIC_TEXTURE)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		}
	}
	else
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);
		
		for (int i = 0; i < arrayName.size(); i++)
		{
			map = loadTexture(arrayName.at(i), w, h);
			if (!map)
			{
				std::cout << "Error cargando el fichero: "
					<< name << std::endl;
				exit(-1);
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, map);

			delete[] map;
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}

void Texture::LoadPerlinTexture()
{
	if (!mymap)
	{
		std::cout << "Error cargando el fichero: "
			<< name << std::endl;
		exit(-1);
	}

	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);
	//Tipo de textura a modificar, numero de midmap, formato de canal interno de la tarjeta grÃ¡fica, ancho, alto, borde, los canales de los datos que le voy a pasar,
	//tipo de canal, puntero a los datos.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, myw, myh, 0, GL_RED, GL_UNSIGNED_BYTE, (GLvoid*)mymap);

	//Generar el Mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	//Indicar el acceso a la textura
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
}

void Texture::Destroy()
{
	//Borramos las texturas
	glDeleteTextures(1, &id);
}

void Texture::create_cube_map( const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right, GLuint* tex_cube) {
	// generate a cube-map texture to hold all the sides
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, tex_cube);

	// load each image and copy into a side of the cube-map texture
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
	// format cube map texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

bool Texture::load_cube_map_side( GLuint texture, GLenum side_target, const char* file_name) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	unsigned int w, h;
	unsigned char*  image_data = loadTexture(file_name, w, h);
	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}

	// copy image data into 'target' side of cube map
	glTexImage2D( side_target, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	free(image_data);
	return true;
}