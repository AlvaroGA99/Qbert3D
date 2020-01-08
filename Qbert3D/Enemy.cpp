#include "Enemy.h"

//Genera una posición aleatoria para el enemigo
Vector3D Enemy::InitPos() {
	int x, y, z;
	do {
		x = rand() % 4;
		z = rand() % (4 - x);
	} while (x + z == 0);
	y = 11.7 - x - z;
	return Vector3D(x,y,z);
}

//Comprueba si al caer, el enemigo está sobre un bloque y de ser así, activa ready para que se pueda mover y se le fija la posición sobre el bloque
void Enemy::CheckFall() {	
	if (this->GetPosition().GetY() <= 8 - this->GetPosition().GetX() - this->GetPosition().GetZ()) {
		this->SetPreCount(25);
		this->SetIsAffectedByGravity(false);
		this->SetSpeed(Vector3D(0, 0, 0));
		this->SetPosition(Vector3D(this->GetPosition().GetX(), 8 - this->GetPosition().GetX() - this->GetPosition().GetZ(), this->GetPosition().GetZ()));
		ready = true;
	}
}