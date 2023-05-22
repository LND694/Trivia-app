#include "IDatabase.h"

/// <summary>
/// function that gets questions from opentdb api
/// </summary>
/// <param name="amountOfQuestions">the amount of questions to get</param>
/// <returns> vector of questions </returns>
vector<Question>& IDatabase::fetchQuestions(const int amountOfQuestions)
{
    curlpp::Easy request;
    string apiUrl = "https://opentdb.com/api.php?amount=" + std::to_string(amountOfQuestions) + "&type=multiple";
    // Set the write callback function
    std::string response;
    // Set the URL
    request.setOpt(curlpp::options::Url(apiUrl));

    request.setOpt(curlpp::options::WriteFunction([&response](char* ptr, size_t size, size_t nmemb) {
        response.append(ptr, size * nmemb);
        return size * nmemb;
        }));
    // Perform the request
    request.perform();

    auto apiResponse = json::parse(response);//parse to json
    vector<Question>* questions = new vector<Question>();
    string question;
    string category;
    string correctAnswer;
    string difficulty;
    vector<string> incorrectAnswers;
    if (apiResponse["response_code"].get<int>() == 0) {
        for (const auto& result : apiResponse["results"])
        {
            category = result["category"].get<std::string>();
            question = result["question"].get<std::string>();
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

size_t IDatabase::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output)
{
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}
