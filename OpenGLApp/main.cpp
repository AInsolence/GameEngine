#include "Core/GameEngine.h"

int main()
{
	//// Render loop while (!MainWindow.GetShouldClose())

	GameEngine::GetInstance().Run();

	return 0;
}
