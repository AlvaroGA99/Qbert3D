#pragma once
#include "QbertObject.h"
class Player :	public QbertObject
{
private:
	int lives;
public:
	Player(Vector3D position, Color color, Vector3D speed, Vector3D orientation) : QbertObject(position, color, speed, orientation), lives(3) {}
	const int GetLives() const{ return lives; }
	void SetLives(const int& lives) { this->lives = lives; }
	void LoseLife() { this->lives--; }
};

