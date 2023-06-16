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
	m_question(question), m_answers(answers), m_rightAnswer(rightAnswer), 
	m_category(category), m_difficulty(difficulty)
{
	if (answers.size() != AMOUNT_ANSWERS)
	{
		throw std::exception("The amount of incorrect answers should be " + AMOUNT_ANSWERS);
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
/// Copy c'tor of class Question
/// </summary>
/// <param name="other"></param>
Question::Question(const Question& other)
{
	this->m_question = other.m_question;
	this->m_category = other.m_category;
	this->m_difficulty = other.m_difficulty;
	this->m_rightAnswer = other.m_rightAnswer;

	//Copying the possibole answers
	for (auto i = other.m_answers.begin(); i != other.m_answers.end(); i++)
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

/// <summary>
/// The function gets the id of the correct answer.
/// </summary>
/// <returns> The id</returns>
unsigned int Question::getCorrectAnswerId() const
{
	unsigned int id = 0;

	//Going over the answers
	for (auto i = this->m_answers.begin(); i != this->m_answers.end(); i++)
	{
		if (*i == this->m_rightAnswer)
		{
			return id;
		}
		id++;
	}
	return id;
}

void Question::setAnswers(const vector<string>& newAnswers)
{
	this->m_answers.~vector();
	this->m_answers = vector<string>();

	//Copying the answers
	for (int i = 0; i < newAnswers.size(); i++)
	{
		this->m_answers.push_back(newAnswers[i]);
	}
}

void Question::setQuestion(const string newQuestion)
{
	this->m_question = newQuestion;
}

/// <summary>
/// The operator '==' checks if Questions are equal or not.
/// </summary>
/// <param name="other"> The other Question to compare to.</param>
/// <returns> If the Questions are equal or not.</returns>
bool Question::operator==(const Question& other) const
{
	return this->m_question == other.m_question;
}
