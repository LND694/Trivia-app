#include "StatisticsUser.h"

StatisticsUser::StatisticsUser(const int id, const int amountGames, const int amountTotalAnswer, const int amountCorrectAnswers, const float averageAnswerTime, const int highScore) :
	m_id(id), m_amountGames(amountGames), m_amountTotalAnswers(amountTotalAnswer),
	m_amountCorrectAnswers(amountCorrectAnswers), m_averageAnswerTime(averageAnswerTime), m_highScore(highScore)
{
}

int StatisticsUser::getAmountGames() const
{
	return this->m_amountGames;
}

int StatisticsUser::getAmountTotalAnswers() const
{
	return this->m_amountTotalAnswers;
}

int StatisticsUser::getAmountCorrectAnswers() const
{
	return this->m_amountCorrectAnswers;
}

float StatisticsUser::getAverageAnswerTime() const
{
	return this->m_averageAnswerTime;
}

int StatisticsUser::getHighScore() const
{
	return this->m_highScore;
}

void StatisticsUser::setAmountGames(const int amountGames)
{
	this->m_amountGames = amountGames;
}

void StatisticsUser::setAmountTotalAnswers(const int amountTotalAnswers)
{
	this->m_amountTotalAnswers = amountTotalAnswers;
}

void StatisticsUser::setAmountCorrectAnswers(const int amountCorrectAnswers)
{
	this->m_amountCorrectAnswers = amountCorrectAnswers;
}

void StatisticsUser::setAverageAnswerTime(const float averageAnswerTime)
{
	this->m_averageAnswerTime = averageAnswerTime;
}

void StatisticsUser::setHighScore(const int highScore)
{
	this->m_highScore = highScore;
}
