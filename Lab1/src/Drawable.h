#pragma once

#include "Transform.h"

class Drawable
{
public:

	virtual const bool& IsActive() const
	{
		return false;
	}

	virtual void SetActive(const bool& value) = 0;
	virtual void Draw() = 0;
	virtual Transform& GetTransform() = 0;
};