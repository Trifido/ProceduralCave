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
uniform float time;
uniform float scalingFactor;

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
	
	//vec4 bumpData = texture2D( displacementMap, inTexCoord.xy );	
	vec4 bumpData = texture( displacementMap, vec2(inTexCoord.x + time, inTexCoord.y));

	//float df = 0.30*bumpData.x + 0.59*bumpData.y + 0.11*bumpData.z;

	vec4 newVertexPos = vec4(inPos + inNormal * bumpData.y * scalingFactor, 1.0); 
	
	gl_Position = modelViewProj * newVertexPos;

	//-----------------------------------------------------------
	
	//gl_Position =  modelViewProj * vec4 (inPos,1.0);
}