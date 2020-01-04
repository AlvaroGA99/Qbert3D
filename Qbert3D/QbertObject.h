#pragma once
#include "Sphere.h"
class QbertObject : public Sphere
{
private:
	Vector3D prevPos;
public:
	QbertObject(Vector3D position, Color color, Vector3D speed, Vector3D orientation): Sphere(position, color, speed, orientation), prevPos(Vector3D(0,0,0)) {}
	inline Vector3D GetPrevPosition() const { return this->prevPos; }
	inline void SetPrevPosition(const Vector3D& vector) { this->prevPos = vector; }
	void move(int dir);
};

