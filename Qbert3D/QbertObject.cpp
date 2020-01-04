#include "QbertObject.h"

void QbertObject::move(int dir) {
	switch (dir) {
		case 0:			
			this->SetSpeed(Vector3D(0, 1, -1));
			break;
		case 1:			
			this->SetSpeed(Vector3D(-1, 1, 0));
			break;
		case 2:			
			this->SetSpeed(Vector3D(0, -1, 1));
			break;
		case 3:
			this->SetSpeed(Vector3D(1, -1, 0));
			break;
	}
}