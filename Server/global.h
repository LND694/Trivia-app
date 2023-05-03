#pragma once

#include <iostream>
#include <vector>

#define MESSAGE_FIELD "message"

using std::vector;
using std::to_string;

typedef vector<unsigned char> Buffer;

enum RESPONSE_CODES{SIGN_UP_RESP_CODE = 201, LOGIN_RESP_CODE = 202, ERROR_RESP_CODE = 401};
