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

	//vec4 bumpData = texture( displacementMap, vec2(inTexCoord.x + time, inTexCoord.y));
	vec4 newVertexPos = vec4(inPos , 1.0); //+ inNormal * bumpData.y * scalingFactor
	
	gl_Position = modelViewProj * newVertexPos;
}
