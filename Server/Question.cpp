#include "Question.h"

/// <summary>
/// C'tr of class Question.
/// </summary>
/// <param name="question"> The question</param>
/// <param name="answers"> The answers for the question</param>
/// <param name="rightAnswer"> The rightAnswer for the question</param>
Question::Question(const string question, const deque<string>& answers, const unsigned int rightAnswer):
	m_question(question), m_answers(deque<string>()), m_rightAnswer(rightAnswer)
{
	if (this->m_rightAnswer >= AMOUNT_ANSWERS)
	{
		throw std::exception("This number can not be the right answer");
	}
	//Copying the answers to the object
	for (auto i = answers.begin(); i != answers.end(); i++)
	{
		this->m_answers.push_back(*i);
	}
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

deque<string>& Question::getAnswers() const
{
	deque<string>* answers = new deque<string>();

	//Copying the answers
	for (auto i = this->m_answers.begin(); i != this->m_answers.end();i++)
	{
		answers->push_back(*i);
	}
	return *answers;
}

unsigned int Question::getRightAnswer() const
{
	return this->m_rightAnswer;
}
