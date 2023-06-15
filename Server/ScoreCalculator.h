#pragma once

#include <cmath>

class ScoreClaculator
{
public:
	static int calculateScore(const int amountCorrectAnswers, const float averageTime);
	static float calculateAverageTime(const int lastAmountAnswers, const float lastAverageTime,
		const int newAmountAnswers, const float newAverageTime);
};
