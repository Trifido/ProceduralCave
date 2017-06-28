#ifndef _SPLINE_H
#define _SPLINE_H

#include <iostream>
#include <vector>

//La spline es de tipo Catmull-Room siguiendo el codigo que se daba.
//Se presentan distintos vectores, para controlar los puntos de control, puntos internos en las curvas,
// puntos escalados.

//Se ha hardcodeado los puntos de control debido a que en la implementacion de la arquitectura, al querer 
//modificar la misma para permitir que un objeto Spline sea el mismo para las distintas mallas, tenemos problemas
//con los puntos escalados, esto ha sido más una cuestión de tiempo y hemos querido dejarlo con el menor numero de bugs posible.

class Vector
{
public:
	float x, y, z;
	Vector() {};
	Vector(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void Normalize() {
		float mod = sqrt(x*x + y*y + z*z);
		this->x /= mod;
		this->y /= mod;
		this->z /= mod;
	}
};

class Spline
{
	private:
		int totalPoints;
		int numInitPoints;
		int pointsPerCurve;
		int numCurves;
		Vector *initPoints;
		Vector *initScalePoints;
		Vector *scalePoints;
		Vector *vectPathCoords;
		std::vector<Vector> vectPathCamera;
		std::vector<float> anglePathCamera;
		Vector *vectPathScaleCoords;
		float *pathCoords;
		float factorScale;

	public:
		Spline();
		Spline(float yCamera, float factorScale);
		inline float *GetPath() { return pathCoords; }
		inline Vector *GetPathVector() { return vectPathCoords; }
		Vector GetCoord(int t);
		Vector GenerateSpline(float t, Vector p1, Vector p2, Vector p3, Vector p4);
		inline int GetPointsPerCurve() { return pointsPerCurve; }
		inline int GetTotalPoints() { return totalPoints; }
		inline int GetTotalCurves() { return numCurves; }
		Vector CalculateNewCoords(float t, int indCurve);
		Vector CalculateInitialCoord(float t, int indCurve);
		void ScaleZPoints(float factor);
		void ScaleXPoints(float factor);
		void GenerateScaleCoords();
		Vector GetScaleCoord(int t);
		inline Vector *GetInitScalePoints(){ return initScalePoints; }
		inline int GetNumInitPoints() { return numInitPoints; }
		void ScaleInitPoint(float factorScale);
		void TranslationX(int id, float x);
		void TranslationZ(int id, float z);
		void UpdateSpline();
		void PrintfSpline();
		int GetNumPathPoints();
};

#endif