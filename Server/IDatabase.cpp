#include "IDatabase.h"

/// <summary>
/// function that gets questions from opentdb api
/// </summary>
/// <param name="amountOfQuestions">the amount of questions to get</param>
/// <returns> vector of questions </returns>
vector<Question>& IDatabase::fetchQuestions(const int amountOfQuestions)
{
    vector<Question>* questions = new vector<Question>();
    string question;
    string category;
    string correctAnswer;
    string difficulty;
    string response;
    vector<string> incorrectAnswers;
    curlpp::Easy request;
 
    string apiUrl = OPENTDB_URL + std::to_string(amountOfQuestions) + "&type=multiple";
    // Set the write callback function
    // Set the URL
    request.setOpt(curlpp::options::Url(apiUrl));

    request.setOpt(curlpp::options::WriteFunction([&response](char* ptr, size_t size, size_t nmemb) {
        response.append(ptr, size * nmemb);
        return size * nmemb;
        }));
    // Perform the request
    request.perform();

    auto apiResponse = json::parse(response);//parse to json
    if (apiResponse["response_code"].get<int>() == 0) {
        for (const auto& result : apiResponse["results"])
        {
            category = result["category"].get<std::string>();
            question = result["question"].get<std::string>();
            question = eraseSubString(question, SUBSTR1);//erase common "&quot;"
            question = eraseSubString(question, SUBSTR2);//erase common "&#039;"
            correctAnswer = result["correct_answer"].get<std::string>();
            difficulty = result["difficulty"].get<std::string>();
            for (const auto& incorrectAnswer : result["incorrect_answers"]) {
                incorrectAnswers.push_back(incorrectAnswer.get<std::string>());
            }
            questions->push_back(Question(question, incorrectAnswers, correctAnswer, category, difficulty));
            incorrectAnswers.clear();//reset the incorrectAnswers
        }
    }
    else
    {
        throw std::exception("The request is invalid!");
    }
    return *questions;

}

string IDatabase::eraseSubString(string str, const string substr)
{
    std::size_t ind;
    ind = str.find(substr);
    while (ind != string::npos)
    {
        str.erase(ind, SUBSTR1.length());
        ind = str.find(SUBSTR1);
    }
    return str;
}

