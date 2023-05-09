#pragma once

#include <iostream>
#include <string>
#include <vector>

#define ASCII_VALUE_0 48

constexpr int SIZE_LENGTH_DATA_FIELD = 10;

using std::vector;
using std::to_string;
using std::string;

enum RESPONSE_CODES{SIGN_UP_RESP_CODE = 201, LOGIN_RESP_CODE = 202, ERROR_RESP_CODE = 255};
enum REQUEST_CODES{SIGN_UP_REQS_CODE = 101, LOGIN_REQS_CODE = 102};

typedef vector<unsigned char> Buffer;
typedef REQUEST_CODES RequestId;
