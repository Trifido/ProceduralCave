#include "Mesh.h"

Mesh::Mesh()
{
	colorTex = Texture("../img/transparent.png"); //../img/color.png
	emiTex = Texture("../img/godRay1.jpg");//../img/emissive.png
	//specularTex = Texture("../img/specMap.png");
	//normalTex = Texture("../img/normal.png");

	path = new Spline();
	timelapse = 0.0f;
	scalingFactor = new float;
	*scalingFactor = 0.0f;
	typeMesh = HOLE_MESH;
}

Mesh::Mesh(bool option)
{
	path = new Spline();
	scalingFactor = new float;
	*scalingFactor = 0.0f;
	idText = new GLuint;

	GenerateSky("../img/skybox/right.jpg", "../img/skybox/left.jpg",
		"../img/skybox/top.jpg", "../img/skybox/bottom.jpg",
		"../img/skybox/back.jpg", "../img/skybox/front.jpg");
}

Mesh::Mesh(char *nameTex1)
{
	colorTex = Texture("../img/transparent.png");
	emiTex = Texture(nameTex1);

	path = new Spline();
	timelapse = 0.0f;
	scalingFactor = new float;
	*scalingFactor = 0.0f;
	typeMesh = GODRAY_MESH;
}

Mesh::Mesh(char *nameTex1, char *nameTex2, bool imported)
{
	if(imported)
		typeMesh = IMPORT_MESH;
	else
		typeMesh = DISPLACEMENT_MESH;

	colorTex = Texture(nameTex1);
	specularTex = Texture(nameTex2);
	/*emiTex = NULL;
	normalTex = NULL;*/
	path = new Spline();
	timelapse = 0.0f;
}

Mesh::Mesh(char *nameTex1, char *nameTex2, char *nameTex3)
{
	typeMesh = DISPLACEMENT_MESH;
	colorTex = Texture(nameTex1);
	colorTex2 = Texture("../img/colorTexture2.jpg");
	colorTex3 = Texture("../img/noiseEmisiveLight.jpg");
	specularTex = Texture(nameTex2);
	displacementMap = Texture(nameTex3);
	scalingFactor = new float;
	/*emiTex = NULL;
	normalTex = NULL;*/
	path = new Spline();
	timelapse = 0.0f;

	smSurface = new SmoothSurface();

	int numSteps = 500;

	for (int i = 0; i < numSteps; ++i) {
		float x = i / float(numSteps - 1) * 10;
		float result = smSurface->Calculate(x);
		//std::cout << "Noise at " << x << ": " << result << std::endl;
		smSurface->SetValue(result);
	}
}

Mesh::Mesh(char *nameTex1, char *nameTex2, char *nameTex3, char *nameTex4)
{
	colorTex = Texture(nameTex1);
	emiTex = Texture(nameTex2);
	specularTex = Texture(nameTex3);
	normalTex = Texture(nameTex4);

	path = new Spline();
	timelapse = 0.0f;
}

void Mesh::InitRender(Camera &camera)
{
	if (typeRender == SKYBOX_MESH)
	{
		glDepthMask(GL_FALSE);
	}

	(*programa).UseProgram();

	if (typeMesh == DYNAMIC_MESH || typeMesh == HOLE_MESH || typeMesh == GODRAY_MESH || typeMesh == EMISSIVE_MESH)
	{
		(*programa).EnableBlending();
	}
	
	if (typeMesh == SKYBOX_MESH)
	{
		camera.SetModelViewProj(camera.GetProj() * camera.GetView() * model);
		(*programa).AddUnifMat4fvP(camera.GetView());
		(*programa).AddUnifMat4fvMVP(camera.GetModelViewProj());
		(*programa).AddUnifMat4fvV(camera.GetProj());
		(*programa).AddUnifTexBoxMap(GetBoxMapId());
	}
	else
	{
		camera.SetModelView(camera.GetView() * model);
		camera.SetModelViewProj(camera.GetProj() * camera.GetView() * model);
		camera.SetNormal(glm::transpose(glm::inverse(camera.GetModelView())));

		(*programa).AddUnif(camera.GetModelView(), camera.GetModelViewProj(), camera.GetNormal());
		(*programa).AddUnifTex(GetColorId(), GetEmiteId(), GetSpecularId(), GetNormalId());
	}

	if (this->GetTypeMesh() == DISPLACEMENT_MESH || this->GetTypeMesh() == HOLE_MESH || this->GetTypeMesh() == CURVE_MESH)
	{
		(*programa).AddUnifPosCamera(camera.GetPos());
		(*programa).AddUnifDispTex(GetDisplacementId());
		(*programa).AddUnifScalingFactor(*scalingFactor);
		(*programa).AddUnifDisp1D(smSurface->GetValues(), smSurface->GetTotalValues());
		(*programa).AddUnifTexEmissive(GetEmiteId2(), GetEmiteId3());

		if (this->GetTypeMesh() == CURVE_MESH)
		{
			(*programa).AddUnifGradColor(colorVertex, numVerts);
		}
	}
	else if (this->GetTypeMesh() == DYNAMIC_MESH || this->GetTypeMesh() == GODRAY_MESH)
	{
		(*programa).AddUnifDispTex(GetDisplacementId());
		(*programa).AddUnifScalingFactor(*scalingFactor);
		(*programa).AddUnifTime(timelapse);
	}

	if (typeMesh != SKYBOX_MESH)
		(*programa).AddUnifLight();
}

