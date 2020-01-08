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
	
	//Inicio de variables necesarias para el juego (bloques, enemigos, el jugador etc.)
	blockColor = new Color[6]{ Color((0.2), (0.4), (0.7)), Color((1), (0), (0)), Color((0.4), (0.7), (0.2)), Color((0.4), (0.2), (0.7)), Color((0.7), (0.12), (0.24)), Color((0.5), (.8), (0.55))};
	blockColor2 = new Color[6]{ Color(1, 0.5, 0), Color(0.5, 1, 0), Color((0.1), (0.2), (0.8)), Color((0.8), (0.5), (0.1)), Color((0.2), (0.62), (0.45)), Color((0.15), (.28), (0.855)) };
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
	mainScene->AddGameObject(this->enemies+2);
	this->scenes.push_back(mainScene);
	this->activeScene = mainScene;


	cout << endl << endl << "ñ---------------------------------------------------------ñ" << endl;
	cout << "|                         QBERT-3D                        |" << endl;				   
	cout << "ñ---------------------------------------------------------ñ" << endl;
	cout << "|                                                         |" << endl;
	cout << "| Juego desarrollado por:                                 |" << endl;
	cout << "|                                                         |" << endl;
	cout << "|   +  Alvaro Gonzalez Alcazar                            |" << endl;
	cout << "|   +  Alejandro Rodriguez Munoz                          |" << endl;
	cout << "|                                                         |" << endl;
	cout << "|                                                         |" << endl;
	cout << "ñ---------------------------------------------------------ñ" << endl << endl << endl;
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
				//Actualizamos todas las vairables antes del comienzo del nivel
				nextLevel = false;
				level++;
				blockCounter = 0;
				ballCount = 0;
				deadBall = 0;
				preCount = 51;
				thePlayer->SetIsAffectedByGravity(false);
				thePlayer->SetSpeed(Vector3D(0, 0, 0));
				thePlayer->SetPosition(Vector3D(0, 8, 0));
				
				//Coloreamos los bloques con el color del siguiente nivel
				for (int i = 0; i < MAPSIZE; i++) {					
					(pointerToBlocks + i)->SetColor(blockColor[level]);
				}
				continueGame = true;

				cout << endl << "ñ---------------------------------------------------------ñ" << endl;
				cout << "|                                                         |" << endl;
				cout << "|                         NIVEL " << level+1 << "                         |" << endl;
				cout << "|                                                         |" << endl;
				cout << "ñ---------------------------------------------------------ñ" << endl;

				thePlayer->SetPreCount(10);
			}

			//Se muestra una cuenta atrás
			else if (preCount % 30 == 0) { 
				cout << endl << "ñ---------------------------------------------------------ñ" << endl;
				cout << "|                                                         |" << endl;
				cout << "|                             " << preCount/30 << "                           |" << endl;
				cout << "|                                                         |" << endl;
				cout << "ñ---------------------------------------------------------ñ" << endl;				
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
				//Cada vez que preCount llega a 0 se genera una bola aleatoria con una probabilidad del 70% en el nivel 1 que va incrementando con cada nivel
				preCount = 50;
				if (ballCount < (level + 7) / 3 && rand() % 10 <= 7+level/5) {
					(enemies + deadBall)->SetPosition((enemies + deadBall)->InitPos());
					(enemies + deadBall)->SetSpeed(Vector3D(0, -.3, 0));
					(enemies + deadBall)->SetIsAffectedByGravity(true);
					(enemies + deadBall)->SetIsDead(false);
					ballCount++;
					deadBall++;					
				}
			}

			//Si hay bolas generadas pasamos a moverlas
			if (ballCount) {
				//Recorremos todas las bolas
				for (int i = 0; i < (level + 7) / 3; i++) {
					//Comprobamos que la bola no esté muerta
					if (!(enemies + i)->GetIsDead()) {
						//Si choca con el jugador este muere
						if ((enemies + i)->GetPosition() == thePlayer->GetPosition()) {
							thePlayer->SetPosition(Vector3D(-1, -1, -1));
							thePlayer->SetSpeed(Vector3D(0, -.6, 0));
							thePlayer->SetIsAffectedByGravity(true);
						}

						//Las bolas se generan encima de las casillas y caen sobre el mapa de juego por lo que debemos comprobar si han caído sobre el mapa
						if (!((enemies + i)->GetReady())) {
							(enemies + i)->SetSpeed(Vector3D(0, -.3, 0));
							(enemies + i)->SetIsAffectedByGravity(true);
							(enemies + i)->CheckFall();
						}//Una vez estén sobre el mapa podemos empezar a calcular su movimiento
						else {
							//No queremos que se muevan en cada update, queremos que antes de pasar a otra casilla se queden un momento en la casilla en la que están
							(enemies + i)->UpdatePreCount();

							//Cuando el contador llega a 0 hacemos que se dirija a otro bloque
							if ((enemies + i)->GetPreCount() == 0 && (enemies + i)->GetSpeed() == Vector3D(0, 0, 0)) {
								(enemies + i)->Move(2 + rand() % 2);
								(enemies + i)->SetPreCount(40 - level * 2);
							}

							//Si se salen del mapa se caen
							else if ((enemies + i)->GetPosition().GetX() <= -1 || (enemies + i)->GetPosition().GetZ() <= -1 || (enemies + i)->GetPosition().GetX() + (enemies + i)->GetPosition().GetZ() >= 7) {
								(enemies + i)->SetSpeed(Vector3D(0, -.3, 0));
								(enemies + i)->SetIsAffectedByGravity(true);
								(enemies + i)->SetReady(false);
								(enemies + i)->SetIsDead(true);								
								ballCount--;
								if (deadBall > i) {
									deadBall = i;
								}
								preCount = 50;
							}

							//Cuando llega a un nuevo cubo hacemos que se detenga y fijamos su posición sobre el bloque
							else if (((abs((enemies + i)->GetPosition().GetX() - (enemies + i)->GetPrevPosition().GetX()) >= 1) || abs((enemies + i)->GetPosition().GetZ() - (enemies + i)->GetPrevPosition().GetZ()) >= 1)) {
								(enemies + i)->SetSpeed(Vector3D(0, 0, 0));
								(enemies + i)->SetPosition(Vector3D(roundf((enemies + i)->GetPosition().GetX()), roundf((enemies + i)->GetPosition().GetY()), roundf((enemies + i)->GetPosition().GetZ())));
							}
						}
					}
				}
			}

			//Precount se actualiza en cada update hasta que llega a 0
			preCount--;

			//Si el jugador se sale del mapa se cae
			if (thePlayer->GetPosition().GetX() <= -1 || thePlayer->GetPosition().GetZ() <= -1 || thePlayer->GetPosition().GetX() + thePlayer->GetPosition().GetZ() >= 7) {
				thePlayer->SetSpeed(Vector3D(0, -.3, 0));
				thePlayer->SetIsAffectedByGravity(true);
			}
			//Si está sobre un bloque, detenemos su avance (hasta que presione una tecla)
			else if (!(thePlayer->GetSpeed() == Vector3D(0, 0, 0)) && ((abs(thePlayer->GetPosition().GetX() - thePlayer->GetPrevPosition().GetX()) >= 1) || abs(thePlayer->GetPosition().GetZ() - thePlayer->GetPrevPosition().GetZ()) >= 1)) {
				thePlayer->SetSpeed(Vector3D(0, 0, 0));
				thePlayer->SetPosition(Vector3D(roundf(thePlayer->GetPosition().GetX()), roundf(thePlayer->GetPosition().GetY()), roundf(thePlayer->GetPosition().GetZ())));

				//Comprobamos si está pisando un bloque que no ha coloreado y si lo está haciendo coloreamos dicho bloque y en caso de llegar a los 28 bloques del mapa avanzamos de nivel
				for (int i = 0; i < MAPSIZE; i++) {
					if ((((pointerToBlocks + i)->GetPosition().GetX() == thePlayer->GetPosition().GetX()) && ((pointerToBlocks + i)->GetPosition().GetZ() == thePlayer->GetPosition().GetZ())) && !((pointerToBlocks + i)->GetColor() == *(blockColor2 + level))) {
						blockCounter++;
						cout << endl << "   * Llevas " << blockCounter << " bloques coloreados" << endl;
						(pointerToBlocks + i)->SetColor(*(blockColor2+level));
						if (blockCounter == MAPSIZE) {
							for (int i = 0; i < (level + 7) / 3; i++) {
								(enemies + i)->SetPosition(Vector3D(0, 0, 0));
								(enemies + i)->SetIsAffectedByGravity(false);
								(enemies + i)->SetIsDead(true);
								(enemies + i)->SetReady(false);
								(enemies + i)->SetSpeed(Vector3D(0, 0, 0));
							}
							//Se gana si te pasas el último nivel
							if (level == 5) {
								cout << endl << endl << "ñ---------------------------------------------------------ñ" << endl;
								cout << "|                                                         |" << endl;
								cout << "|                      HAS GANADO !!!!!!                  |" << endl;
								cout << "|                                                         |" << endl;
								cout << "ñ---------------------------------------------------------ñ" << endl;							
							}
							else {
								cout << endl << endl << "ñ---------------------------------------------------------ñ" << endl;
								cout << "|                                                         |" << endl;
								cout << "|                    HAS SUPERADO EL NIVEL                |" << endl;
								cout << "|                                                         |" << endl;
								cout << "ñ---------------------------------------------------------ñ" << endl;
								nextLevel = true;
								preCount = 90;
							}
							continueGame = false;							
						}
					}
				}
			}

			//Muerte del jugador
			if (thePlayer->GetPosition().GetY() <= -4) {
				thePlayer->LoseLife();
				//Si te quedan vidas
				if (thePlayer->GetLives() > 0) {
					cout << endl << endl << "ñ---------------------------------------------------------ñ" << endl;
					cout << "|                                                         |" << endl;
					cout << "|                    Vidas restantes:" << thePlayer->GetLives() << "                    |" << endl;
					cout << "|                                                         |" << endl;
					cout << "ñ---------------------------------------------------------ñ" << endl;
				}
				//Cuando pierdes todas las vidas
				else {
					string s;

					//Procedemos a escribir las puntuaciones en el archivo ranking.txt
					fs.open("ranking.txt", ios::app);

					if (fs.is_open()) {
						fs << endl << "Nivel: " << level + 1 << endl;
						fs << "Numero de bloques: " << blockCounter << endl;
					}
					else {
						cerr << "Error de apertura del archivo de salida." << endl;
					}

					fs.close();

					//Al igualar esta variable a false se pausa el juego.
					continueGame = false;

					cout << endl << endl << "ñ---------------------------------------------------------ñ" << endl;
					cout << "|                                                         |" << endl;
					cout << "|                     FIN DEL JUEGO                       |" << endl;
					cout << "|                       CONTINUAR ?                       |" << endl;
					cout << "|                                                         |" << endl;
					cout << "ñ---------------------------------------------------------ñ" << endl;					

					//Leemos e imprimimos todas las puntuaciones almacenadas en el txt.
					cout << endl << endl << "ñ---------------------------------------------------------ñ" << endl;
					cout << "|                                                         |" << endl;
					cout << "|                      PUNTUACIONES                       |" << endl;					
					cout << "|                                                         |" << endl;
					cout << "ñ---------------------------------------------------------ñ" << endl;

					cout << "  * A continuacion vamos a mostrar todas las puntuaciones almacenadas." << endl << "  * La de esta partida se encuentra en el ultimo lugar." << endl << endl;

					fe.open("ranking.txt", ios::in);

					if (fe.is_open()) {
						getline(fe, s);

						while (!fe.eof()) {
							getline(fe, s);
							cout << s << endl;
						}
					}
					else {
						cerr << "Error de apertura del archivo de entrada." << endl;
					}

					cout << endl << "   * Si deseas continuar presiona 1, de lo contrario presiona 0: ";
					cin >> continueGame;

					if (continueGame) {
						//Seguir jugando
						preCount = 90;
						level = -1;
						nextLevel = true;
						continueGame = false;
						thePlayer->SetLives(3);
					}
					else {
						//Salir del juego
						cout << "   * Gracias por jugar a QBERT-3D.";
						exit(1);
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
	//Aquí se define el movimiento del jugador, el jugador sólo podrá moverse si continueGame está activado
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
		//default:
			//He desactivado el movimiento de cámara en la version final, para activarlo descomentar

			//this->activeScene->ProcessKeyPressed(key, xPosition, yPosition);
		}
		thePlayer->SetPreCount(10 + level / 2);
	}
	else if (!continueGame){
		//Descomentar para activar el movimiento de cámara con el juego en pausa

		//this->activeScene->ProcessKeyPressed(key, xPosition, yPosition);
	}
}

void Game::ProcessMouseClick(const int& button, const int& state, const int& xPosition, const int& yPosition) 
{
	this->activeScene->ProcessMouseClick(button, state, xPosition, yPosition);
}
