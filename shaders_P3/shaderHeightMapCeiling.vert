#version 330 core

in vec3 inPos;	
in vec3 inColor;
in vec2 inTexCoord;
in vec3 inNormal;
in vec3 inTangent;

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;
uniform sampler2D displacementMap;
uniform float scalingFactor;

out vec4 color;
out vec3 pos;
out vec3 norm;
out vec2 texCoord;
out vec3 tangent;
out vec4 newVertexPos;

void main()
{
	color = vec4(inColor, 1.0);
	tangent = (modelView * vec4(inTangent, 0)).xyz;
	texCoord = inTexCoord;
	norm = (normal * vec4(inNormal, 0.0)).xyz;
	pos = (modelView * vec4(inPos, 1.0)).xyz;

	//-----------------------------------------------------------
	//float scalingFactor = 0.5f;
	
	vec4 bumpData = texture2D( displacementMap, inTexCoord.xy );	

	float df = 1.0*bumpData.x;// 0.30*bumpData.x +  0.11*bumpData.z;

	newVertexPos = vec4(inPos + inNormal * df * scalingFactor, 1.0);
	
	gl_Position = modelViewProj * newVertexPos;

	//-----------------------------------------------------------
	
	//gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
