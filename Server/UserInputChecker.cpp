#include "UserInputChecker.h"


/// <summary>
/// The functin checks if the password is legal and if
/// not so waht is not valid in its format.
/// </summary>
/// <param name="password">The password to check.</param>
/// <returns> The code of the result.</returns>
CHECK_PASSWORD_RESULTS UserInputChecker::doesPasswordLegal(const string password)
{
    if (!doesHaveRightLength(password, PASSWORD_RIGHT_LEN))
    {
        return LENGTH_PASSWORD_INVALID;
    }
    else if (!doesHaveCapitalLetter(password) || !doesHaveSmallLetter(password) || !doesHaveADigit(password))
    {
        return DOES_NOT_HAVE_LETTERS_OR_DIGITS;
    }
    else if (!doesHaveASpecialChar(password, SPECIAL_CHARS_PASSWORD))
    {
        return DOES_NOT_HAVE_SPECIAL_SYMBOL;
    }
    return PASSWORD_IS_LEGAL;
}

/// <summary>
/// The function checks if a string has a certain length or not.
/// </summary>
/// <param name="str"> The string to check</param>
/// <param name="len"> The len it should has.</param>
/// <returns> a bool value- if it has the right length or not.</returns>
bool UserInputChecker::doesHaveRightLength(const string str, const int len)
{
    return len == str.length();
}

/// <summary>
/// The function checks if a string has
/// at least 1 small letter.
/// </summary>
/// <param name="str"> The string to check</param>
/// <returns> a bool value- if it has or not.</returns>
bool UserInputChecker::doesHaveSmallLetter(const string str)
{
    regex regex("([a-z]+)");
    smatch matches;
    regex_search(str, matches, regex);
    return !matches.empty();
}

/// <summary>
/// The function checks if the string has at least 
/// 1 capital letter.
/// </summary>
/// <param name="str"> The string to check.</param>
/// <returns> if it has or not</returns>
bool UserInputChecker::doesHaveCapitalLetter(const string str)
{
    regex regex("[(A-Z)]+");
    smatch matches;
    regex_search(str, matches, regex);
    return !matches.empty();
}

/// <summary>
/// The function checks if the string has at least 
/// 1 digit.
/// </summary>
/// <param name="str"> The string to check.</param>
/// <returns> if it has or not</returns>
bool UserInputChecker::doesHaveADigit(const string str)
{
    regex regex("\\d");
    smatch matches;
    regex_search(str, matches, regex);
    return !matches.empty();
}

/// <summary>
/// The function checks if the string has
/// at least 1 from the special chars.
/// </summary>
/// <param name="str"> The string to check.</param>
/// <param name="specialChars"> The special characters as string</param>
/// <returns> if it ahs or not</returns>
bool UserInputChecker::doesHaveASpecialChar(const string str, const string specialChars)
{
    regex regex("([" + specialChars + "]+)");
    smatch matches;
    regex_search(str, matches, regex);
    return !matches.empty();
}

/// <summary>
/// The function checks if an email is legal or 
/// how it is not valid.
/// </summary>
/// <param name="email"> The email to check</param>
/// <returns> The result of the check</returns>
CHECK_EMAIL_RESULTS UserInputChecker::isLegalEmail(const string email)
{
    const vector<string> EMAIL_SUFFIXES = { "gmail.com", "walla.com", ".co.il", ".com", ".cyber.org.il" };
    string suffixEmail = "";
    if (!doesHaveJustOnceAChar(email, AT_SIGN))
    {
        return AT_SIGN_NOT_VALID_OR_EXIST;
    }

    //Taking the string after the AT_SIGN
    suffixEmail = email.substr(email.find(AT_SIGN), email.length() - 1);

    if (!doesHaveASuffix(suffixEmail, EMAIL_SUFFIXES))
    {
        return EMAIL_SUFFIX_NOT_VALID;
    }
    return EMAIL_IS_LEGAL;

}

/// <summary>
/// The function checks if a phone number is legal or 
/// how it is not valid.
/// </summary>
/// <param name="phoneNumber"> The email to check</param>
/// <returns> The result of the check</returns>
CHECK_PHONE_NUM_RESULTS UserInputChecker::isPhoneNumberLegal(const string phoneNumber)
{
    const vector<string> PHONE_PREFIXES = { "02", "04", "054", "052", "058", "050" };
    regex regex("");
    smatch matches;

    if (!doesHaveRightLength(phoneNumber, PHONE_NUM_RIGHT_LEN))
    {
        return LENGTH_PHONE_NUM_INVALID;
    }
    if (!doesHaveAPrefix(phoneNumber, PHONE_PREFIXES))
    {
        return INVALID_PREFIX_PHONE_NUM;
    }


    regex.assign("(\\d{2,3}-\\d{7,8})");
    regex_search(phoneNumber, matches, regex);
    if (matches.empty())
    {
        return WRONG_FORMAT_PHONE_NUM;
    }
    return PHONE_IS_LEGAL;
}

/// <summary>
/// THe function checks if a string has one of  
/// certain suffixes or not.
/// </summary>
/// <param name="str"> The string to scan</param>
/// <param name="suffixes"> The suffixes it should have</param>
/// <returns></returns>
bool UserInputChecker::doesHaveASuffix(const string str, const vector<string>& suffixes)
{
    regex regex("");
    smatch matches;

    //Going over the suffixes to find
    for (auto i = suffixes.begin(); i != suffixes.end(); i++)
    {
        regex.assign("((.)+" + *i + ")");
        regex_search(str, matches, regex);
        if (!matches.empty())
        {
            return true;
        }
    }
    return false;
}

