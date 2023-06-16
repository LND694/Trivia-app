#pragma once

#define ID_FIELD "ID"
#define HIGH_SCORE_FIELD "HIGH_SCORE"
#define AMOUNT_GAMES_FIELD "AMOUNT_GAMES"
#define AMOUNT_TOTAL_ANSWERS_FIELD "TOTAL_AMOUNT_ANSWERS"
#define AMOUNT_CORRECT_ANSWERS_FIELD "AMOUNT_CORRECT_ANSWERS"
#define AVERAGE_TIME_FIELD "AVERAGE_ANSWER_TIME"

class StatisticsUser
{
private:
	//Fields
	int m_id;
	int m_amountGames;
	int m_amountTotalAnswers;
	int m_amountCorrectAnswers;
	float m_averageAnswerTime;
	int m_highScore;

public:
	StatisticsUser(const int id, const int amountGames, const int amountTotalAnswer,
		const int amountCorrectAnswers, const float averageAnswerTime, const int highScore);

	//Getters
	int getAmountGames() const;
	int getAmountTotalAnswers() const;
	int getAmountCorrectAnswers() const;
	float getAverageAnswerTime() const;
	int getHighScore() const;

	//Setters
	void setAmountGames(const int amountGames);
	void setAmountTotalAnswers(const int amountTotalAnswers);
	void setAmountCorrectAnswers(const int amountCorrectAnswers);
	void setAverageAnswerTime(const float averageAnswerTime);
	void setHighScore(const int highScore);
};