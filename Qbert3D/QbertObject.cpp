#include "QbertObject.h"

void QbertObject::move(int dir) {
	switch (dir) {
		case 0:
			//x+1,y+1,z+1
			this->SetSpeed(Vector3D(1, 1, 1));
			break;
		case 1:
			//x-1,y+1,z+1
			this->SetSpeed(Vector3D(-1, 1, 1));
			break;
		case 2:
			//x-1,y-1,z-1
			this->SetSpeed(Vector3D(-1, -1, -1));
			break;
		case 3:
			//x+1,y-1,z+1
			this->SetSpeed(Vector3D(1, -1, 1));
			break;
	}
}