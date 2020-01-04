#include "Game.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Torus.h"
#include "Cuboid.h"
#include "Model.h"
#include "Enemy.h"
#include "Player.h"
#include "Block.h"

#define MAPSIZE 28

void Game::Create() 
{
	Scene* mainScene = new(nothrow) Scene();
	FlyingCamera mainCamera = mainScene->GetCamera();
	mainCamera.SetPosition(Vector3D(7.2f, 11.0f, 7.0f));
	mainCamera.SetOrientation(Vector3D(38.0f, -45.0f, 0.0f));
	mainScene->SetCamera(mainCamera);
	int floor= 0;
	int index = 0;
	Block* pointerToBlocks = new(nothrow) Block[MAPSIZE];
	if (pointerToBlocks != nullptr) {
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j <= i; j++) {
				pointerToBlocks[index] = Block(
					Vector3D((i + j-i), (7-i), (-j+i)),
					Color((0.2), (0.4), (0.7)), Vector3D(0, 0, 0),
					0.1
				);
				mainScene->AddGameObject(pointerToBlocks + index);
				index++;
			}
		}		
	}	
	this->thePlayer = new(nothrow) Player(
		Vector3D((0), (8), (0)),
		Color((0.8), (0.7), (0.2)),
		Vector3D(),
		Vector3D()
	);

	this->thePlayer->SetIsAffectedByGravity(false);

	mainScene->AddGameObject(this->thePlayer);
	this->scenes.push_back(mainScene);
	this->activeScene = mainScene;
}

void Game::Render() 
{
	this->activeScene->Render();
}

void Game::Update()
{
	milliseconds currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	if ((currentTime.count() - this->initialMilliseconds.count()) - this->lastUpdatedTime > UPDATE_PERIOD)
	{
		this->activeScene->Update(TIME_INCREMENT);
		this->lastUpdatedTime = currentTime.count() - this->initialMilliseconds.count();
	}
}

void Game::ProcessMouseMovement(const int& xPosition, const int& yPosition) 
{
	this->activeScene->ProcessMouseMovement(xPosition, yPosition);
}

void Game::ProcessKeyPressed(const unsigned char& key, const int& xPosition, const int& yPosition)
{
	
	switch (key) {
	case 'w' :
		if (this->thePlayer->GetSpeed)
		this->thePlayer->move(0);
		break;
	case 'a' :
		this->thePlayer->move(1);
		break;
	case 's' :
		this->thePlayer->move(2);
		break;
	case 'd' :
		this->thePlayer->move(3);
		break;
	}
	this->activeScene->ProcessKeyPressed(key, xPosition, yPosition);
}

void Game::ProcessMouseClick(const int& button, const int& state, const int& xPosition, const int& yPosition) 
{
	this->activeScene->ProcessMouseClick(button, state, xPosition, yPosition);
}