/// <summary>
/// THe function checks if a string has one of  
/// certain prefixes or not.
/// </summary>
/// <param name="str"> The string to scan</param>
/// <param name="suffixes"> The prefixes it should have</param>
/// <returns></returns>
bool UserInputChecker::doesHaveAPrefix(const string str, const vector<string>& prefixes)
{
    regex regex("");
    smatch matches;

    //Going over the suffixes to find
    for (auto i = prefixes.begin(); i != prefixes.end(); i++)
    {
        regex.assign("(" + *i + "(.)+)");
        regex_search(str, matches, regex);
        if (!matches.empty())
        {
            return true;
        }
    }
    return false;
}

/// <summary>
/// The function checks if a string has only letters.
/// </summary>
/// <param name="str"> the string to check</param>
/// <returns> if has only letters or not.</returns>
bool UserInputChecker::doesHaveOnlyLetters(const string str)
{
    regex regex("([A-Za-z]*)");
    smatch matches;
    regex_search(str, matches, regex);
    return matches.begin()->str().length() == str.length();
}

/// <summary>
/// The function checks if a date is legal or
/// how it is not valid.
/// </summary>
/// <param name="date"> The date to check</param>
/// <returns> The result of the check</returns>
CHECK_DATE_RESULTS UserInputChecker::isDateValid(const string date)
{
    bool result = false;
    regex regex("(\\d{2}\\.\\d{2}\\.\\d{4})");
    smatch matches;
    regex_search(date, matches, regex);
    result = !matches.empty();
    regex.assign("(\\d{2}/\\d{2}/\\d{4})");
    regex_search(date, matches, regex);

    if (!result && matches.empty()) //both formats were not accomplished
    {
        return WRONG_FORMAT_DATE;
    }

    if (!isDateLegal(date)) //there is no such real date
    {
        return DATE_NOT_EXIST;
    }

    return DATE_IS_LEGAL;
}

/// <summary>
/// The function checks if an address is legal
/// how how it is not valid.
/// </summary>
/// <param name="address"> The address to check.</param>
/// <returns> The result of the check.</returns>
CHECK_ADDRESS_RESULTS UserInputChecker::isAddressValid(const string address)
{
    regex regex("([A-Za-z\\D]+,[\\d]+,[A-Za-z\\D]+)");
    smatch matches;
    regex_search(address, matches, regex);

    if (matches.empty() || !matches.suffix().str().empty() || !matches.prefix().str().empty())
    {
        return WRONG_FORMAT_ADDRESS;
    }

    return ADDRESS_IS_LEGAL;
}

CHECK_QUESTION_RESULTS UserInputChecker::isQuestionValid(const string question, const string difficultyQuestion)
{
    vector<string> suffixQuestion = {QUESTION_MARK};
    bool isDiffLegal = false;
    vector<string> questionWords = { "What", "Why", "Where", "When", "How", "Which", "Who", "Whose"};

    //if there is no question mark in the end of the question
    if (!doesHaveASuffix(question, suffixQuestion))
    {
        return NO_QUESTION_MARK;
    }

    //Going over the difficulties
    for (int i = 0; i < AMOUNT_DIFFS; i++)
    {
        if (DIFFICULTIES[i] == difficultyQuestion)
        {
            isDiffLegal = true;
        }
    }
    if (!isDiffLegal)
    {
        return INVALID_DIFFICULTY;
    }

    //if there are no question words in the start of the question
    if (!doesHaveAPrefix(question, questionWords))
    {
        return NO_QUESTION_WORD;
    }

    return QUESTION_IS_LEGAL;
}

/// <summary>
/// The function checks if a date really
/// exists.
/// </summary>
/// <param name="date"> The date to check</param>
/// <returns> if it exists or not</returns>
bool UserInputChecker::isDateLegal(const string date)
{
    regex regex("(\\d{2}).(\\d{2}).(\\d{4})");
    smatch matches;
    int day = 0, month = 0, year = 0;
    bool answer = false;
    time_t now = time(nullptr);
    tm currentDate = tm();
    tm* theDate = new tm();
    
    //Getting the date from the string
    regex_search(date, matches, regex);

    day = atoi(matches[1].str().c_str());
    month = atoi(matches[2].str().c_str());
    year = atoi(matches[3].str().c_str());

    theDate->tm_year = year - STARTING_YEAR;
    theDate->tm_mon = month - 1;
    theDate->tm_mday = day;

    localtime_s(&currentDate, &now);

    answer = std::mktime(&currentDate) >= std::mktime(theDate);

    delete theDate;
    return answer;
}

/// <summary>
/// The function checks if a string has only
/// 1 from a specific letter.
/// </summary>
/// <param name="str"> The string to check.</param>
/// <param name="character"> The unique character</param>
/// <returns> if the character is unique in the string or not.</returns>
bool UserInputChecker::doesHaveJustOnceAChar(const string str, const char character)
{
    string strRegex = "(" + string(1, character) + ")";
    regex regex(strRegex);
    smatch matches;
    regex_search(str, matches, regex);
    return matches.size() == 2 &&
        !doesHaveASpecialChar(matches.prefix().str(), string(1, character)) &&
        !doesHaveASpecialChar(matches.suffix().str(), string(1, character));
}

