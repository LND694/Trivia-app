#pragma once
#include <string>
#include <deque>
#include <exception>

using std::string;
using std::deque;

constexpr unsigned int AMOUNT_ANSWERS = 4;

class Question
{
private:
	string m_question;
	deque<string> m_answers;
	unsigned int m_rightAnswer;

public:
	//C'tor&D'tor
	Question(const string question, const deque<string>& answers, const unsigned int rightAnswer);
	~Question();

	//Getters
	string getQuestion() const;
	deque<string>& getAnswers() const;
	unsigned int getRightAnswer() const;

};