#pragma once
#include <string>
#include <vector>
#include <exception>

using std::string;
using std::vector;

constexpr unsigned int AMOUNT_ANSWERS = 4;

class Question
{
private:
	string m_question;
	string m_rightAnswer;
	string m_category;
	string m_difficulty;
	vector<string> m_answers;

public:
	//C'tor&D'tor
	Question(const string question, const vector<string>& answers, 
		const string rightAnswer, const string category, const string difficulty);
	Question();
	~Question();

	//Getters
	string getQuestion() const;
	vector<string>& getAnswers() const;
	string getRightAnswer() const;
	string getCategory() const;
	string getDifficulty() const;

};