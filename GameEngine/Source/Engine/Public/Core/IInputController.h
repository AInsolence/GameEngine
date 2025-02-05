#pragma once

class IInputController
{
public:
	virtual ~IInputController() = default;

	virtual void HandleInput(GLfloat DeltaTime) = 0;
};