void Mesh::Render()
{
	//Activamos el VAO del objeto, activandose todos los VBO 
	glBindVertexArray(vao);

	if (typeRender == TYPE_FILLS)
	{
		if (typeMesh == SKYBOX_MESH)
		{
			glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, (void*)0);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
			glDepthMask(GL_TRUE);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, (void*)0);
		}
	}
	else if (typeRender == TYPE_POINTS)
	{
		if (typeMesh == SKYBOX_MESH)
			glDrawArrays(GL_POINTS, 0, 36);
		else
			glDrawElements(GL_POINTS, numFaces * 3, GL_UNSIGNED_INT, (void*)0);
	}
	else
	{
		if (typeMesh == SKYBOX_MESH)
			glDrawArrays(GL_LINES, 0, 36);
		else
			glDrawElements(GL_LINES, numFaces * 3, GL_UNSIGNED_INT, (void*)0);
	}

	if (typeMesh == DYNAMIC_MESH || typeMesh == HOLE_MESH || typeMesh == GODRAY_MESH || typeMesh == EMISSIVE_MESH)
	{
		(*programa).DisableBlending();
	}
	
	glBindVertexArray(0);
		
	glUseProgram(NULL);
}

void Mesh::SetTypeRender(TypeRender type)
{
	this->typeRender = type;
}

void Mesh::LoadVBO(unsigned int &VBO, int dataSize, const float *vertexArray, GLint size, int idAtrib)
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//printf("DataSize: %d\n", dataSize);
	glBufferData(GL_ARRAY_BUFFER, dataSize, &vertexArray[0], GL_STATIC_DRAW);
	glVertexAttribPointer(idAtrib, size, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(idAtrib);
}

void Mesh::LoadVBO(unsigned int &VBO, int dataSize, const int *vertexArray, GLint size, int idAtrib)
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//printf("DataSize: %d\n", dataSize);
	glBufferData(GL_ARRAY_BUFFER, dataSize, &vertexArray[0], GL_STATIC_DRAW);
	glVertexAttribPointer(idAtrib, size, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(idAtrib);
}

void Mesh::LoadIBO(unsigned int &IBO, int dataSize, const unsigned int *indexArray)
{
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, &indexArray[0], GL_STATIC_DRAW);
}

void Mesh::Copy(Mesh source)
{
	this->arrayIndex = source.arrayIndex;
	this->vertexArray = source.vertexArray;
	this->uvArray = source.uvArray;
	this->normalArray = source.normalArray;
	this->tangentArray = source.tangentArray;
}

void Mesh::Rotate(float &angle, glm::vec3 axis)
{
	model *= glm::rotate(glm::mat4(1.0f), angle, axis);
}

void Mesh::Translate(glm::vec3 cord)
{
	model *= glm::translate(glm::mat4(1.0f), cord);
}

void Mesh::Rotation(float angle, glm::vec3 axis) {
	model *= glm::rotate(glm::mat4(1.0f), angle, axis);
}

void Mesh::Traslation(glm::vec3 displacement)
{
	model *= glm::translate(glm::mat4(1.0f), displacement);
}

void Mesh::Scalation(glm::vec3 scal)
{
	model *= glm::scale(glm::mat4(1.0f), scal);
}

void Mesh::Orbit(float &internAngle, float &externAngle, glm::vec3 externRadius) {
	Rotation(internAngle, glm::vec3(0, 1.0f, 0));
	Translate(externRadius);
	Rotate(externAngle, glm::vec3(0, 1.0f, 0));
}

void Mesh::PathAnimation()
{
	if (typeMesh == DEFAULT_MESH || typeMesh == IMPORT_MESH)
	{
		Vector coord = path->GetCoord((int)timelapse);
		model *= glm::translate(glm::mat4(1.0f), glm::vec3(coord.x, coord.y, coord.z));
		timelapse += 0.01f;
	}
	else if (typeMesh == DYNAMIC_MESH)
		timelapse += 0.0001f;
	else if (typeMesh == GODRAY_MESH)
		timelapse += 0.0001f;
}

void Mesh::Destroy(GLSLProgram &programa) 
{
	//Borramos los distintos buffers anteriormente creados
	if (programa.getPos() != -1) glDeleteBuffers(1, &posVBO);
	if (programa.getColor() != -1) glDeleteBuffers(1, &colorVBO);
	if (programa.getNormal() != -1) glDeleteBuffers(1, &normalVBO);
	if (programa.getTexCoord() != -1) glDeleteBuffers(1, &texCoordVBO);
	if (programa.getTangent() != -1) glDeleteBuffers(1, &tangentVBO);
	if (programa.getTexCoordU() != -1) glDeleteBuffers(1, &texCoordUVBO);
	if (programa.getTexCoordV() != -1) glDeleteBuffers(1, &texCoordVVBO);

	colorTex.Destroy();
	colorTex2.Destroy();
	colorTex3.Destroy();
	emiTex.Destroy();
	specularTex.Destroy();
	normalTex.Destroy();
	displacementMap.Destroy();
	skyboxTex.Destroy();

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &posVBO);
	glDeleteBuffers(1, &colorVBO);
	glDeleteBuffers(1, &normalVBO);
	glDeleteBuffers(1, &texCoordVBO);
	glDeleteBuffers(1, &tangentVBO);
	glDeleteBuffers(1, &texCoordUVBO);
	glDeleteBuffers(1, &texCoordVVBO);

		//Destroy(programa);

	glDeleteBuffers(1, &triangleIndexVBO);
}

