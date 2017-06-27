#version 330 core
#extension GL_ARB_shading_language_420pack : enable //Habilita una extension de OpenGL 4.2 para usar el layout (binding)

#define MAX_SIZE 8u

out vec4 outColor;

in vec3 color;
in vec3 pos;
in vec3 norm;
in vec2 texCoord;
in vec3 tangent;
in vec4 newVertexPos;
in vec3 lightPointColor;

//Texturas del objeto

uniform sampler2D colorTex;
uniform sampler2D colorTex2;
uniform sampler2D colorTex3;
uniform sampler2D emiTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

//Intensidad de luz ambiental
uniform vec3 Ia;

//Arrays de coordenadas de luces
uniform vec3 PosPoint[MAX_SIZE];
uniform vec3 PosSpot[MAX_SIZE];
uniform vec3 PosDirec[MAX_SIZE];

//Arrays de intensidades de luces
uniform vec3 intPoint[MAX_SIZE];
uniform vec3 intSpot[MAX_SIZE];
uniform vec3 intDirec[MAX_SIZE];

//Número máximo de luces
uniform int numPoint;
uniform int numSpot;
uniform int numDirec;

//Propiedades del objeto
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 Ke, Ke2, Ke3;
vec3 N;
vec3 noiseLight;

float n;

in vec4 vertex;
uniform vec4 CameraEye;

vec3 shade(int i);
vec3 spotShade(int i);
vec3 direcShade(int i);
vec3 CalcBumpedNormal();
vec3 applyFog(vec3  rgb, float distance, vec3  rayOri, vec3  rayDir);
float GetAttenuationFactor(vec3 position);

float getFogFactor(float d)
{
    const float FogMax = 70.0;
    const float FogMin = 20;

    if (d>=FogMax) return 1;
    if (d<=FogMin) return 0;

    return 1 - (FogMax - d) / (FogMax - FogMin);
}

void main()
{
	Ka = texture(colorTex, texCoord).rgb;
	Ka += color;
	Kd = Ka;
	Ke = texture(emiTex, texCoord).rgb;
	Ke2 = texture(colorTex2, texCoord).rgb;
	Ks = texture(specularTex, texCoord).xyz;
	noiseLight = texture(colorTex3, texCoord).rgb;
	
	n = 200.0;
	
	N = normalize (norm);
	//N = CalcBumpedNormal();

	vec3 colorP;

	outColor = vec4(0);

	for(int i = 0; i < numPoint; i++)
	{
		colorP = shade(i);
	}

	for(int i = 0; i < numSpot; i++)
	{
		colorP = spotShade(i);   
	}

	for(int i = 0; i < numDirec; i++)
	{
		colorP = direcShade(i);   
	}

	float d = length(pos);
	float alpha = getFogFactor(d);

	vec4 FogColor = vec4(0.3,0.4,0.5, 1.0);

	outColor = mix(vec4(colorP, 1.0), FogColor, alpha);

	if(noiseLight.r < 0.02 && noiseLight.g < 0.02 && noiseLight.b < 0.02)
	{
		outColor = vec4(0.0, 1.0, 0.0, 1.0);
	}
	else if(noiseLight.r < 0.03 && noiseLight.g < 0.03 && noiseLight.b < 0.03)
	{
		outColor = vec4(0.0, 1.0, 0.7, 1.0);
	}

	//outColor = vec4(colorP, 1.0);
}

vec3 shade(int i)
{
//Ia
	vec3 color =  vec3(0.1);//Ia * Ka;

	//Diffuse 
	vec3 L = normalize (PosPoint[i] - pos);
	//
	vec3 diffuse = GetAttenuationFactor(PosPoint[i]) * intPoint[i] * Kd * dot (L,N);
	color += clamp(diffuse, 0.0, 1.0);
	
	//Specular
	//vec3 V = normalize (-pos);
	//vec3 R = normalize (reflect (-L,N));
	//float factor = max (dot (R,V), 0.01);
	// GetAttenuationFactor(PosPoint[i]) *
	//vec3 specular = GetAttenuationFactor(PosPoint[i]) * intPoint[i] * Ks * pow(factor,n);
	//color += clamp(specular, 0.0, 1.0);

	//Color-Textura emisivo
	color += Ke;
	
	return color;
}

vec3 spotShade(int i)
{
	vec3 Ip;
	vec3 color = Ia * Ka;

	vec3 D = normalize(PosSpot[i]);
	vec3 L = normalize(vec3(PosSpot[i] - pos));

	vec3 diff = intSpot[i] * Kd * dot(N, L);	

	float radiAng = 15.0 * 3.1416 / 180.0;
	float cosAng = cos(radiAng);
	float escPro = dot(D, L);

	if( escPro > cosAng){

		vec3 V = normalize(-pos);
		vec3 R = reflect (-L, N); //Se usa -L porque L es la dirección contraria del rayo.
		float Sfactor = max (dot(R, V), 0.0001);
		Sfactor = pow (Sfactor,n);
		vec3 spec = intSpot[i] * Ks * Sfactor;

		Ip = pow( (escPro - cosAng) / (1.0 - cosAng) , n) * (diff + spec);

	}
	else{
		Ip = vec3(0.0);
		color += diff;
	}

	color += clamp(Ip, 0.0, 1.0);
	color += Ke;

	return color;
}

vec3 direcShade(int i)
{
	vec3 color = Ia * Ka;

	//Diffuse 
	vec3 L = normalize (PosDirec[i]);
	vec3 diffuse = intDirec[i] * Kd * dot (L,N);
	color += clamp(diffuse, 0.0, 1.0);
	
	//Specular
	vec3 V = normalize (-pos);
	vec3 R = normalize (reflect (-L,N));
	float factor = max (dot (R,V), 0.0001);

	vec3 specular = intDirec[i] * Ks * pow(factor,n);
	color += clamp(specular, 0.0, 1.0);

	//Color-Textura emisivo
	color += Ke;
	
	return color;
}

vec3 CalcBumpedNormal()
{
    vec3 Tangent = normalize(tangent);
    Tangent = normalize(Tangent - dot(Tangent, N) * N);
    vec3 Bitangent = cross(Tangent, N);
    vec3 BumpMapNormal = texture(normalTex, texCoord).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, N);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}

vec3 applyFog( in vec3  rgb, in float distance, in vec3  rayOri, in vec3  rayDir )  // camera to point vector
{
	float b = 0.025;
	float c = 0.035;

    float fogAmount = c * exp(-rayOri.y*b) * (1.0-exp( -distance*rayDir.y*b ))/rayDir.y;
    vec3 fogColor = vec3(0.5,0.6,0.7);
    return mix(rgb, fogColor, fogAmount);
}

float GetAttenuationFactor(vec3 position)
{
	float result = 1.0;

	float c1 = 1.0;
	float c2 = 0.5;
	float c3 = 0.05;

	float dist = length(normalize(position));
	result /= (c1 + c2*dist + c3*dist*dist);

	return min(result, 1.0);
}