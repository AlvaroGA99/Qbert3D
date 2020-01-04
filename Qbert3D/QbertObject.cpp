#include "QbertObject.h"

void QbertObject::move(int dir) {
	this->SetPrevPosition(this->GetPosition());
	switch (dir) {
		case 0:			
			this->SetSpeed(Vector3D(0, .5, -.5));
			break;
		case 1:			
			this->SetSpeed(Vector3D(-.5, .5, 0));
			break;
		case 2:			
			this->SetSpeed(Vector3D(0, -.5, .5));
			break;
		case 3:			
			this->SetSpeed(Vector3D(.5, -.5, 0));
			break;
	}
}