void Mesh::InitDefaultMesh() {
	//Creo el VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if ((*programa).getPos() != -1)
	{
		LoadVBO(posVBO, cubeNVertex * sizeof(float) * 3, cubeVertexRayPos, 3, (*programa).getPos());
	}
	if ((*programa).getColor() != -1)
	{
		LoadVBO(colorVBO, cubeNVertex * sizeof(float) * 3, cubeVertexColor, 3, (*programa).getColor());
	}
	if ((*programa).getNormal() != -1)
	{
		LoadVBO(normalVBO, cubeNVertex * sizeof(float) * 3, cubeVertexNormal, 3, (*programa).getNormal());
	}
	if ((*programa).getTexCoord() != -1)
	{
		LoadVBO(texCoordVBO, cubeNVertex * sizeof(float) * 2, cubeVertexRayTexCoord, 2, (*programa).getTexCoord());
	}
	if ((*programa).getTangent() != -1)
	{
		LoadVBO(tangentVBO, cubeNVertex * sizeof(float) * 3, cubeVertexTangent, 3, (*programa).getTangent());
	}

	LoadIBO(triangleIndexVBO, cubeNTriangleIndex * sizeof(unsigned int) * 3, cubeTriangleIndex);

	colorTex.LoadTexture();
	emiTex.LoadTexture();
	//specularTex.LoadTexture();
	//normalTex.LoadTexture();

	numFaces = cubeNTriangleIndex;

	model = glm::mat4(1.0f);
}

void Mesh::InitMesh(const std::string &pFile) {
	ImportMesh(pFile);
	//Creo el VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if ((*programa).getPos() != -1)
	{
		LoadVBO(posVBO, numVerts * sizeof(float) * 3, vertexArray, 3, (*programa).getPos());
	}
	if ((*programa).getColor() != -1)
	{
		LoadVBO(colorVBO, cubeNVertex * sizeof(float) * 3, cubeVertexColor, 3, (*programa).getColor());
	}
	if ((*programa).getNormal() != -1)
	{
		LoadVBO(normalVBO, numVerts * sizeof(float) * 3, normalArray, 3, (*programa).getNormal());
	}
	if ((*programa).getTexCoord() != -1)
	{
		LoadVBO(texCoordVBO, numVerts * sizeof(float) * 2, uvArray, 2, (*programa).getTexCoord());
	}
	if ((*programa).getTangent() != -1)
	{
		LoadVBO(tangentVBO, numVerts * sizeof(float) * 3, tangentArray, 3, (*programa).getTangent());
	}

	LoadIBO(triangleIndexVBO, numFaces * sizeof(unsigned int) * 3, arrayIndex);

	colorTex.LoadTexture();
	specularTex.LoadTexture();

	model = glm::mat4(1.0f);
}

void Mesh::InitPlaneMesh(int numVertX, int numVertY, float disp, float scaleFactor, float density, float offsetInside, bool invert, bool holes, float tiling)
{
	typeMesh = CURVE_MESH;

	GenerateCurvedPlane(500, 500, tiling, density, offsetInside, invert);

	CreateColorVertex(500, 500);

	//*scalingFactor = scaleFactor;

	*scalingFactor = scaleFactor * 17.0f;

	//Creo el VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if ((*programa).getPos() != -1)
	{
		LoadVBO(posVBO, numVerts * sizeof(float) * 3, vertexArray, 3, (*programa).getPos());
	}
	if ((*programa).getColor() != -1)
	{
		LoadVBO(colorVBO, cubeNVertex * sizeof(float) * 3, cubeVertexColor, 3, (*programa).getColor());
	}
	if ((*programa).getNormal() != -1)
	{
		LoadVBO(normalVBO, numVerts * sizeof(float) * 3, normalArray, 3, (*programa).getNormal());
	}
	if ((*programa).getTexCoord() != -1)
	{
		LoadVBO(texCoordVBO, numVerts * sizeof(float) * 2, uvArray, 2, (*programa).getTexCoord());
	}
	if ((*programa).getTangent() != -1)
	{
		LoadVBO(tangentVBO, numVerts * sizeof(float) * 3, tangentArray, 3, (*programa).getTangent());
	}
	if ((*programa).getTexCoordU() != -1)
	{
		LoadVBO(texCoordUVBO, numVerts * sizeof(int), uArray, 1, (*programa).getTexCoordU());
	}
	if ((*programa).getTexCoordV() != -1)
	{
		LoadVBO(texCoordVVBO, numVerts * sizeof(int), vArray, 1, (*programa).getTexCoordV());
	}

	LoadIBO(triangleIndexVBO, numFaces * sizeof(unsigned int) * 3, arrayIndex);

	colorTex3.LoadTexture();
	colorTex.LoadTexture();
	specularTex.LoadTexture();
	displacementMap.LoadTexture();

	model = glm::mat4(1.0f);
}

