#pragma once
#include "QbertObject.h"
class Enemy : public QbertObject
{
private:
	int initY;
	bool ready;
public : 
	Enemy() : QbertObject(Vector3D(), Color(1, 0, 0), Vector3D(), Vector3D()), ready(false) {}
	Vector3D InitPos();
	void SetReady(const bool& ready) { this->ready = ready; }
	bool GetReady() const { return this->ready; }
	void CheckFall();
	/*~Enemy() {
		delete this;
	}*/
};

