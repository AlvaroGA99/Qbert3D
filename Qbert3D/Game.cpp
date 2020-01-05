#include "Game.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Torus.h"
#include "Cuboid.h"
#include "Model.h"
#include <Math.h>
#include <ctime>

#define MAPSIZE 28

void Game::Create() 
{
	srand(time(NULL));
	Scene* mainScene = new(nothrow) Scene();
	FlyingCamera mainCamera = mainScene->GetCamera();
	mainCamera.SetPosition(Vector3D(7.2f, 11.0f, 7.0f));
	mainCamera.SetOrientation(Vector3D(38.0f, -45.0f, 0.0f));
	mainScene->SetCamera(mainCamera);
	int floor= 0;
	int index = 0;
	pointerToBlocks = new(nothrow) Block[MAPSIZE];
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

	Enemy* enemigo = new Enemy();

	this->thePlayer->SetIsAffectedByGravity(false);

	mainScene->AddGameObject(this->thePlayer);
	mainScene->AddGameObject(enemigo);
	this->scenes.push_back(mainScene);
	this->activeScene = mainScene;

	continueGame = true;

	std::cout << endl << endl << "�---------------------------------------------------------�" << endl;
	std::cout << "|                         QBERT-3D                        |" << endl;				   
	std::cout << "�---------------------------------------------------------�" << endl;
	std::cout << "|                                                         |" << endl;
	std::cout << "| Juego desarrollado por:                                 |" << endl;
	std::cout << "|                                                         |" << endl;
	std::cout << "|   +  Alvaro Gonzalez Alcazar                            |" << endl;
	std::cout << "|   +  Alejandro Rodriguez Munoz                          |" << endl;
	std::cout << "|                                                         |" << endl;
	std::cout << "|                                                         |" << endl;
	std::cout << "�---------------------------------------------------------�" << endl << endl << endl;
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
		if (continueGame) {
			this->activeScene->Update(TIME_INCREMENT);
			this->lastUpdatedTime = currentTime.count() - this->initialMilliseconds.count();

			if (thePlayer->GetPosition().GetX() <= -1 || thePlayer->GetPosition().GetZ() <= -1 || thePlayer->GetPosition().GetX() + thePlayer->GetPosition().GetZ() >= 7) {
				thePlayer->SetSpeed(Vector3D(0, -.3, 0));
				thePlayer->SetIsAffectedByGravity(true);
			}
			else if (!(thePlayer->GetSpeed() == Vector3D(0, 0, 0)) && ((abs(thePlayer->GetPosition().GetX() - thePlayer->GetPrevPosition().GetX()) >= 1) || abs(thePlayer->GetPosition().GetZ() - thePlayer->GetPrevPosition().GetZ()) >= 1)) {
				thePlayer->SetSpeed(Vector3D(0, 0, 0));
				thePlayer->SetPosition(Vector3D(roundf(thePlayer->GetPosition().GetX()), roundf(thePlayer->GetPosition().GetY()), roundf(thePlayer->GetPosition().GetZ())));

				for (int i = 0; i < MAPSIZE; i++) {
					if ((((pointerToBlocks + i)->GetPosition().GetX() == thePlayer->GetPosition().GetX()) && ((pointerToBlocks + i)->GetPosition().GetZ() == thePlayer->GetPosition().GetZ())) && !((pointerToBlocks + i)->GetColor() == Color(1, 0.5, 0))) {
						contadorBloques++;
						cout << endl << "   * Llevas " << contadorBloques << " bloques coloreados" << endl;
						(pointerToBlocks + i)->SetColor(Color(1, 0.5, 0));
						if (contadorBloques == MAPSIZE) {
							std::cout << endl << endl << "�---------------------------------------------------------�" << endl;
							std::cout << "|                                                         |" << endl;
							std::cout << "|                      HAS GANADO !!!!!!                  |" << endl;
							std::cout << "|                                                         |" << endl;
							std::cout << "�---------------------------------------------------------�" << endl;

							//pauseGame(); win(); nextLevel();
							continueGame = false;
						}
					}
					/*
					std::cout << "X: " << thePlayer->GetPosition().GetX() << " " << thePlayer->GetPrevPosition().GetX() << endl;
					std::cout << "Y: " << thePlayer->GetPosition().GetY() << " " << thePlayer->GetPrevPosition().GetY() << endl;
					std::cout << "Z: " << thePlayer->GetPosition().GetZ() << " " << thePlayer->GetPrevPosition().GetZ() << endl << endl;
					*/
				}
			}
			if (thePlayer->GetPosition().GetY() <= -4) {
				thePlayer->LoseLife();

				if (thePlayer->GetLives() >= 0) {
					std::cout << endl << endl << "�---------------------------------------------------------�" << endl;
					std::cout << "|                                                         |" << endl;
					std::cout << "|                    Vidas restantes:" << thePlayer->GetLives() << "                    |" << endl;
					std::cout << "|                                                         |" << endl;
					std::cout << "�---------------------------------------------------------�" << endl;
				}
				else {
					//pauseGame();
					continueGame = false;
					std::cout << endl << endl << "�---------------------------------------------------------�" << endl;
					std::cout << "|                                                         |" << endl;
					std::cout << "|                     FIN DEL JUEGO                       |" << endl;
					std::cout << "|                       CONTINUAR ?                       |" << endl;
					std::cout << "|                                                         |" << endl;
					std::cout << "�---------------------------------------------------------�" << endl;
					std::cout << endl << "   * Si deseas continuar presiona 1, de lo contrario presiona 0: ";
					std::cin >> continueGame;

					if (continueGame) {
						//restartGame();
						thePlayer->SetLives(3);
					}
					else {
						std::cout << "   * Gracias por jugar a QBERT-3D.";
						//exitGame();
					}
				}

				thePlayer->SetIsAffectedByGravity(false);
				thePlayer->SetSpeed(Vector3D(0, 0, 0));
				thePlayer->SetPosition(Vector3D(0, 8, 0));
			}
		}
	}
}

void Game::ProcessMouseMovement(const int& xPosition, const int& yPosition) 
{
	this->activeScene->ProcessMouseMovement(xPosition, yPosition);
}

void Game::ProcessKeyPressed(const unsigned char& key, const int& xPosition, const int& yPosition)
{	
	if (continueGame) {
		switch (key) {
		case'w':
		case 'W':
			if (this->thePlayer->GetSpeed() == Vector3D(0, 0, 0)) {
				this->thePlayer->Move(0);
			}
			break;
		case 'a':
		case 'A':
			if (this->thePlayer->GetSpeed() == Vector3D(0, 0, 0)) {
				this->thePlayer->Move(1);
			}
			break;
		case 's':
		case 'S':
			if (this->thePlayer->GetSpeed() == Vector3D(0, 0, 0)) {
				this->thePlayer->Move(2);
			}
			break;
		case 'd':
		case 'D':
			if (this->thePlayer->GetSpeed() == Vector3D(0, 0, 0)) {
				this->thePlayer->Move(3);
			}
			break;
		default:
			this->activeScene->ProcessKeyPressed(key, xPosition, yPosition);
		}
	}
	else {
		this->activeScene->ProcessKeyPressed(key, xPosition, yPosition);
	}
}

void Game::ProcessMouseClick(const int& button, const int& state, const int& xPosition, const int& yPosition) 
{
	this->activeScene->ProcessMouseClick(button, state, xPosition, yPosition);
}
