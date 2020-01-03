#pragma once
#include "QbertObject.h"
class Player :	public QbertObject
{
private:
public:
	Player(Vector3D position, Color color, Vector3D speed, Vector3D orientation) : QbertObject(position, color, speed, orientation) {}
};

