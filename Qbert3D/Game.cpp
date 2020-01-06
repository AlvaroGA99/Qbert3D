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
	
	blockColor = new Color[6]{ Color((0.2), (0.4), (0.7)), Color((1), (0), (0)), Color((0.4), (0.7), (0.2)), Color((0.4), (0.2), (0.7)), Color((0.7), (0.12), (0.24)), Color((0.5), (.8), (0.55))};
	blockColor2 = new Color[6]{ Color(1, 0.5, 0), Color(0.5, 1, 0), Color((0.4), (0.7), (0.2)), Color((0.4), (0.2), (0.7)), Color((0.7), (0.12), (0.24)), Color((0.5), (.8), (0.55)) };
	continueGame = false;
	nextLevel = true;
	level = -1;
	preCount = 90;

	pointerToBlocks = new(nothrow) Block[MAPSIZE];
	if (pointerToBlocks != nullptr) {
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j <= i; j++) {
				pointerToBlocks[index] = Block(
					Vector3D((i + j-i), (7-i), (-j+i)),
					blockColor[0], Vector3D(0, 0, 0),
					0.1
				);
				mainScene->AddGameObject(pointerToBlocks + index);
				index++;
			}
		}		
	}

	enemies = new(nothrow) Enemy[3];
	enemies[0] = Enemy();
	enemies[1] = Enemy();
	enemies[2] = Enemy();

	this->thePlayer = new Player(
		Vector3D((0), (8), (0)),
		Color((0.8), (0.7), (0.2)),
		Vector3D(),
		Vector3D()
	);

	mainScene->AddGameObject(this->thePlayer);
	mainScene->AddGameObject(this->enemies);
	mainScene->AddGameObject(this->enemies+1);
	mainScene->AddGameObject(this->enemies);
	this->scenes.push_back(mainScene);
	this->activeScene = mainScene;


	std::cout << endl << endl << "ñ---------------------------------------------------------ñ" << endl;
	std::cout << "|                         QBERT-3D                        |" << endl;				   
	std::cout << "ñ---------------------------------------------------------ñ" << endl;
	std::cout << "|                                                         |" << endl;
	std::cout << "| Juego desarrollado por:                                 |" << endl;
	std::cout << "|                                                         |" << endl;
	std::cout << "|   +  Alvaro Gonzalez Alcazar                            |" << endl;
	std::cout << "|   +  Alejandro Rodriguez Munoz                          |" << endl;
	std::cout << "|                                                         |" << endl;
	std::cout << "|                                                         |" << endl;
	std::cout << "ñ---------------------------------------------------------ñ" << endl << endl << endl;
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
		//Inicio de nivel
		if (nextLevel) {
			if (preCount == 0) {
				nextLevel = false;
				level++;
				blockCounter = 0;
				ballCount = 0;
				preCount = 50;
				thePlayer->SetIsAffectedByGravity(false);
				thePlayer->SetSpeed(Vector3D(0, 0, 0));
				thePlayer->SetPosition(Vector3D(0, 8, 0));
				for (int i = 0; i < MAPSIZE; i++) {
					(pointerToBlocks + i)->SetColor(blockColor[level]);
				}
				continueGame = true;

				std::cout << endl << "ñ---------------------------------------------------------ñ" << endl;
				std::cout << "|                                                         |" << endl;
				std::cout << "|                         NIVEL " << level+1 << "                         |" << endl;
				std::cout << "|                                                         |" << endl;
				std::cout << "ñ---------------------------------------------------------ñ" << endl;

				thePlayer->SetPreCount(10);
			}
			else if (preCount % 30 == 0) { 
				std::cout << endl << "ñ---------------------------------------------------------ñ" << endl;
				std::cout << "|                                                         |" << endl;
				std::cout << "|                             " << preCount/30 << "                           |" << endl;
				std::cout << "|                                                         |" << endl;
				std::cout << "ñ---------------------------------------------------------ñ" << endl;				
			}
			preCount--;
		}

		//Mientras se juega
		if (continueGame) {			
			this->activeScene->Update(TIME_INCREMENT);
			this->lastUpdatedTime = currentTime.count() - this->initialMilliseconds.count();

			if (thePlayer->GetPreCount() != 0) {
				thePlayer->UpdatePreCount();
			}
			
			if (!preCount) {
				preCount = 50;
				if (ballCount < (level + 7) / 3 && rand() % 10 <= 7+level/5) {
					(enemies+ballCount)->SetPosition((enemies + ballCount)->InitPos());
					(enemies + ballCount)->SetIsAffectedByGravity(true);
					ballCount++;
				}
			}

			if (ballCount) {
				for (int i = 0; i < ballCount; i++) {
					//if (!((enemies + i)->GetReady())){						
						(enemies + i)->CheckFall();			
					//}
					(enemies + i)->UpdatePreCount();
					if ((enemies + i)->GetPreCount() == 0) {
						(enemies + i)->Move(2 + rand() % 1);
					}
				}
			}
			

			preCount--;

			if (thePlayer->GetPosition().GetX() <= -1 || thePlayer->GetPosition().GetZ() <= -1 || thePlayer->GetPosition().GetX() + thePlayer->GetPosition().GetZ() >= 7) {
				thePlayer->SetSpeed(Vector3D(0, -.3, 0));
				thePlayer->SetIsAffectedByGravity(true);
			}
			else if (!(thePlayer->GetSpeed() == Vector3D(0, 0, 0)) && ((abs(thePlayer->GetPosition().GetX() - thePlayer->GetPrevPosition().GetX()) >= 1) || abs(thePlayer->GetPosition().GetZ() - thePlayer->GetPrevPosition().GetZ()) >= 1)) {
				thePlayer->SetSpeed(Vector3D(0, 0, 0));
				thePlayer->SetPosition(Vector3D(roundf(thePlayer->GetPosition().GetX()), roundf(thePlayer->GetPosition().GetY()), roundf(thePlayer->GetPosition().GetZ())));

				for (int i = 0; i < MAPSIZE; i++) {
					if ((((pointerToBlocks + i)->GetPosition().GetX() == thePlayer->GetPosition().GetX()) && ((pointerToBlocks + i)->GetPosition().GetZ() == thePlayer->GetPosition().GetZ())) && !((pointerToBlocks + i)->GetColor() == *(blockColor2 + level))) {
						blockCounter++;
						cout << endl << "   * Llevas " << blockCounter << " bloques coloreados" << endl;
						(pointerToBlocks + i)->SetColor(*(blockColor2+level));
						if (blockCounter == MAPSIZE) {
							if (level == 6) {
								std::cout << endl << endl << "ñ---------------------------------------------------------ñ" << endl;
								std::cout << "|                                                         |" << endl;
								std::cout << "|                      HAS GANADO !!!!!!                  |" << endl;
								std::cout << "|                                                         |" << endl;
								std::cout << "ñ---------------------------------------------------------ñ" << endl;							
							}
							else {
								std::cout << endl << endl << "ñ---------------------------------------------------------ñ" << endl;
								std::cout << "|                                                         |" << endl;
								std::cout << "|                    HAS SUPERADO EL NIVEL                |" << endl;
								std::cout << "|                                                         |" << endl;
								std::cout << "ñ---------------------------------------------------------ñ" << endl;
								nextLevel = true;
								preCount = 90;
							}
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

			//Muerte del jugador
			if (thePlayer->GetPosition().GetY() <= -4) {
				thePlayer->LoseLife();

				if (thePlayer->GetLives() >= 0) {
					std::cout << endl << endl << "ñ---------------------------------------------------------ñ" << endl;
					std::cout << "|                                                         |" << endl;
					std::cout << "|                    Vidas restantes:" << thePlayer->GetLives() << "                    |" << endl;
					std::cout << "|                                                         |" << endl;
					std::cout << "ñ---------------------------------------------------------ñ" << endl;
				}
				else {
					continueGame = false;
					std::cout << endl << endl << "ñ---------------------------------------------------------ñ" << endl;
					std::cout << "|                                                         |" << endl;
					std::cout << "|                     FIN DEL JUEGO                       |" << endl;
					std::cout << "|                       CONTINUAR ?                       |" << endl;
					std::cout << "|                                                         |" << endl;
					std::cout << "ñ---------------------------------------------------------ñ" << endl;
					std::cout << endl << "   * Si deseas continuar presiona 1, de lo contrario presiona 0: ";
					std::cin >> continueGame;

					if (continueGame) {
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
	else if (!continueGame){
		this->activeScene->ProcessKeyPressed(key, xPosition, yPosition);
	}
}

void Game::ProcessMouseClick(const int& button, const int& state, const int& xPosition, const int& yPosition) 
{
	this->activeScene->ProcessMouseClick(button, state, xPosition, yPosition);
}
