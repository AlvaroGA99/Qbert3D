#pragma once
#include "Sphere.h"
class QbertObject : public Sphere
{
public:
	QbertObject(Vector3D position, Color color, Vector3D speed, Vector3D orientation): Sphere(position, color, speed, orientation) {}
	void move(int dir);
};

