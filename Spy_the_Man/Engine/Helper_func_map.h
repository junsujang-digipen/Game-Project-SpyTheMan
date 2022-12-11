/*--------------------------------------------------------------
File Name: Helper_func_map.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 10/12/2021
-----------------------------------------------------------------*/
#pragma once
namespace helper {
	inline double map(double target, double min, double max, double targetMin, double targetMax) {
		if (target > max) {
			target = max;
		}
		if (target < min) {
			target = min;
		}
		double sum = (target - min) / (max - min) * (targetMax - targetMin) + targetMin;
		
		return sum;
	}

	
	inline glm::vec2 Lerp(glm::vec2 startPos,glm::vec2 endPos, float Alpha)
	{
		return startPos * (1.f - Alpha) + endPos * Alpha;
	}

	template<typename T>
	constexpr T Lerp(T startPos, T endPos, float Alpha)
	{
		return static_cast<T>(startPos * (1.f - Alpha) + endPos * Alpha);
	}

	template<typename T>
	constexpr T Lerp(T startPos, T endPos, double Alpha)
	{
		return static_cast<T>(startPos * (1. - Alpha) + endPos * Alpha);
	}
}