void Mesh::InitPlane(float width, float height, float disp, float scaleFactor, bool invert, float tiling)
{
	GeneratePlane((int)width, (int)height, tiling, disp, invert);

	if(typeMesh == DYNAMIC_MESH)
		*scalingFactor = scaleFactor * 13.0f;
	else
		*scalingFactor = scaleFactor * 16.0f;
	//(scaleFactor + 0.01f) * 4.0f;

	//Creo el VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if ((*programa).getPos() != -1)
	{
		LoadVBO(posVBO, numVerts * sizeof(float) * 3, vertexArray, 3, (*programa).getPos());
	}
	if ((*programa).getColor() != -1)
	{
		LoadVBO(colorVBO, cubeNVertex * sizeof(float) * 3, cubeVertexColor, 3, (*programa).getColor());
	}
	if ((*programa).getNormal() != -1)
	{
		LoadVBO(normalVBO, numVerts * sizeof(float) * 3, normalArray, 3, (*programa).getNormal());
	}
	if ((*programa).getTexCoord() != -1)
	{
		LoadVBO(texCoordVBO, numVerts * sizeof(float) * 2, uvArray, 2, (*programa).getTexCoord());
	}
	if ((*programa).getTangent() != -1)
	{
		LoadVBO(tangentVBO, numVerts * sizeof(float) * 3, tangentArray, 3, (*programa).getTangent());
	}	
	if ((*programa).getTexCoordU() != -1)
	{
		LoadVBO(texCoordUVBO, numVerts * sizeof(int), uArray, 1, (*programa).getTexCoordU());
	}

	LoadIBO(triangleIndexVBO, numFaces * sizeof(unsigned int) * 3, arrayIndex);

	colorTex.LoadTexture();

	if (this->typeMesh == DISPLACEMENT_MESH)
	{
		colorTex2.LoadTexture();
		colorTex3.LoadTexture();
	}

	specularTex.LoadTexture();
	displacementMap.LoadTexture();

	model = glm::mat4(1.0f);
}

void Mesh::InitPlaneGodRay(float width, float height, float disp, float scaleFactor, bool invert, float tiling)
{
	GeneratePlane((int)width, (int)height, tiling, disp, invert);

	*scalingFactor = scaleFactor;

	//Creo el VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if ((*programa).getPos() != -1)
	{
		LoadVBO(posVBO, numVerts * sizeof(float) * 3, vertexArray, 3, (*programa).getPos());
	}
	if ((*programa).getNormal() != -1)
	{
		LoadVBO(normalVBO, numVerts * sizeof(float) * 3, normalArray, 3, (*programa).getNormal());
	}
	if ((*programa).getTexCoord() != -1)
	{
		LoadVBO(texCoordVBO, numVerts * sizeof(float) * 2, uvArray, 2, (*programa).getTexCoord());
	}
	if ((*programa).getTangent() != -1)
	{
		LoadVBO(tangentVBO, numVerts * sizeof(float) * 3, tangentArray, 3, (*programa).getTangent());
	}

	LoadIBO(triangleIndexVBO, numFaces * sizeof(unsigned int) * 3, arrayIndex);

	colorTex.LoadTexture();
	specularTex.LoadTexture();
	emiTex.LoadTexture();
	displacementMap.LoadTexture();

	model = glm::mat4(1.0f);
}

void Mesh::InitSky()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	if ((*programa).getPos() != -1)
	{
		LoadVBO(posVBO, cubeNVertex * sizeof(float) * 3, cubeVertexPos, 3, (*programa).getPos());
	}

	//if ((*programa).getTexCoord() != -1)
	//{
	//	LoadVBO(texCoordVBO, cubeNVertex * sizeof(float) * 2, cubeVertexTexCoord, 2, (*programa).getTexCoord());
	//}

	LoadIBO(triangleIndexVBO, cubeNTriangleIndex * sizeof(unsigned int) * 3, cubeTriangleInvertIndex);

	skyboxTex.create_cube_map("../img/skybox/right.jpg", "../img/skybox/left.jpg",
		"../img/skybox/top.jpg", "../img/skybox/bottom.jpg",
		"../img/skybox/back.jpg", "../img/skybox/front.jpg", idText);

	numFaces = cubeNTriangleIndex;
	model = glm::mat4(1.0f);
}

void Mesh::ImportMesh(const std::string &pFile) {
	Assimp::Importer importer;

	importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
		aiComponent_CAMERAS | aiComponent_LIGHTS | aiComponent_COLORS |
		aiComponent_BONEWEIGHTS | aiComponent_ANIMATIONS | aiComponent_TANGENTS_AND_BITANGENTS);
	
	const aiScene *scene = importer.ReadFile(pFile, aiProcess_GenSmoothNormals | aiProcess_RemoveComponent | aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	aiMesh *mesh = scene->mMeshes[0];

	numVerts = mesh->mNumFaces * 3;

	vertexArray = new float[mesh->mNumFaces * 3 * 3];
	normalArray = new float[mesh->mNumFaces * 3 * 3];
	uvArray = new float[mesh->mNumFaces * 3 * 2];
	arrayIndex = new unsigned[mesh->mNumFaces * 3];
	tangentArray = new float[mesh->mNumFaces * 3 * 3];

	int cont = 0;
	numFaces = 0;

	for (unsigned int i = 0; i<mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
		numFaces++;
		for (int j = 0; j < 3; j++)
		{
			//if (mesh->mTextureCoords != NULL)
			//{
				aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
				memcpy(uvArray, &uv, sizeof(float) * 2);
				uvArray += 2;
			//}

			//if (mesh->mNormals != NULL) 
			//{
				aiVector3D normal = mesh->mNormals[face.mIndices[j]];
				memcpy(normalArray, &normal, sizeof(float) * 3);
				normalArray += 3;
			//}

			//if (mesh->mTangents != NULL)
			//{
				aiVector3D tangent = mesh->mTangents[face.mIndices[j]];
				memcpy(tangentArray, &tangent, sizeof(float) * 3);
				tangentArray += 3;
			//}
			
	/*		if (mesh->mNumVertices != NULL)
			{*/
				aiVector3D pos = mesh->mVertices[face.mIndices[j]];
				memcpy(vertexArray, &pos, sizeof(float) * 3);
				vertexArray += 3;
			//}

			arrayIndex[cont] = face.mIndices[j];
			cont++;
		}
	}

	uvArray -= mesh->mNumFaces * 3 * 2;
	normalArray -= mesh->mNumFaces * 3 * 3;
	vertexArray -= mesh->mNumFaces * 3 * 3;
	tangentArray -= mesh->mNumFaces * 3 * 3;


}

