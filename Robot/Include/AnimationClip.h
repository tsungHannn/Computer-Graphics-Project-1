#pragma once

#include "BaseModelPart.h"
#include "AnimationCurve.h"

#include <string>
#include <map>
#include <fstream>
#include <string>
#include <sstream>

template<class TPart>
class AnimationClip
{
public:
	typedef void (*ApplyFunc)(unsigned int time, TPart* part, const std::string& prop, float value);
	typedef void (*ResetFunc)(unsigned int time, TPart* part);

	AnimationClip()
	{
		length = 0;
	}
	AnimationClip(TPart* parts, size_t partSize, ApplyFunc applyFunc, ResetFunc resetFunc)
	{
		AnimationClip<TPart>::parts = parts;
		AnimationClip<TPart>::partSize = partSize;
		AnimationClip<TPart>::applyFunc = applyFunc;
		AnimationClip<TPart>::resetFunc = resetFunc;
		length = 0;
	}

	void addAnimationCurve(unsigned int partId, std::string prop, AnimationCurve curve)
	{
		int curveLength = curve.getLength();
		length = length > curveLength ? length : curveLength;
		animationCurves[{partId, prop}] = curve;
	}

	void parseFile(char* fileName)
	{
		std::ifstream fs = std::ifstream(fileName, std::ios::in);
		if (fs.fail()) {
			printf("[Anim.Clip] Unable to parse from file, file %s cannot be opened for read.\n", fileName);
			return;
		}
		std::string line;
		while (std::getline(fs, line))
		{
			std::stringstream ss = std::stringstream(line);
			std::string cell;

			std::getline(ss, cell, ',');
			if (cell == "") {
				continue;
			}
			unsigned int time = atoi(cell.c_str());
			std::getline(ss, cell, ',');
			int partId = atoi(cell.c_str());
			std::getline(ss, cell, ',');
			std::string prop = cell;
			std::getline(ss, cell, ',');
			float value = float(atof(cell.c_str()));
			animationCurves[{partId, prop}].addKeyframe(time, value);
			length = length > time ? length : time;
		}
	}

	unsigned int getLength() {
		return length;
	}

	void update(unsigned int time)
	{
		for (size_t i = 0; i < partSize; i++) {
			resetFunc(time, &parts[i]);
		}
		for (auto iter = animationCurves.begin(); iter != animationCurves.end(); iter++) {
			AnimationCurve curve = iter->second;
			applyFunc(time, &parts[iter->first.first], iter->first.second, curve.evaluate(time));
		}
	}
private:
	TPart* parts;
	size_t partSize;
	std::map<std::pair<int, std::string>, AnimationCurve> animationCurves;
	ApplyFunc applyFunc;
	ResetFunc resetFunc;
	unsigned int length;
};
