#ifndef _GLSLPROGRAM_H
#define _GLSLPROGRAM_H

#include <gl/glew.h>
#include <gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#include "auxiliar.h"
#include "Light.h"

class GLSLProgram
{
private:
	char *vname, *fname;

	unsigned int vshader;
	unsigned int fshader;
	unsigned int program;

	int inPos;
	int inColor;
	int inNormal;
	int inTexCoord;
	int inTangent;
	int inTexCoordU;

	int uModelViewMat;
	int uModelViewProjMat;
	int uNormalMat;
	int uProj;
	int uView;

	int uColorTex;
	int uEmiTex;
	int uSpecularTex;
	int uNormalTex;
	int uDisplacementMap;
	int uboxMapTex;
	int uDisplacement1D;

	//Plane scaling factor
	int uScalingFactor;

	//Water displacement texture time
	int uTime;

	//Uniform luz ambiental
	int uIntAmbiental;
	//Uniforms luz puntual
	int uPosPoint, uIntPoint, uNumPoint;
	//Uniforms luz spot
	int uPosSpot, uIntSpot, uNumSpot;
	//Uniform luz direccional
	int uPosDirec, uIntDirec, uNumDirec;

	//Num. total Lights
	int numPoint, numSpot, numDirec;

	std::vector<Light*> lights;
	glm::vec3 ambientLight;

	glm::vec3 *posPointLights;
	glm::vec3 *intPointLights;
	glm::vec3 *posSpotLights;
	glm::vec3 *intSpotLights;
	glm::vec3 *posDirLights;
	glm::vec3 *intDirLights;

private:
	GLuint LoadShader(const char *fileName, GLenum type);
	GLuint LoadShaderDisplacement(const char *fileName, GLenum type);
	
	void AddUnifNumLight(int idNum, int num);
	void AddUnifPosLight(int idUnif, glm::vec3 *vect, int sizeArray);
	void AddUnifIntLight(int idUnif, glm::vec3 *vect, int sizeArray);
	void AddUnifTexC(unsigned int color);
	void AddUnifTexE(unsigned int emi);
	void AddUnifTexS(unsigned int spec);
	void AddUnifTexN(unsigned int nor);
	void AddUnifTexD(unsigned int dis);
	void AddUnif1fAmbiental(glm::vec3 ambient);

public:
	GLSLProgram();
	void InitShader(const char *vname, const char *fname);
	void InitSkyShader(const char *vname, const char *fname);
	void Destroy();
	void UseProgram();
	void AddUnif(glm::mat4 &modelView, glm::mat4 &modelViewProj, glm::mat4 &normal);
	void AddUnifMat4fvMV(glm::mat4 &mat);
	void AddUnifMat4fvMVP(glm::mat4 &mat);
	void AddUnifMat4fvN(glm::mat4 &mat);
	void AddUnifMat4fvP(glm::mat4 &mat);
	void AddUnifMat4fvV(glm::mat4 &mat);
	void AddUnifTex(unsigned int color, unsigned int emi, unsigned int spec, unsigned int nor);
	void AddUnifDispTex(unsigned int displacement);
	void AddUnifLight();
	void AddUnifTexBoxMap(unsigned int boxMap);
	//PLANE Scaling Factor
	void AddUnifScalingFactor(float scaleFactor);
	//WATER
	void AddUnifTime(float time);
	//SMOOTH SURFACE
	void AddUnifDisp1D(float *vect, int sizeArray);

	int getPos();
	int getColor();
	int getNormal();
	int getTexCoord();
	int getTangent();
	int getTexCoordU();

	void AddLight(Light &light);
	inline void AddAmbientLight(glm::vec3 &ai){ ambientLight = ai; }

	void EnableBlending();
	void DisableBlending();
};




#endif