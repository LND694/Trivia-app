#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <WinSock2.h>

constexpr int SIZE_LENGTH_DATA_FIELD = 10;
constexpr int SIZE_CODE_FIELD = 10;

using std::vector;
using std::to_string;
using std::string;

typedef vector<unsigned char> Buffer;

enum RESPONSE_CODES{SIGN_UP_RESP_CODE = 201, LOGIN_RESP_CODE = 202, ERROR_RESP_CODE = 255};
