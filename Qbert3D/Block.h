#pragma once
#include "Cube.h"
class Block : public Cube
{
private:
	Vector3D mapPosition;
public:
	Block() : Cube() {}

	Block(Vector3D position, Color color, Vector3D speed, Vector3D orientation) :
		Cube(position, color, speed, orientation) {		this->SetIsAffectedByGravity(false);	}
};

