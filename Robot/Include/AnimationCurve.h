#pragma once

#include "BaseModelPart.h"

#include <map>

class AnimationCurve
{
public:
	AnimationCurve();
	~AnimationCurve();

	void addKeyframe(int time, float value);
	float evaluate(int time);
	int getLength();
private:
	std::map<int, float> keyframes;
};
