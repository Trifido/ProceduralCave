#include "Spline.h"

Spline::Spline()
{
	pointsPerCurve = 10;
	numCurves = 3;
	numInitPoints = 6;
	totalPoints = numCurves * pointsPerCurve;

	this->initPoints = new Vector[numInitPoints];
	this->initScalePoints = new Vector[numInitPoints];
	this->scalePoints = new Vector[numInitPoints];
	this->pathCoords = new float[totalPoints * numCurves];
	this->vectPathCoords = new Vector[totalPoints];
	this->vectPathScaleCoords = new Vector[totalPoints];

	this->initPoints[0] = Vector(-0.500000f, 0.000000f, -0.866025f);
	this->initPoints[1] = Vector(1.000000f, 0.000000f, 0.000000f);
	this->initPoints[2] = Vector(-0.500000f, 0.000000f, 0.866025f);
	this->initPoints[3] = Vector(-0.500000f, 0.000000f, -0.866025f);
	this->initPoints[4] = Vector(1.000000f, 0.000000f, 0.000000f);
	this->initPoints[5] = Vector(-0.500000f, 0.000000f, 0.866025f);

	this->scalePoints[0] = Vector(-0.500000f, 0.000000f, -0.866025f);
	this->scalePoints[1] = Vector(1.000000f, 0.000000f, 0.000000f);
	this->scalePoints[2] = Vector(-0.500000f, 0.000000f, 0.866025f);
	this->scalePoints[3] = Vector(-0.500000f, 0.000000f, -0.866025f);
	this->scalePoints[4] = Vector(1.000000f, 0.000000f, 0.000000f);
	this->scalePoints[5] = Vector(-0.500000f, 0.000000f, 0.866025f);

	int indice = 0;
	int indiceLinear = 0;

	for (int sp = 0, indEj = 0; sp < 3; sp++, indEj++) {
		for (float t = 0.0f; t < 1.0f; t += 1.0f / pointsPerCurve, indiceLinear += 3, indice++) {
			vectPathCoords[indice] = GenerateSpline(t, initPoints[indEj], initPoints[indEj + 1], initPoints[indEj + 2], initPoints[indEj + 3]);
			pathCoords[indiceLinear] = vectPathCoords[indice].x;
			pathCoords[indiceLinear + 1] = vectPathCoords[indice].y;
			pathCoords[indiceLinear + 2] = vectPathCoords[indice].z;
			//printf("(%f, %f, %f)\n", vectPathCoords[indice].x, vectPathCoords[indice].y, vectPathCoords[indice].z);
		}
	}
}

Spline::Spline(float yCamera, float factorScale)
{
	pointsPerCurve = 10;
	numCurves = 3;
	numInitPoints = 6;
	totalPoints = numCurves * pointsPerCurve;
	this->factorScale = factorScale;

	this->initPoints = new Vector[numInitPoints];
	this->initScalePoints = new Vector[numInitPoints];
	this->scalePoints = new Vector[numInitPoints];
	this->pathCoords = new float[totalPoints * numCurves];
	this->vectPathCoords = new Vector[totalPoints];
	this->vectPathScaleCoords = new Vector[totalPoints];

	this->initPoints[0] = Vector(-0.500000f, yCamera, -0.866025f);
	this->initPoints[1] = Vector(1.000000f, yCamera, 0.000000f);
	this->initPoints[2] = Vector(-0.500000f, yCamera, 0.866025f);
	this->initPoints[3] = Vector(-0.500000f, yCamera, -0.866025f);
	this->initPoints[4] = Vector(1.000000f, yCamera, 0.000000f);
	this->initPoints[5] = Vector(-0.500000f, yCamera, 0.866025f);

	this->scalePoints[0] = Vector(-0.500000f, yCamera, -0.866025f);
	this->scalePoints[1] = Vector(1.000000f, yCamera, 0.000000f);
	this->scalePoints[2] = Vector(-0.500000f, yCamera, 0.866025f);
	this->scalePoints[3] = Vector(-0.500000f, yCamera, -0.866025f);
	this->scalePoints[4] = Vector(1.000000f, yCamera, 0.000000f);
	this->scalePoints[5] = Vector(-0.500000f, yCamera, 0.866025f);

	for (int i = 0; i < numInitPoints; i++)
	{
		scalePoints[i].x *= factorScale;
		scalePoints[i].z *= factorScale;
		this->initScalePoints[i].x = scalePoints[i].x;
		this->initScalePoints[i].y = 0.0f;
		this->initScalePoints[i].z = scalePoints[i].z;
	}

	int indice = 0;
	int indiceLinear = 0;

	for (int sp = 0, indEj = 0; sp < 3; sp++, indEj++) {
		for (float t = 0.0f; t < 1.0f; t += 1.0f / pointsPerCurve, indiceLinear += 3, indice++) {
			vectPathCoords[indice] = GenerateSpline(t, initPoints[indEj], initPoints[indEj + 1], initPoints[indEj + 2], initPoints[indEj + 3]);
			vectPathScaleCoords[indice] = GenerateSpline(t, scalePoints[indEj], scalePoints[indEj + 1], scalePoints[indEj + 2], scalePoints[indEj + 3]);
			pathCoords[indiceLinear] = vectPathCoords[indice].x;
			pathCoords[indiceLinear + 1] = vectPathCoords[indice].y;
			pathCoords[indiceLinear + 2] = vectPathCoords[indice].z;
			//printf("(%f, %f, %f)\n", vectPathCoords[indice].x, vectPathCoords[indice].y, vectPathCoords[indice].z);
		}
	}
}

