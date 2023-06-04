#include "Question.h"

/// <summary>
/// C'tor of class Question.
/// </summary>
/// <param name="question"> The question</param>
/// <param name="answers"> The incorrect answers for the questions</param>
/// <param name="rightAnswer"> The right answer for the question</param>
/// <param name="category"> The category of the question</param>
/// <param name="difficulty"> The difficulty level of the question</param>
Question::Question(const string question, const vector<string>& answers,
	const string rightAnswer, const string category, const string difficulty):
	m_question(question), m_answers(vector<string>()), m_rightAnswer(rightAnswer), 
	m_category(category), m_difficulty(difficulty)
{
	if (answers.size() != AMOUNT_ANSWERS - 1)
	{
		throw std::exception("The amount of incorrect answers should be " + AMOUNT_ANSWERS - 1);
	}
	//Copying the answers to the object
	for (auto i = answers.begin(); i != answers.end(); i++)
	{
		this->m_answers.push_back(*i);
	}
}

/// <summary>
/// Default C'tor of class Question
/// </summary>
Question::Question():
	m_question(""), m_rightAnswer(""), m_answers(vector<string>()),
	m_category(""), m_difficulty("")
{
}

/// <summary>
/// D'tor of class Question.
/// </summary>
Question::~Question()
{
}

string Question::getQuestion() const
{
	return this->m_question;
}

vector<string>& Question::getAnswers() const
{
	vector<string>* answers = new vector<string>();

	//Copying the answers
	for (auto i = this->m_answers.begin(); i != this->m_answers.end();i++)
	{
		answers->push_back(*i);
	}
	return *answers;
}

string Question::getRightAnswer() const
{
	return this->m_rightAnswer;
}

string Question::getCategory() const
{
	return this->m_category;
}

string Question::getDifficulty() const
{
	return this->m_difficulty;
}
