#include "Enemy.h"

Vector3D Enemy::InitPos() {
	int x, y, z;
	do {
		x = rand() % 4;
		z = rand() % (4 - x);
	} while (x + z == 0);
	y = 9.7 - x - z;
	return Vector3D(x,y,z);
}

bool Enemy::CheckFall() {
	if (this->GetPosition().GetY() <= 8 - this->GetPosition().GetX() - this->GetPosition().GetZ()) {
		this->SetIsAffectedByGravity(false);
		this->SetPosition(Vector3D(this->GetPosition().GetX(), 8 - this->GetPosition().GetX() - this->GetPosition().GetZ(), this->GetPosition().GetZ()));
		return true;
	}
	return false;
}