Vector Spline::GenerateSpline(float t, Vector p1, Vector p2, Vector p3, Vector p4)
{
	float t2 = t*t;
	float t3 = t*t*t;
	Vector v; // Interpolated point

   //Catmull Rom spline Calculation
	v.x = ((-t3 + 2 * t2 - t)*(p1.x) + (3 * t3 - 5 * t2 + 2)*(p2.x) + (-3 * t3 + 4 * t2 + t)* (p3.x) + (t3 - t2)*(p4.x)) / 2;
	v.z = ((-t3 + 2 * t2 - t)*(p1.z) + (3 * t3 - 5 * t2 + 2)*(p2.z) + (-3 * t3 + 4 * t2 + t)* (p3.z) + (t3 - t2)*(p4.z)) / 2;
	v.y = p1.y;

	return v;
}

Vector Spline::GetCoord(int t)
{
	t %= totalPoints;
	return vectPathCoords[t];
}

Vector Spline::GetScaleCoord(int t)
{
	t %= totalPoints;
	return vectPathScaleCoords[t];
}

Vector Spline::CalculateNewCoords(float t, int indCurve)
{
	return GenerateSpline(t, scalePoints[indCurve], scalePoints[indCurve + 1], scalePoints[indCurve + 2], scalePoints[indCurve + 3]);
}

Vector Spline::CalculateInitialCoord(float t, int indCurve)
{
	return GenerateSpline(t, initPoints[indCurve], initPoints[indCurve + 1], initPoints[indCurve + 2], initPoints[indCurve + 3]);
}

void Spline::ScaleZPoints(float factor)
{
	this->factorScale = factor;
	for (int i = 0; i < numInitPoints; i++)
	{
		scalePoints[i].z *= factor;
	}
}

void Spline::ScaleXPoints(float factor)
{
	this->factorScale = factor;
	for (int i = 0; i < numInitPoints; i++)
	{
		scalePoints[i].x *= factor;
	}
}

void Spline::GenerateScaleCoords()
{
	int indice = 0;

	for (int sp = 0, indEj = 0; sp < 3; sp++, indEj++) {
		for (float t = 0.0f; t < 1.0f; t += 1.0f / pointsPerCurve, indice++) {
			vectPathScaleCoords[indice] = GenerateSpline(t, scalePoints[indEj], scalePoints[indEj + 1], scalePoints[indEj + 2], scalePoints[indEj + 3]);
		}
	}
}

void Spline::ScaleInitPoint(float factorScale)
{
	this->factorScale = factorScale;

	for (int i = 0; i < numInitPoints; i++)
	{
		scalePoints[i].x *= factorScale;
		scalePoints[i].z *= factorScale;
		this->initScalePoints[i].x = scalePoints[i].x;
		this->initScalePoints[i].y = 0.0f;
		this->initScalePoints[i].z = scalePoints[i].z;
	}
}

void Spline::TranslationX(int id, float x)
{
	//this->initScalePoints[id].x += x;
	this->initPoints[id].x += x;
}

void Spline::TranslationZ(int id, float z)
{
	//this->initScalePoints[id].z += z;
	this->initPoints[id].z += z;
}

void Spline::UpdateSpline()
{
	for (int i = 0; i < numInitPoints; i++)
	{
		scalePoints[i].x = initPoints[i].x * factorScale;
		scalePoints[i].z = initPoints[i].z * factorScale;
		printf("Scale: %f\n", factorScale);
		printf("%f, 0.0, %f\n", scalePoints[i].x, scalePoints[i].z);
	}

	int indice = 0;

	for (int sp = 0, indEj = 0; sp < 3; sp++, indEj++) {
		for (float t = 0.0f; t < 1.0f; t += 1.0f / pointsPerCurve, indice++) {
			vectPathScaleCoords[indice] = GenerateSpline(t, scalePoints[indEj], scalePoints[indEj + 1], scalePoints[indEj + 2], scalePoints[indEj + 3]);
		}
	}
}

void Spline::PrintfSpline()
{
	int indice = 0;
	int indiceLinear = 0;

	for (int sp = 0, indEj = 0; sp < 3; sp++, indEj++) {
		for (float t = 0.0f; t < 1.0f; t += 1.0f / pointsPerCurve, indiceLinear += 3, indice++) {
			printf("%f, 0.0, %f\n", vectPathScaleCoords[indice].x, vectPathScaleCoords[indice].z);
		}
	}
}