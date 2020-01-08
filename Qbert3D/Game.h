#pragma once
#include "Scene.h"
#include "Enemy.h"
#include "Player.h"
#include "Block.h"
#include <chrono>
#include <fstream>
	
using namespace std::chrono;

class Game
{
private:
	const double TIME_INCREMENT = 0.4; //ms tiempo en el juego
	const long UPDATE_PERIOD = 10; //ms tiempo en el mundo real

	milliseconds initialMilliseconds;
	long lastUpdatedTime;

	Scene* activeScene;
	vector<Scene*> scenes;
	Player* thePlayer;
	Vector3D auxPos;
	Block* pointerToBlocks;
	int blockCounter;
	bool continueGame;
	bool nextLevel;
	int level;
	int preCount;
	Color* blockColor;
	Color* blockColor2;
	Enemy* enemies;
	int ballCount;
	int deadBall;
	bool ready;
	ofstream fs;
	ifstream fe;
public:

	Game() : blockCounter(0),auxPos(Vector3D()), pointerToBlocks(nullptr), thePlayer(nullptr), activeScene(nullptr), initialMilliseconds(duration_cast<milliseconds>(system_clock::now().time_since_epoch())), lastUpdatedTime(0) {}

	void Create();
	void Render();
	void Update();
	void ProcessMouseMovement(const int& xPosition, const int& yPosition);
	void ProcessKeyPressed(const unsigned char& key, const int& xPosition, const int& yPosition);
	void ProcessMouseClick(const int& button, const int& state, const int& xPosition, const int& yPosition);
};