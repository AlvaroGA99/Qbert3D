#include "QbertObject.h"

//Determina como se mueven los entes del juego
void QbertObject::Move(int dir) {
	this->SetPrevPosition(this->GetPosition());
	if (!preCount) {
		switch (dir) {
		case 0:
			this->SetSpeed(Vector3D(0, .25, -.25));
			break;
		case 1:
			this->SetSpeed(Vector3D(-.25, .25, 0));
			break;
		case 2:
			this->SetSpeed(Vector3D(0, -.25, .25));
			break;
		case 3:
			this->SetSpeed(Vector3D(.25, -.25, 0));
			break;
		}
	}
}