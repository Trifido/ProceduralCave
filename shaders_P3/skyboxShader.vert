#version 330 core

in vec3 inPos;

out vec3 texCoord;

uniform mat4 projection;
uniform mat4 modelViewProj;
uniform mat4 view;

void main()
{
    texCoord = inPos;
    //vec4 pos =   projection * view * vec4(inPos, 1.0);  
	//gl_Position = pos;//.xyww;

	gl_Position = modelViewProj * vec4 (inPos,1.0);
}  