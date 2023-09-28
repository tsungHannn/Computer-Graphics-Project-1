#pragma once

#include "Common.h"
#include "BaseModelPart.h"
#include "AnimationClip.h"

#include <map>

template<class TPart, typename TState>
class Animation
{
public:
	Animation() {
		isPlaying = false;
		loop = false;
		time = 0;
		currentState = TState();
	}

	void addClip(TState state, AnimationClip<TPart> clip)
	{
		animationClips[state] = clip;
	}
	void removeClip(TState state) 
	{
		animationClips.erase(state);
	}

	void setState(TState state)
	{
		Animation::currentState = state;
		time = 0;

	}
	TState getState(TState state)
	{
		return Animation::currentState;
	}

	void play()
	{
		isPlaying = true;
	}
	void stop()
	{
		isPlaying = false;
	}
	void reset() {
		time = 0;
	}

	void setLoop(bool loop) {
		Animation::loop = loop;
	}

	void update(unsigned int dt)
	{
		if (isPlaying) {
			AnimationClip<TPart> clip = animationClips[currentState];
			time += dt;
			unsigned int clipLength = clip.getLength();
			if (clipLength < time) {
				if (loop && clipLength) {
					unsigned int newTime = time % clipLength;
					time = newTime;

				}
				else {
					time = clipLength;
					stop();
				}
			}
			clip.update(time);
		}
	}

private:
	bool isPlaying;
	bool loop;
	unsigned int time;
	TState currentState;
	std::map<TState, AnimationClip<TPart>> animationClips;
};
