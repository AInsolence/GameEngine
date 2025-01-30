#pragma once
#include <atomic>
#include <mutex>

#include "World.h"

class GameEngine
{
public:

	static GameEngine& GetInstance()
	{
		static GameEngine EngineInstance;

		return EngineInstance;
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

