#include "../Include/AnimationCurve.h"

AnimationCurve::AnimationCurve()
{
}

AnimationCurve::~AnimationCurve()
{
}

void AnimationCurve::addKeyframe(int time, float value)
{
	keyframes[time] = value;
}

float AnimationCurve::evaluate(int time)
{
	if (keyframes.size() == 0) {
		return 0.0f;
	}
	auto iter = keyframes.lower_bound(time);
	if (iter == keyframes.begin()) {
		return iter->second;
	}
	if (iter == keyframes.end()) {
		iter--;
		return iter->second;
	}
	int upperTime = iter->first;
	float upperVal = iter->second;
	iter--;
	int lowerTime = iter->first;
	float lowerVal = iter->second;

	int timeStep = upperTime - lowerTime;
	if (timeStep == 0) {
		timeStep = 1;
	}
	return lowerVal + ((upperVal - lowerVal) / (timeStep)) * (time - lowerTime);
}

int AnimationCurve::getLength()
{
	auto iter = keyframes.end();
	if (iter == keyframes.end()) {
		return 0;
	}
	return iter->first;
}
