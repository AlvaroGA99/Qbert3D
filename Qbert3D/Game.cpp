#include "Game.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Torus.h"
#include "Cuboid.h"
#include "Model.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

void Game::Create()
{
	Scene* mainScene = new(nothrow) Scene();
	FlyingCamera mainCamera = mainScene->GetCamera();
	mainCamera.SetPosition(Vector3D(-5.0f, 10.0f, 20.0f));
	mainCamera.SetOrientation(Vector3D(20.0f, 35.0f, 0.0f));
	mainScene->SetCamera(mainCamera);

	Cube* cubes = new(nothrow) Cube[8];
	for (int index = 0; index < 8; index++)
	{
		cubes[index] = Cube();
		cubes[index].SetColor(Color(0.1f, 0.2f, 0.8f));
		cubes[index].SetSize(0.2f);
		cubes[index].SetIsAffectedByGravity(false);
		cubes[index].SetOrientationSpeed(Vector3D(10.0f, 10.0f, 10.0f));
		mainScene->AddGameObject(cubes + index);
	}
	cubes[0].SetPosition(Vector3D(0.0f, 0.0f, 0.0f));
	cubes[1].SetPosition(Vector3D(0.0f, 0.0f, 10.0f));
	cubes[2].SetPosition(Vector3D(0.0f, 10.0f, 0.0f));
	cubes[3].SetPosition(Vector3D(0.0f, 10.0f, 10.0f));
	cubes[4].SetPosition(Vector3D(10.0f, 0.0f, 0.0f));
	cubes[5].SetPosition(Vector3D(10.0f, 0.0f, 10.0f));
	cubes[6].SetPosition(Vector3D(10.0f, 10.0f, 0.0f));
	cubes[7].SetPosition(Vector3D(10.0f, 10.0f, 10.0f));


	Cube* pointerToCubes = new(nothrow) Cube[4];
	if (pointerToCubes != nullptr) {
		for (int index = 0; index < 4; index++) {
			pointerToCubes[index] = Cube(
				Vector3D((0.2 + index * 0.1), (0.3 + index * 0.1), (0.1 + index * 0.1)),
				Color((0.2 + index * 0.2), (0.4 + index * 0.1), (0.7 + index * 0.1)),
				Vector3D((0.02 + index * 0.01), (0.2 + index * 0.01), (0.1 + index * 0.01)),
				0.1 + index * 0.1
			);
			pointerToCubes[index].SetOrientationSpeed(Vector3D(10.0f, 1.0f, 0.0f));
			mainScene->AddGameObject(pointerToCubes + index);
		}
	}

	Sphere* pointerToSphere = nullptr;


	pointerToSphere = new(nothrow) Sphere(
		Vector3D((0.3), (0.4), (0.9)),
		Color((0.8), (0.7), (0.2)),
		Vector3D((0.2), (0.3), (0.1)),
		Vector3D(),
		0.4
	);
	mainScene->AddGameObject(pointerToSphere);

	Cylinder* cylinder = new Cylinder(1.0f, 4.0f);
	cylinder->SetPosition(Vector3D(4.0f, 8.0f, 6.0f));
	cylinder->SetColor(Color(0.4f, 0.5f, 0.2f));
	cylinder->SetOrientationSpeed(Vector3D(1.0f, 0.0f, 0.0f));
	mainScene->AddGameObject(cylinder);

	Torus* torus = new Torus();
	torus->SetPosition(Vector3D(2.0f, 3.0f, 1.0f));
	torus->SetColor(Color(0.8f, 0.9f, 0.8f));
	torus->SetOrientationSpeed(Vector3D(0.0f, 6.0f, 0.0f));
	torus->SetIsAffectedByGravity(false);
	mainScene->AddGameObject(torus);

	Cuboid* back = new Cuboid();
	back->SetPosition(Vector3D(5.0f, 5.0f, -0.5f));
	back->SetColor(Color(0.9f, 0.6f, 0.5f));
	back->SetHeight(10.0f);
	back->SetLength(10.0f);
	back->SetWidth(1.0f);
	back->SetIsAffectedByGravity(false);
	mainScene->AddGameObject(back);

	Cuboid* floor = new Cuboid();
	floor->SetPosition(Vector3D(5.0f, -0.5f, 5.0f));
	floor->SetColor(Color(0.5f, 0.9f, 0.6f));
	floor->SetHeight(1.0f);
	floor->SetLength(10.0f);
	floor->SetWidth(10.0f);
	floor->SetIsAffectedByGravity(false);
	mainScene->AddGameObject(floor);

	Cuboid* right = new Cuboid();
	right->SetPosition(Vector3D(10.5f, 5.0f, 5.0f));
	right->SetColor(Color(0.6f, 0.5f, 0.9f));
	right->SetHeight(10.0f);
	right->SetLength(1.0f);
	right->SetWidth(10.0f);
	right->SetIsAffectedByGravity(false);
	mainScene->AddGameObject(right);

	Model* star = new Model();
	star->LoadModel("Star.obj");
	star->SetPosition(Vector3D(6, 8, 4));
	star->SetOrientationSpeed(Vector3D(0,1,0));
	star->SetColor(Color(0.8, 0.6, 0.1));
	star->SetIsAffectedByGravity(false);
	mainScene->AddGameObject(star);

	this->scenes.push_back(mainScene);
	this->activeScene = mainScene;
}

void Game::Create(const string& file) 
{
	ifstream inputFileStream(file);
	json jsonInfo;
	if (inputFileStream.is_open() && inputFileStream.good())
	{
		inputFileStream >> jsonInfo;

		cout << jsonInfo << endl;

		for (int idx = 0; idx < jsonInfo.at("scenes").size(); idx++)
		{
			json currentScene = jsonInfo.at("scenes").at(idx);

			Scene* scene = new Scene(Vector3D(currentScene.at("limit").at("x"), currentScene.at("limit").at("x"), currentScene.at("limit").at("x")));
			for (int idx2 = 0; idx2 < currentScene.at("objects").size(); idx2++)
			{
				json currentSolid = currentScene.at("objects").at(idx2);
				if (currentSolid.at("type") == "cube")
				{
					Cube* cube = new Cube();
					cube->SetPosition(Vector3D(currentSolid.at("position").at("x"), currentSolid.at("position").at("y"), currentSolid.at("position").at("z")));
					cube->SetSpeed(Vector3D(currentSolid.at("velocity").at("x"), currentSolid.at("velocity").at("y"), currentSolid.at("velocity").at("z")));
					cube->SetColor(Color(currentSolid.at("color").at("x"), currentSolid.at("color").at("y"), currentSolid.at("color").at("z")));
					cube->SetSize(currentSolid.at("size"));
					scene->AddGameObject(cube);
				}
			}
			this->scenes.push_back(scene);
		}
		this->activeScene = this->scenes[0];
	}
	else
	{
		cout << "Error leyendo archivo: " << file << endl;
	}
	
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
	this->activeScene->ProcessKeyPressed(key, xPosition, yPosition);
}

void Game::ProcessMouseClick(const int& button, const int& state, const int& xPosition, const int& yPosition) 
{
	this->activeScene->ProcessMouseClick(button, state, xPosition, yPosition);
}