void Mesh::AddDisplacementShader(GLSLProgram &ps) {
	this->programa = &ps;
	this->typeMesh = DISPLACEMENT_MESH;
}

void Mesh::AddDisplacementHoleShader(GLSLProgram &ps) {
	this->programa = &ps;
	this->typeMesh = HOLE_MESH;
}

void Mesh::GeneratePlane(int width, int height, float tiling, float density, bool invert)
{
	vertexArray = new float[width * height * 3];
	numVerts = width * height;
	numVertsU = width;

	int tamW = width;
	int tamH = height;

	if (width % 2 == 0)
		tamW--;
	if (height % 2 == 0)
		tamH--;

	arrayIndex = new unsigned int[tamW * tamH * 2 * 3];

	int ind = 0;
	for (int w = -width/2; w < width/2; w++)
	{
		for (int h = -height/2; h < height/2; h++, ind += 3)
		{
			vertexArray[ind] = float(w) * density;
			if(invert)
				vertexArray[ind+1] = 50.0f;
			else
				vertexArray[ind + 1] = 0.0f;
			vertexArray[ind+2] = float(h) * density;
		}
	}

	//INDICES
	ind = 0;
	for (int w = 0; w < tamW; w++)
	{
		for (int h = 0; h < tamH; h++, ind +=6)
		{
			if (!invert)
			{
				//First Triangle
				arrayIndex[ind] = h + height * w;
				arrayIndex[ind + 1] = h + height * w + 1;
				arrayIndex[ind + 2] = h + height * w + height;

				//Second Triangle
				arrayIndex[ind + 3] = h + height * w + 1;
				arrayIndex[ind + 4] = h + 1 + height * w + height;
				arrayIndex[ind + 5] = h + height * w + height;
			}
			else
			{
				//First Triangle
				arrayIndex[ind] = h + height * w;
				arrayIndex[ind + 2] = h + height * w + 1;
				arrayIndex[ind + 1] = h + height * w + height;

				//Second Triangle
				arrayIndex[ind + 3] = h + height * w + 1;
				arrayIndex[ind + 5] = h + 1 + height * w + height;
				arrayIndex[ind + 4] = h + height * w + height;
			}
		}
	}

	numFaces = ind / 3;
	
	//Coordinates Normales
	uvArray = new float[width * height * 2];
	uArray = new int[width * height];
	normalArray = new float[width * height * 3];

	for (int i = 0; i < width * height; i++) {
		normalArray[i * 3] = 0.0f;
		normalArray[i * 3 + 2] = 0.0f;

		if (invert)
			normalArray[i * 3 + 1] = -1.0f;
		else
			normalArray[i * 3 + 1] = 1.0f;
	}

	//Coordinates UV
	float u = tiling / (float)width;
	float v = tiling / (float)height;
	float cU, cV;

	ind = 0;
	cU = cV = 0.0f;

	for (int h = 0; h < height; h++)
		for (int w = 0; w < width; w++)
			uArray[h * (int)width + w] = 0.0f;

	for (int w = 0; w < width; w++)
	{
		for (int h = 0; h < height; h++, ind += 2)
		{
			uvArray[ind] = cU;
			uvArray[ind+1] = cV;
			cV += v;
		}
		cU += u;
		cV = 0.0f;
	}
}

