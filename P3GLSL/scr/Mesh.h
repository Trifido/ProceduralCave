#ifndef _MESH_H
#define _MESH_H

#include "BOX.h"
#include "GLSLProgram.h"
#include "Texture.h"
#include "Camera.h"
#include "Spline.h"
#include "SmoothSurface.h"

#include <vector>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h> 

enum TypeMesh
{
	DEFAULT_MESH = 0,
	IMPORT_MESH = 1,
	DISPLACEMENT_MESH = 2,
	DYNAMIC_MESH = 3,
	SKYBOX_MESH = 4,
	HOLE_MESH = 5,
	GODRAY_MESH = 6
};

enum TypeRender
{
	TYPE_POINTS = 1,
	TYPE_LINES = 2,
	TYPE_FILLS = 3
};

float Distance2DwY(Vector v1, Vector v2);

class Mesh
{
	private:

		//Tipo de Mesh
		TypeMesh typeMesh = DEFAULT_MESH;
		TypeRender typeRender = TYPE_FILLS;

		//VBOs que forman parte del objeto
		unsigned int posVBO;
		unsigned int colorVBO;
		unsigned int normalVBO;
		unsigned int texCoordVBO;
		unsigned int texCoordUVBO;
		unsigned int tangentVBO;
		unsigned int triangleIndexVBO;

		//Textures
		Texture colorTex;
		Texture colorTex2;
		Texture colorTex3;
		Texture emiTex;
		Texture specularTex;
		Texture normalTex;
		Texture displacementMap;
		Texture skyboxTex;
		GLuint *idText;

		//Model matrix
		glm::mat4 model;

		//ProgramShaders
		GLuint		vao;
		GLSLProgram	*programa;

		//Scaling Factor PLANE
		float		*scalingFactor;

		//Path Animation
		float	timelapse;
		
		//Flag curvePlane to update
		bool isCurvePlane = false;
		float density, offsetInside;
		bool invert;

		//Smooth Surface
		SmoothSurface *smSurface;

public:
		//Assimp Mesh
		float		*vertexArray;
		float		*normalArray;
		float		*uvArray;
		int			*uArray;
		float		*tangentArray;
		unsigned	*arrayIndex;
		int			numVerts;
		int			numFaces;
		int			numVertsU;

		Spline		*path;

	private:
		void LoadVBO(unsigned int &VBO, int dataSize, const float *vertexArray, GLint size, int idAtrib);
		void LoadVBO(unsigned int &VBO, int dataSize, const int *vertexArray, GLint size, int idAtrib);
		void LoadIBO(unsigned int &IBO, int dataSize, const unsigned int *indexArray);
		void Rotate(float &angle, glm::vec3 axis);
		void Translate(glm::vec3 cord);

	public:
		Mesh();	//Default Mesh (Cube)
		Mesh(bool option);
		Mesh(char *nameTex1);
		Mesh(char *nameTex1, char *nameTex2, bool imported = false);
		Mesh(char *nameTex1, char *nameTex2, char *nameTex3);
		Mesh(char *nameTex1, char *nameTex2, char *nameTex3, char *nameTex4);
		~Mesh();
		
		void Copy(Mesh source);
		void Destroy(GLSLProgram &programa);
		void Rotation(float angle, glm::vec3 axis);
		void Traslation(glm::vec3 displacement);
		void Scalation(glm::vec3 scale);
		void Orbit(float &internAngle, float &externAngle, glm::vec3 externRadius);
		void PathAnimation();
		void InitRender(Camera &camera);
		void Render();
		void SetTypeRender(TypeRender type);
		void InitDefaultMesh();
		void InitMesh(const std::string &pFile);
		void InitPlaneMesh(int numVertX, int numVertY, float disp, float scalingFactor, float density, float offsetInside, bool invert, bool holes = false, float tiling = 1.0f);
		void InitPlane(float width, float height, float disp, float scalingFactor, bool invert, float tiling = 1.0f);
		void InitPlaneGodRay(float width, float height, float disp, float scaleFactor, bool invert, float tiling = 1.0f);
		void InitSky();

		inline void AddShader(GLSLProgram &ps) { programa = &ps; }
		void AddDisplacementShader(GLSLProgram &ps);
		void AddDisplacementHoleShader(GLSLProgram &ps);

		inline unsigned int GetEmiteId() { return emiTex.GetId(); }
		inline unsigned int GetEmiteId2() { return colorTex2.GetId(); }
		inline unsigned int GetEmiteId3() { return colorTex3.GetId(); }
		inline unsigned int GetColorId() { return colorTex.GetId(); }
		inline unsigned int GetSpecularId() { return specularTex.GetId(); }
		inline unsigned int GetNormalId() { return normalTex.GetId(); }
		inline unsigned int GetDisplacementId() { return displacementMap.GetId(); }
		inline GLuint GetBoxMapId() { return *idText; }
		inline int GetTypeMesh() { return typeMesh; }

		void ImportMesh(const std::string &pFile);

		//Metodos para generar cueva procedural
		void GeneratePlane(int width, int height,  float tiling, float density = 1.0f, bool invert = false);
		void GenerateCurvedPlane(int width, int height, float tiling, float density = 1.0f, float offsetInside = 1, bool invert = false);
		void InvertPlane();
		void ModifyPlane(float value);
		void GenerateWater(char *nameTex1, char *nameTex2, char *nameTex3, float scale);
		void GenerateSky(char *filename1, char *filename2, char *filename3, char *filename4, char *filename5, char *filename6);
		void GenerateGodRay(char *nameTex1, char *nameTex2, float scale);

		inline void ChangeScalingFactor(float data) { *scalingFactor += data; }
		inline float GetScalingFactor() { return *scalingFactor; }

		bool isCurvedPlane() { return isCurvePlane; }
		void UpdateMesh();
		void UpdatePlaneMesh();
};

//float DistanceVector(Vector v1, Vector v2) {
//	return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2));
//}

#endif