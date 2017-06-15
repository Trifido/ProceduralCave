#ifndef _SMOOTH_H
#define _SMOOTH_H

#include <glm/glm.hpp>
#include <vector>
#include <math.h>

float Lerp(float start, float end, float percent);

class SmoothSurface
{
	private:
		static const unsigned nMaxVertex = 1000;
		unsigned vertexUsage = 0;
		float values[nMaxVertex];
		float finalValues[nMaxVertex];
		const float PI = (float)atan(1) * 4;
	public:
		SmoothSurface();
		void SetSmoothSurface(unsigned vertexUsage);
		//SmoothSurface(unsigned vertexUsage);
		float SmoothSurface::Calculate(const float &x);	
		float *GetValues() { return &values[0]; }
		unsigned GetTotalValues() { return vertexUsage; }
		void SetValue(float num);
};


#endif