void Mesh::GenerateCurvedPlane(int width, int height, float tiling, float density, float offsetInside, bool invert)
{
	isCurvePlane = true;
	this->density = density;
	this->offsetInside = offsetInside;
	this->invert = invert;
	vertexArray = new float[width * height * 3];
	normalArray = new float[width * height * 3];
	numVerts = width * height;
	numVertsU = width;

	int tamW = width;
	int tamH = height;

	if (width % 2 == 0)
		tamW--;
	if (height % 2 == 0)
		tamH--;

	arrayIndex = new unsigned int[tamW * tamH * 2 * 3];

	//----------------------------------------------------------------------------------------------
	path->ScaleZPoints(offsetInside);
	path->ScaleXPoints(offsetInside);

	int numCurves = path->GetTotalCurves();

	int changeCurve = width / numCurves;
	float muestra = 1.0f / changeCurve;
	float m = 0.0f;

	int indCurve = 0;
	int ind = 0;
	for (int w = 0; w < width; w++)
	{
		if (m >= 1.0f) {
			m = 0.0f;
			indCurve++;
		}

		Vector coord = path->CalculateNewCoords(m, indCurve);
		Vector initialCoord = path->CalculateInitialCoord(m, indCurve);
		Vector normal = Vector(coord.x - initialCoord.x, 0.0f, coord.z - initialCoord.z);
		Vector invnormal = Vector(initialCoord.x - coord.x, 0.0f, initialCoord.z - coord.z);
		normal.Normalize();
		invnormal.Normalize();

		for (int h = 0; h < height; h++, ind += 3)
		{
			vertexArray[ind] = coord.x; // (w)* density;
			vertexArray[ind + 1] = float(h) * density; //float(h) * density; 
			vertexArray[ind + 2] =  coord.z;

			if (!invert)
			{
				normalArray[ind] = invnormal.x;
				normalArray[ind + 2] = invnormal.z;
			}
			else
			{
				normalArray[ind] = normal.x;
				normalArray[ind + 2] = normal.z;
			}
			
			normalArray[ind + 1] = 0.0f;
		}

		m += muestra;
	}

	int offInd = 0;
	ind -= height*3;
	for (int h = 0, offInd = 0; h < height; h++, ind += 3, offInd += 3)
	{
		vertexArray[ind] = vertexArray[h * 3];
		vertexArray[ind + 1] = vertexArray[h * 3 + 1];
		vertexArray[ind + 2] = vertexArray[h * 3 + 2];

		normalArray[ind] = normalArray[h * 3];
		normalArray[ind + 1] = normalArray[h * 3 + 1];
		normalArray[ind + 2] = normalArray[h * 3 + 2];
	}


	//----------------------------------------------------------------------------------------------
	//INDICES
	ind = 0;
	for (int w = 0; w < tamW; w++)
	{
		for (int h = 0; h < tamH; h++, ind += 6)
		{
			if (invert)
			{
				//First Triangle
				arrayIndex[ind] = h + height * w;
				arrayIndex[ind + 1] = h + height * w + 1;
				arrayIndex[ind + 2] = h + height * w + height;

				//Second Triangle
				arrayIndex[ind + 3] = h + height * w + 1;
				arrayIndex[ind + 4] = h + 1 + height * w + height;
				arrayIndex[ind + 5] = h + height * w + height;
			}
			else
			{
				//First Triangle
				arrayIndex[ind] = h + height * w;
				arrayIndex[ind + 2] = h + height * w + 1;
				arrayIndex[ind + 1] = h + height * w + height;

				//Second Triangle
				arrayIndex[ind + 3] = h + height * w + 1;
				arrayIndex[ind + 5] = h + 1 + height * w + height;
				arrayIndex[ind + 4] = h + height * w + height;
			}
		}
	}

	numFaces = ind / 3;

	//Coordinates Normales
	uvArray = new float[width * height * 2];
	uArray = new int[width * height];
	vArray = new int[width * height];

	//Coordinates UV
	float u = tiling / (float)width;
	float v = tiling / (float)height;
	float cU, cV;

	ind = 0;
	cU = cV = 0.0f;

	for (int h = 0; h < height; h++)
		for (int w = 0; w < width; w++) {
			uArray[h * (int)width + w] = h;
			vArray[h * (int)width + w] = w;
		}

	for (int w = 0; w < width; w++)
	{
		for (int h = 0; h < height; h++, ind += 2)
		{
			uvArray[ind] = cU;
			uvArray[ind + 1] = cV;
			cV += v;
		}
		cU += u;
		cV = 0.0f;
	}
}

void Mesh::InvertPlane()
{
	for (int i = 0; i < numVerts * 3; i += 3)
	{
		normalArray[i] = -normalArray[i];
	}
}

void Mesh::ModifyPlane(float value)
{
	for (int i = 0; i < numVerts * 3; i += 3)
	{
		vertexArray[i] = value * vertexArray[i] * vertexArray[i];
	}
}

void Mesh::GenerateWater(char *nameTex1, char *nameTex2, char *nameTex3, float scale)
{
	typeMesh = DYNAMIC_MESH;
	colorTex = Texture(nameTex1);
	specularTex = Texture(nameTex2);
	displacementMap = Texture();
	displacementMap.WaterTexture(nameTex3);
	scalingFactor = new float;

	*scalingFactor = scale;

	timelapse = 0.0f;
}

void Mesh::GenerateGodRay(char *nameTex1, char *nameTex2, float scale)
{
	typeMesh = GODRAY_MESH;
	colorTex = Texture(nameTex1);
	specularTex = Texture(nameTex1);
	emiTex = Texture(nameTex1);
	displacementMap = Texture();
	displacementMap.WaterTexture(nameTex2);
	scalingFactor = new float;

	*scalingFactor = 0.0f;

	timelapse = 0.0f;
}

void Mesh::GeneratePlant(char *nameTex1, char *nameTex2, float scale)
{
	typeMesh = EMISSIVE_MESH;
	colorTex = Texture(nameTex1);
	specularTex = Texture(nameTex1);
	emiTex = Texture(nameTex2);
	//displacementMap = Texture();
	//displacementMap.WaterTexture(nameTex2);
	scalingFactor = new float;

	*scalingFactor = 0.0f;

	timelapse = 0.0f;
}

void Mesh::GenerateSky(char *filename1, char *filename2, char *filename3, char *filename4, char *filename5, char *filename6)
{
	typeMesh = SKYBOX_MESH;
	skyboxTex = Texture(filename1, filename2, filename3, filename4, filename5, filename6);
	timelapse = 0.0f;
}

float Distance2DwY(Vector v1, Vector v2) {
	return sqrt(pow(v2.x - v1.x, 2) + pow(v2.z - v1.z, 2));
}

