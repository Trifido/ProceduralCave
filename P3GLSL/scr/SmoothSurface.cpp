#include "SmoothSurface.h"

SmoothSurface::SmoothSurface() {
	SetSmoothSurface(500);
}

void SmoothSurface::SetSmoothSurface(unsigned vertexUsage)
{
	srand(vertexUsage);

	for (unsigned i = 0; i < nMaxVertex; ++i)
	{
		values[i] = ((float)rand() / (RAND_MAX));
	}

}

float SmoothSurface::Calculate(const float &x)
{
	int xi = (int)x;
	int xMin = xi % (int)nMaxVertex;
	float t = x - xi;
	int xMax = (xMin == nMaxVertex - 1) ? 0 : xMin + 1;

	assert(t >= 0 && t <= 1);
	float tRemapCosine = (1 - (float)cos(t * PI)) * 0.5f;

	return Lerp(values[xMin], values[xMax], tRemapCosine);
}

float Lerp(float start, float end, float percent)
{
	return (start + percent*(end - start));
}

void SmoothSurface::SetValue(float num)
{
	finalValues[vertexUsage] = num;
	vertexUsage++;
}