#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <mutex>

#define ASCII_VALUE_0 48
#define NAME_KEY "username"
#define PASSOWRD_KEY "password"
#define EMAIL_KEY "email"
#define ADDRESS_KEY "address"
#define PHONE_NUM_KEY "phone"
#define BORN_DATE_KEY "date"
#define ROOM_ID "roomId"
#define ANSWER_TIMEOUT "answerTimeout"
#define MAX_USERS "maxUsers"
#define QUESTION_COUNT "questionCount"
#define ROOM_NAME "roomName"
#define ANSWER_ID "answerId"
#define QUESTION "question"
#define WRONG_ANSWERS "wrongAnswers"
#define RIGHT_ANSWER "rightAnswer"
#define DIFFICULTY "difficulty"

constexpr int SIZE_LENGTH_DATA_FIELD = 10;
constexpr int SIZE_CODE_FIELD = 3;

using std::vector;
using std::to_string;
using std::string;
using std::mutex;

enum RESPONSE_CODES {
	SIGN_UP_RESP_CODE = 201,
	LOGIN_RESP_CODE = 202,
	LOGOUT_RESP_CODE = 203,
	GET_ROOMS_RESP_CODE = 204,
	GET_PLAYERS_IN_ROOM_RESP_CODE = 205,
	JOIN_ROOM_RESP_CODE = 206,
	CREATE_ROOM_RESP_CODE = 207,
	GET_HIGH_SCORE_RESP_CODE = 208,
	GET_PERS_STATS_RESP_CODE = 209,
	CLOSE_RESP_CODE = 210,
	START_GAME_RESP_CODE = 211,
	LEAVE_ROOM_RESP_CODE = 212,
	GET_ROOM_STATE_RESP_CODE = 213,
	LEAVE_GAME_RESP_CODE = 214,
	GET_QUESTION_RESP_CODE = 215,
	SUBMIT_ANSWER_RESP_CODE = 216,
	GET_GAME_RES_RESP_CODE = 217,
	ADD_QUESTION_RESP_CODE = 218,
	ERROR_RESP_CODE = 255
};

enum REQUEST_CODES {
	SIGN_UP_REQS_CODE = 101,
	LOGIN_REQS_CODE = 102,
	LOGOUT_REQS_CODE = 103,
	GET_ROOMS_REQS_CODE = 104,
	GET_PLAYERS_IN_ROOM_REQS_CODE = 105,
	JOIN_ROOM_REQS_CODE = 106,
	CREATE_ROOM_REQS_CODE = 107,
	GET_HIGH_SCORE_REQS_CODE = 108,
	GET_PERS_STATS_REQS_CODE = 109,
	CLOSE_ROOM_REQS_CODE = 110,
	START_GAME_REQS_CODE = 111,
	GET_ROOM_STATE_REQS_CODE = 112,
	LEAVE_ROOM_REQS_CODE = 113,
	LEAVE_GAME_REQS_CODE = 114,
	GET_QUESTION_REQS_CODE = 115,
	SUBMIT_ANSWER_REQS_CODE = 116,
	GET_GAME_RESULT_REQS_CODE = 117,
	ADD_QUESTION_REQS_CODE = 118,
	ERROR_REQS_CODE = 255
};

typedef vector<unsigned char> Buffer;
typedef unsigned int GameId;
typedef REQUEST_CODES RequestId;
typedef mutex Lock;

