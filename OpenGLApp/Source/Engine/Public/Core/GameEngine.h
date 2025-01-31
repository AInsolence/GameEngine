#pragma once

#include "World.h"

class GameEngine
{
public:

	static GameEngine& Get()
	{
		static GameEngine Engine;

		return Engine;
	}

	void Run();

private:
	GameEngine();

	void Start();
	void Stop();

	std::shared_ptr<World> WorldInstance;

	GLdouble DeltaTime = 0.0;
	GLdouble LastTime = 0.0;
};

