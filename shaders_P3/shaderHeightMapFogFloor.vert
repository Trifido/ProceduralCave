#version 330 core

#define MAX_SIZE_DISP 500u

in vec3 inPos;	
in vec3 inColor;
in vec2 inTexCoord;
in vec3 inNormal;
in vec3 inTangent;
in int inTexCoordU;
in int inTexCoordV;

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;
uniform sampler2D displacementMap;
uniform float scalingFactor;
uniform float displacement1D[MAX_SIZE_DISP];
//uniform float gradientColor[MAX_SIZE_DISP];

out vec3 color;
out vec3 pos;
out vec3 norm;
out vec2 texCoord;
out vec3 tangent;
out vec4 vertex;
out vec4 newVertexPos;
out vec3 lightPointColor;

void main()
{
	color = inColor;
	//float redColor = gradientColor[inTexCoordV];
	//color = vec3(redColor, 0.0, 0.0);
	tangent = (modelView * vec4(inTangent, 0)).xyz;
	texCoord = inTexCoord;
	norm = (normal * vec4(inNormal, 0.0)).xyz;
	pos = (modelView * vec4(inPos, 1.0)).xyz;

	//-----------------------------------------------------------
	//float scalingFactor = 0.5f;
	
	vec4 bumpData = texture2D( displacementMap, inTexCoord.xy );
	float smoothBump = displacement1D[inTexCoordU];

	float df = 0.30*bumpData.x + 0.59*(bumpData.x*0.7 + smoothBump*0.3) + 0.11*bumpData.x;
	//float df = 0.30*bumpData.x + 0.59*(bumpData.x*0.7) + 0.11*bumpData.x;

	//float df = smoothBump;

	newVertexPos = vec4(inPos + inNormal * df * scalingFactor, 1.0);
	//newVertexPos = vec4(inPos, 1.0);

	if(newVertexPos.y < 1.5)
		lightPointColor = vec3(0.0, 1.0, 0.0);
	else
		lightPointColor = vec3(0.0, 0.0, 0.0);

	gl_Position = modelViewProj * newVertexPos;

	//-----------------------------------------------------------
	
	//gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
