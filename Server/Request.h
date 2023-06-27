#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

/// <summary>
/// struct for logins
/// </summary>
typedef struct LoginRequest {
	string username;
	string password;
}LoginRequest;

/// <summary>
/// struct for sign up
/// </summary>
typedef struct SignupRequest : LoginRequest {
	string email;
	string address;
	string phoneNum;
	string bornDate;

}SignupRequest;


typedef struct RequestWithRoomId
{
	unsigned int roomId;
}RequestWithRoomId;

typedef struct GetPlayersInRoomRequest : RequestWithRoomId
{

}GetPlayersInRoomRequest;

typedef struct JoinRoomRequest : RequestWithRoomId
{

}JoinRoomRequest;


typedef struct CreateRoomRequest
{
	string roomName;
	unsigned int maxUsers{};
	unsigned int questionCount{};
	unsigned int answerTimeout{};

}CreateRoomRequest;

typedef struct AddQuestionRequest
{
	vector<string> wrongAnswers;
	string question;
	string rightAnswer;
	string difficulty;
} AddQuestionRequest;


typedef struct SubmitAnswerRequest {
	unsigned int answerId;
}SubmitAnswerRequest;