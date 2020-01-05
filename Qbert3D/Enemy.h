#pragma once
#include "QbertObject.h"
class Enemy : public QbertObject
{
public : 
	Enemy() : QbertObject(Vector3D(1,6,1 ), Color(1,0,0), Vector3D(), Vector3D()) {}
	~Enemy() {
		delete this;
	}
};

