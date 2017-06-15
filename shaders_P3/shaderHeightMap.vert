#version 330 core

#define MAX_SIZE_DISP 500u

in vec3 inPos;	
in vec3 inColor;
in vec2 inTexCoord;
in vec3 inNormal;
in vec3 inTangent;
in int inTexCoordU;

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;
uniform sampler2D displacementMap;
uniform float scalingFactor;
uniform float displacement1D[MAX_SIZE_DISP];

out vec3 color;
out vec3 pos;
out vec3 norm;
out vec2 texCoord;
out vec3 tangent;

void main()
{
	color = inColor;
	tangent = (modelView * vec4(inTangent, 0)).xyz;
	texCoord = inTexCoord;
	norm = (normal * vec4(inNormal, 0.0)).xyz;
	pos = (modelView * vec4(inPos, 1.0)).xyz;

	//-----------------------------------------------------------
	//float scalingFactor = 0.5f;
	
	vec4 bumpData = texture2D( displacementMap, inTexCoord.xy );
	float smoothBump = displacement1D[inTexCoordU];

	float df = 0.30*bumpData.x + 0.59*(bumpData.y*0.7 + smoothBump*0.3) + 0.11*bumpData.z;

	//float df = smoothBump;

	vec4 newVertexPos = vec4(inPos + inNormal * df * scalingFactor, 1.0);
	
	gl_Position = modelViewProj * newVertexPos;

	//-----------------------------------------------------------
	
	//gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
