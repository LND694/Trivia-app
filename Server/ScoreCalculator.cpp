#include "ScoreCalculator.h"

/// <summary>
/// The function calculates the score of the user.
/// </summary>
/// <param name="amountCorrectAnswers"> The amount of his correct answers.</param>
/// <param name="averageTime"> His average time</param>
/// <returns>an integer value- his score</returns>
int ScoreClaculator::calculateScore(const int amountCorrectAnswers, const float averageTime)
{
    return roundf(amountCorrectAnswers * averageTime);
}

/// <summary>
/// The function calculates weighted average of the
/// average times.
/// </summary>
/// <param name="lastAmountAnswers"> The last amount of answers.</param>
/// <param name="lastAverageTime"> The last averageAnswerTime.</param>
/// <param name="newAmountAnswers"> The amount of the new answers.</param>
/// <param name="newAverageTime"> The average time of the new answers.</param>
/// <returns> The new average time.</returns>
float ScoreClaculator::calculateAverageTime(const int lastAmountAnswers, const float lastAverageTime, const int newAmountAnswers, const float newAverageTime)
{
    return ((lastAmountAnswers * lastAverageTime) + (newAmountAnswers * newAverageTime)) / (lastAmountAnswers + newAmountAnswers);
}
