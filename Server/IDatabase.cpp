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
    vector<string> answers;
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
    response = eraseSubString(response, SUBSTR1);
    response = eraseSubString(response, SUBSTR2);
    auto apiResponse = json::parse(response);//parse to json
    if (apiResponse["response_code"].get<int>() == 0) {
        for (const auto& result : apiResponse["results"])
        {
            category = result["category"].get<string>();
            question = result["question"].get<string>();
            correctAnswer = result["correct_answer"].get<string>();
            difficulty = result["difficulty"].get<string>();
            for (const auto& incorrectAnswer : result["incorrect_answers"]) {
                answers.push_back(incorrectAnswer.get<string>());
            }
            answers.push_back(correctAnswer);

            questions->push_back(Question(question, answers, correctAnswer, category, difficulty, false));
            answers.clear();//reset the incorrectAnswers
        }
    }
    else
    {
        throw std::exception("The request is invalid!");
    }
    return *questions;

}

/// <summary>
/// The function updates a StatisticsUser variable by his
/// new GameData.
/// </summary>
/// <param name="oldStats"> The old statistics of the user.</param>
/// <param name="newStats"> The new statistics of the last game
/// of the user.</param>
void IDatabase::updateStatistics(StatisticsUser& oldStats, const GameData& newStats)
{
    //Getting the old and the new scores
    int highScore = oldStats.getHighScore();
    int lastScore = ScoreClaculator::calculateScore(newStats.correctAnswerCount, newStats.averageAnswerTime);

    //Setting the averageAnswerTime
    oldStats.setAverageAnswerTime(ScoreClaculator::calculateAverageTime(oldStats.getAmountTotalAnswers(),
        oldStats.getAverageAnswerTime(),
        newStats.correctAnswerCount + newStats.wrongAnswerCount,
        newStats.averageAnswerTime));

    //Settng the amount of the games and the answers
    oldStats.setAmountGames(oldStats.getAmountGames() + 1);
    oldStats.setAmountCorrectAnswers(oldStats.getAmountCorrectAnswers() + newStats.correctAnswerCount);
    oldStats.setAmountTotalAnswers(oldStats.getAmountTotalAnswers() + newStats.correctAnswerCount + newStats.wrongAnswerCount);

    //Setting the high score(if the record was broken)
    if (highScore < lastScore)
    {
        oldStats.setHighScore(lastScore);
    }
}

/// <summary>
/// The function randomize the order of the vector
/// of the answers.
/// </summary>
/// <param name="answers"> The answers to randomize their order.</param>
/// <returns> The answers with new order.</returns>
vector<string>& IDatabase::randomizeOrderAnswers(const vector<string>& answers)
{
    vector<string>* randomizedAnswers = new vector<string>();
    random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, AMOUNT_ANSWERS - 1);
    int randomIndex = dist(rng);
    bool hasAnswerAlready = false; //intializing the variables

    //Going over the answers in the vector
    for (int i = 0; i < AMOUNT_ANSWERS; i++)
    {
        while (hasAnswerAlready)
        {
            hasAnswerAlready = false;

            //Randomizing the next index
            rng = std::mt19937(rd());
            dist = std::uniform_int_distribution<int>(0, AMOUNT_ANSWERS - 1);
            randomIndex = dist(rng);

            //Going over the randomized oreder questions
            for (auto i = randomizedAnswers->begin(); i != randomizedAnswers->end(); i++)
            {
                //If this question in this index is already in the new answers.
                if (answers[randomIndex] == *i)
                {
                    hasAnswerAlready = true;
                }
            }
        }
        randomizedAnswers->push_back(answers[randomIndex]);
        hasAnswerAlready = true;
    }

    return *randomizedAnswers;
}

/// <summary>
/// erase a certain substring from a string
/// </summary>
/// <param name="str"> a string</param>
/// <param name="substr"> the substring to find in the string</param>
/// <returns> string without the substring</returns>
string IDatabase::eraseSubString(string str, const string substr)
{
    std::size_t ind;
    ind = str.find(substr);
    while (ind != string::npos)
    {
        str.erase(ind, substr.length());
        ind = str.find(substr);
    }
    return str;
}