void Mesh::UpdateMesh()
{
	if (isCurvePlane)
	{
		int tamW = numVertsU;
		int tamH = numVerts / numVertsU;

		if (tamW % 2 == 0)
			tamW--;
		if (tamH % 2 == 0)
			tamH--;

		//arrayIndex = new unsigned int[tamW * tamH * 2 * 3];

		//----------------------------------------------------------------------------------------------

		int numCurves = path->GetTotalCurves();

		int changeCurve = numVertsU / numCurves;
		float muestra = 1.0f / changeCurve;
		float m = 0.0f;

		int indCurve = 0;
		int ind = 0;
		for (int w = 0; w < numVertsU; w++)
		{
			if (m >= 1.0f) {
				m = 0.0f;
				indCurve++;
			}

			Vector coord = path->CalculateNewCoords(m, indCurve);
			Vector initialCoord = path->CalculateInitialCoord(m, indCurve);
			Vector normal = Vector(coord.x - initialCoord.x, 0.0f, coord.z - initialCoord.z);
			Vector invnormal = Vector(initialCoord.x - coord.x, 0.0f, initialCoord.z - coord.z);
			normal.Normalize();
			invnormal.Normalize();

			for (int h = 0; h < tamH+1 ; h++, ind += 3)
			{
				vertexArray[ind] = coord.x; // (w)* density;
				vertexArray[ind + 1] = float(h) * density; //float(h) * density; 
				vertexArray[ind + 2] = coord.z;

				if (!invert)
				{
					normalArray[ind] = invnormal.x;
					normalArray[ind + 2] = invnormal.z;
				}
				else
				{
					normalArray[ind] = normal.x;
					normalArray[ind + 2] = normal.z;
				}

				normalArray[ind + 1] = 0.0f;
			}

			m += muestra;
		}

		int offInd = 0;
		ind -= (tamH+1) * 3;
		for (int h = 0, offInd = 0; h < tamH+1; h++, ind += 3, offInd += 3)
		{
			vertexArray[ind] = vertexArray[h * 3];
			vertexArray[ind + 1] = vertexArray[h * 3 + 1];
			vertexArray[ind + 2] = vertexArray[h * 3 + 2];

			normalArray[ind] = normalArray[h * 3];
			normalArray[ind + 1] = normalArray[h * 3 + 1];
			normalArray[ind + 2] = normalArray[h * 3 + 2];
		}
	}
}

void Mesh::UpdatePlaneMesh()
{
	path->UpdateSpline();
	UpdateMesh();

	//Creo el VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if ((*programa).getPos() != -1)
	{
		LoadVBO(posVBO, numVerts * sizeof(float) * 3, vertexArray, 3, (*programa).getPos());
	}
	if ((*programa).getColor() != -1)
	{
		LoadVBO(colorVBO, cubeNVertex * sizeof(float) * 3, cubeVertexColor, 3, (*programa).getColor());
	}
	if ((*programa).getNormal() != -1)
	{
		LoadVBO(normalVBO, numVerts * sizeof(float) * 3, normalArray, 3, (*programa).getNormal());
	}
	if ((*programa).getTexCoord() != -1)
	{
		LoadVBO(texCoordVBO, numVerts * sizeof(float) * 2, uvArray, 2, (*programa).getTexCoord());
	}
	if ((*programa).getTangent() != -1)
	{
		LoadVBO(tangentVBO, numVerts * sizeof(float) * 3, tangentArray, 3, (*programa).getTangent());
	}
	if ((*programa).getTexCoordU() != -1)
	{
		LoadVBO(texCoordUVBO, numVerts * sizeof(int), uArray, 1, (*programa).getTexCoordU());
	}

	LoadIBO(triangleIndexVBO, numFaces * sizeof(unsigned int) * 3, arrayIndex);

	colorTex.LoadTexture();
	specularTex.LoadTexture();
	displacementMap.LoadTexture();

	model = glm::mat4(1.0f);
}

