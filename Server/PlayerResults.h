#pragma once

#include <string>

using std::string;

typedef struct PlayerResults
{
	string username;
	unsigned int correctAnswerCount{};
	unsigned int wrongAnswerCount{};
	float averageAnswerTime{};
}PlayerResults;