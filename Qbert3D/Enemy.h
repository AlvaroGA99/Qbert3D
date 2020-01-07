#pragma once
#include "QbertObject.h"
class Enemy : public QbertObject
{
private:
	int initY;
	bool isDead;
	bool ready;
public : 
	Enemy() : QbertObject(Vector3D(), Color(1, 0, 0), Vector3D(), Vector3D()), ready(false), isDead(true) {}
	Vector3D InitPos();
	void SetReady(const bool& ready) { this->ready = ready; }
	bool GetReady() const { return this->ready; }
	void SetIsDead(const bool& isDead) { this->isDead = isDead; }
	bool GetIsDead() const { return this->isDead; }
	void CheckFall();
	/*~Enemy() {
		delete this;
	}*/
};

