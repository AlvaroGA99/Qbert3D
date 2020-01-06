#pragma once
#include "QbertObject.h"
class Enemy : public QbertObject
{
private:
	int initY;
public : 
	Enemy() : QbertObject(Vector3D(), Color(1, 0, 0), Vector3D(), Vector3D()) {}
	Vector3D InitPos();
	bool CheckFall();
	/*~Enemy() {
		delete this;
	}*/
};

