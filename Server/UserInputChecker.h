#pragma once

#include <regex>
#include <ctime>

using std::time_t;
using std::tm;

using std::string;
using std::vector;

using std::atoi;
using std::exception;

using std::regex;
using std::smatch;
using std::regex_search;

#define AT_SIGN '@' 
constexpr int PASSWORD_RIGHT_LEN = 8;
constexpr int PHONE_NUM_RIGHT_LEN = 11;
constexpr int STARTING_YEAR = 1900;
const string SPECIAL_CHARS_PASSWORD = "!@#$%^&*";
const string QUESTION_MARK = "?";

const string DIFFICULTIES[] = { "easy", "medium", "hard" };
constexpr int AMOUNT_DIFFS = 3;

enum CHECK_PASSWORD_RESULTS {
	PASSWORD_IS_LEGAL = 0,
	LENGTH_PASSWORD_INVALID = 1,
	DOES_NOT_HAVE_LETTERS_OR_DIGITS = 2,
	DOES_NOT_HAVE_SPECIAL_SYMBOL = 3
};

enum CHECK_EMAIL_RESULTS {
	EMAIL_IS_LEGAL = 0,
	AT_SIGN_NOT_VALID_OR_EXIST = 1,
	EMAIL_SUFFIX_NOT_VALID = 3
};

enum CHECK_PHONE_NUM_RESULTS {
	PHONE_IS_LEGAL = 0,
	LENGTH_PHONE_NUM_INVALID = 1,
	INVALID_PREFIX_PHONE_NUM = 2,
	WRONG_FORMAT_PHONE_NUM = 3
};

enum CHECK_DATE_RESULTS {
	DATE_IS_LEGAL = 0,
	WRONG_FORMAT_DATE = 1,
	DATE_NOT_EXIST = 2
};

enum CHECK_ADDRESS_RESULTS
{
	ADDRESS_IS_LEGAL = 0,
	WRONG_FORMAT_ADDRESS = 1
};

enum CHECK_QUESTION_RESULTS
{
	QUESTION_IS_LEGAL = 0,
	NO_QUESTION_MARK = 1,
	INVALID_DIFFICULTY = 2,
	NO_QUESTION_WORD = 3,
};

class UserInputChecker
{
public:
	//Functions
	static CHECK_PASSWORD_RESULTS doesPasswordLegal(const string password);
	static CHECK_EMAIL_RESULTS isLegalEmail(const string email);
	static CHECK_PHONE_NUM_RESULTS isPhoneNumberLegal(const string phoneNumber);
	static CHECK_DATE_RESULTS isDateValid(const string date);
	static CHECK_ADDRESS_RESULTS isAddressValid(const string address);
	static CHECK_QUESTION_RESULTS isQuestionValid(const string question, const string difficultyQuestion);

private:

	//Help functions
	static bool doesHaveRightLength(const string str, const int len);
	static bool doesHaveSmallLetter(const string str);
	static bool doesHaveCapitalLetter(const string str);
	static bool doesHaveADigit(const string str);
	static bool doesHaveASuffix(const string str, const vector<string>& suffixes);
	static bool doesHaveAPrefix(const string str, const vector<string>& prefixes);
	static bool doesHaveASpecialChar(const string str, const string specialChars);
	static bool doesHaveOnlyLetters(const string str);
	static bool doesHaveJustOnceAChar(const string str, const char character);
	static bool isDateLegal(const string date);
};