void Mesh::GenerateProceduralPlants(int numEdgeVertex)
{
	const float PI2 = 6.28318530718;
	int width = 10;
	int height = numEdgeVertex;

	numVerts = width * height;
	numVertsU = width;
	vertexArray = new float[numVerts * 3];

	int tamW = width;
	int tamH = height;

	if (width % 2 == 0)
		tamW--;
	if (height % 2 == 0)
		tamH--;

	arrayIndex = new unsigned int[tamW * tamH * 2 * 3];

	//Generamos los vertices del perfil
	for (int i = 0; i < numEdgeVertex * 3; i+=3)
	{
		vertexArray[i] = ((float)rand() / (RAND_MAX)) * 10;
		vertexArray[i + 1] = (float)i * 10;
		vertexArray[i + 2] = 0.0f;
	}

	int cont = numEdgeVertex * 3;
	float ang = PI2 / 10;

	for (float angulo = 0; angulo < PI2; angulo += ang)
	{
		for (int i = 0; i < numEdgeVertex * 3; i+=3)
		{
			float xValue = cos(angulo) * vertexArray[i] + sin(angulo) * vertexArray[i+2];
			float yValue = vertexArray[i+1];
			float zValue = -sin(angulo) * vertexArray[i] + cos(angulo) * vertexArray[i+2];

			vertexArray[cont] = xValue;
			vertexArray[cont+1] = yValue;
			vertexArray[cont+2] = zValue;
			cont += 3;
		}
	}

	for (int i = 0; i < numVerts*3; i+=3)
		printf("i: %i -> (%f, %f,%f)\n", i, vertexArray[i], vertexArray[i + 1], vertexArray[i + 2]);


	//INDICES
	int ind = 0;
	for (int w = 0; w < tamW; w++)
	{
		for (int h = 0; h < tamH; h++, ind += 6)
		{
			//First Triangle
			arrayIndex[ind] = h + height * w;
			arrayIndex[ind + 1] = h + height * w + 1;
			arrayIndex[ind + 2] = h + height * w + height;

			//Second Triangle
			arrayIndex[ind + 3] = h + height * w + 1;
			arrayIndex[ind + 4] = h + 1 + height * w + height;
			arrayIndex[ind + 5] = h + height * w + height;
		}
	}	

	//Coordinates Normales
	uvArray = new float[width * height * 2];
	normalArray = new float[width * height * 3];

	for (int i = 0; i < width * height; i++) {

		normalArray[i * 3] = 0.0f;
		normalArray[i * 3 + 2] = 0.0f;
		normalArray[i * 3 + 1] = 1.0f;
	}

	//Coordinates UV
	float u = (float)width;
	float v = (float)height;
	float cU, cV;

	ind = 0;
	cU = cV = 0.0f;

	for (int w = 0; w < width; w++)
	{
		for (int h = 0; h < height; h++, ind += 2)
		{
			uvArray[ind] = cU;
			uvArray[ind + 1] = cV;
			cV += v;
		}
		cU += u;
		cV = 0.0f;
	}
}

void Mesh::CreateProceduralPlant()
{
	//typeMesh = EMISSIVE_MESH;

	//colorTex = Texture("../img/colorTexture3.png"); //../img/color.png
	////emiTex = Texture("../img/godRay1.jpg");//../img/emissive.png
	//specularTex = Texture("../img/specMap.png");

	//this->GeneratePlant();

	//*scalingFactor = 1.0f;

	////Creo el VAO
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	//if ((*programa).getPos() != -1)
	//{
	//	LoadVBO(posVBO, numVerts * sizeof(float) * 3, vertexArray, 3, (*programa).getPos());
	//}
	//if ((*programa).getColor() != -1)
	//{
	//	LoadVBO(colorVBO, cubeNVertex * sizeof(float) * 3, cubeVertexColor, 3, (*programa).getColor());
	//}
	//if ((*programa).getNormal() != -1)
	//{
	//	LoadVBO(normalVBO, numVerts * sizeof(float) * 3, normalArray, 3, (*programa).getNormal());
	//}
	//if ((*programa).getTexCoord() != -1)
	//{
	//	LoadVBO(texCoordVBO, numVerts * sizeof(float) * 2, uvArray, 2, (*programa).getTexCoord());
	//}
	////if ((*programa).getTangent() != -1)
	////{
	////	LoadVBO(tangentVBO, numVerts * sizeof(float) * 3, tangentArray, 3, (*programa).getTangent());
	////}
	////if ((*programa).getTexCoordU() != -1)
	////{
	////	LoadVBO(texCoordUVBO, numVerts * sizeof(int), uArray, 1, (*programa).getTexCoordU());
	////}

	//LoadIBO(triangleIndexVBO, numFaces * sizeof(unsigned int) * 3, arrayIndex);

	//colorTex.LoadTexture();

	////if (this->typeMesh == DISPLACEMENT_MESH)
	////{
	////	colorTex2.LoadTexture();
	////	colorTex3.LoadTexture();
	////}

	//specularTex.LoadTexture();
	////displacementMap.LoadTexture();

	//model = glm::mat4(1.0f);
}

void Mesh::InitPlanePlant(float width, float height, float disp, float scaleFactor, bool invert, float tiling)
{
	GeneratePlane((int)width, (int)height, tiling, disp, invert);

	*scalingFactor = scaleFactor;

	//Creo el VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if ((*programa).getPos() != -1)
	{
		LoadVBO(posVBO, numVerts * sizeof(float) * 3, vertexArray, 3, (*programa).getPos());
	}
	if ((*programa).getNormal() != -1)
	{
		LoadVBO(normalVBO, numVerts * sizeof(float) * 3, normalArray, 3, (*programa).getNormal());
	}
	if ((*programa).getTexCoord() != -1)
	{
		LoadVBO(texCoordVBO, numVerts * sizeof(float) * 2, uvArray, 2, (*programa).getTexCoord());
	}

	LoadIBO(triangleIndexVBO, numFaces * sizeof(unsigned int) * 3, arrayIndex);

	colorTex.LoadTexture();
	specularTex.LoadTexture();
	emiTex.LoadTexture();
	//displacementMap.LoadTexture();

	model = glm::mat4(1.0f);
}

void Mesh::CreateColorVertex(int width, int height)
{
	float r1 = 0.0f / 255.0f;
	float r2 = 120.0f / 255.0f;
	float r3 = 0.0f / 255.0f;
	bool rchange = false;
	colorVertex = new float[width * height];

	float ra = (r2 - r1) / height;
	float rc = r1;
	for (int fil = 0; fil < height; fil++, rc += ra)
	{
		for (int col = 0; col < width; col++)
		{
			colorVertex[fil] = rc;
		}

		if (fil >= (height * 0.5) && !rchange)
		{
			ra = (r3 - r2) / height;
			rchange = true;
		}
	}
}