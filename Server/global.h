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
	ERROR_RESP_CODE = 255};

enum REQUEST_CODES{SIGN_UP_REQS_CODE = 101, LOGIN_REQS_CODE = 102};

typedef vector<unsigned char> Buffer;
typedef REQUEST_CODES RequestId;
typedef mutex Lock;

