#pragma once
#include "Sphere.h"
class QbertObject : public Sphere
{
private:
	Vector3D prevPos;
	int preCount;
public:
	QbertObject(Vector3D position, Color color, Vector3D speed, Vector3D orientation) : Sphere(position, color, speed, orientation), prevPos(Vector3D(0, 0, 0)) { this->SetIsAffectedByGravity(false); }
	inline Vector3D GetPrevPosition() const { return this->prevPos; }
	inline void SetPrevPosition(const Vector3D& vector) { this->prevPos = vector; }
	void SetPreCount(const int& preCount) { this->preCount = preCount; }
	int GetPreCount() { return this->preCount; }
	void UpdatePreCount() { this->preCount--; }
	void Move(int dir);
};

