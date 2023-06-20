#include "LoginRequestHandler.h"

/// <summary>
/// C`tor for login handler
/// </summary>
/// <param name="reqHandFac"> The fatory of the login request handler.</param>
LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory* reqHandFac):
    m_handlerFactory(reqHandFac)
{

}

/// <summary>
/// The function checks out if the Request
/// is relevent to the state of the user.
/// </summary>
/// <param name="requestInfo"> The info of the request to check.</param>
/// <returns> a bool value- if it is relevent or not</returns>
bool LoginRequestHandler::isRequestRelevent(const RequestInfo& requestInfo)
{
    return SIGN_UP_REQS_CODE == requestInfo.id || LOGIN_REQS_CODE == requestInfo.id;
}

/// <summary>
/// The function checks a request and handle it according to 
/// its RequestId.
/// </summary>
/// <param name="requestInfo">The information about the request.</param>
/// <returns>a RequestInfo value- the results of the check</returns>
RequestResult& LoginRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    //Handling the request
    if (requestInfo.id == SIGN_UP_REQS_CODE)
    {
        return signUp(requestInfo);
    }
    return login(requestInfo); //login code
}

/// <summary>
/// the function  handles sign in request info and return the request result result
/// </summary>
/// <param name="requestInfo"> the info the client sent</param>
/// <returns> the result for the request</returns>
RequestResult& LoginRequestHandler::login(const RequestInfo& requestInfo)
{
    RequestResult* reqRes = new RequestResult();
    LoginRequest logReq;
    LoginResponse logResp = LoginResponse();
    ErrorResopnse errResp;

    logReq = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);//get the details from the buffer
    this->m_handlerFactory->getLoginManager()->login(logReq.username, logReq.password);//actual login
    logResp.status = OK_STATUS_CODE;
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(logResp);//turn the response into buffer of the request result
    reqRes->newHandler = this->m_handlerFactory->createMenuRequestHandler(LoggedUser(logReq.username));//send a new menuHandler if the login was ssuccessful
    return *reqRes;
}


/// <summary>
/// function that handles signUp requests
/// </summary>
/// <param name="requestInfo"> the info the client has sent</param>
/// <returns> the result of the given request</returns>
RequestResult& LoginRequestHandler::signUp(const RequestInfo& requestInfo)
{
    string excpStr = "";
    RequestResult* reqRes = new RequestResult();
    SignUpResponse signUpResp = SignUpResponse();
    ErrorResopnse errResp;
    SignupRequest signUpReq;


    signUpReq = JsonRequestPacketDeserializer::desrializeSignupRequest(requestInfo.buffer);//get the sign up details to login manager
    //Checking the properties of the new user
    excpStr = LoginRequestHandler::checkIfInputValid(signUpReq);
    if (!excpStr.empty()) // there is an invalid input
    {
        throw std::exception(excpStr.c_str());
    }
    this->m_handlerFactory->getLoginManager()->signUp(signUpReq);//sign up (call database)
    signUpResp.status = OK_STATUS_CODE;
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(signUpResp);//turn the response into buffer of the request result
    reqRes->newHandler = this->m_handlerFactory->createMenuRequestHandler(LoggedUser(signUpReq.username));//send a new menuHandler if the login was ssuccessful
    return *reqRes;
}

/// <summary>
/// The function checks if the input of the signupRequest 
/// was valid or not.
/// </summary>
/// <param name="signupReq"> The signupRequest with the data to check.</param>
/// <returns> The string of the invalid data which was given.</returns>
string LoginRequestHandler::checkIfInputValid(const SignupRequest& signupReq)
{
    string excpStr = "";
    CHECK_PASSWORD_RESULTS passRes = UserInputChecker::doesPasswordLegal(signupReq.password);
    CHECK_EMAIL_RESULTS emailRes = UserInputChecker::isLegalEmail(signupReq.email);
    CHECK_PHONE_NUM_RESULTS phoneRes = UserInputChecker::isPhoneNumberLegal(signupReq.phoneNum);
    CHECK_ADDRESS_RESULTS addrRes = UserInputChecker::isAddressValid(signupReq.address);
    CHECK_DATE_RESULTS dateRes = UserInputChecker::isDateValid(signupReq.bornDate);

    //Password results checking
    switch (passRes)
    {
    case LENGTH_PASSWORD_INVALID:
        excpStr += "The length of the password should be " + to_string(PASSWORD_RIGHT_LEN) + ".\n";
        break;
    case DOES_NOT_HAVE_LETTERS_OR_DIGITS:
        excpStr += "The password should contain letters and digits.\n";
        break;
    case DOES_NOT_HAVE_SPECIAL_SYMBOL:
        excpStr += "The password should contain one of these symbols: {" + SPECIAL_CHARS_PASSWORD + "}.\n";
        break;
    }

    //Email results checking
    switch (emailRes)
    {
    case AT_SIGN_NOT_VALID_OR_EXIST:
        excpStr += "The email should contain the symbol at sign only once.\n";
        break;
    case EMAIL_SUFFIX_NOT_VALID:
        excpStr += "The suffix of the email is not recognized. Are you sure it exists?\n";
        break;
    }

    //Phone number results checking
    switch (phoneRes)
    {
    case LENGTH_PHONE_NUM_INVALID:
        excpStr += "The length of the phone should be " + to_string(PHONE_NUM_RIGHT_LEN -1) + ".\n";
        break;
    case INVALID_PREFIX_PHONE_NUM:
        excpStr += "The prefix of the phone is not recognized. Are you sure it exists?\n";
        break;
    case WRONG_FORMAT_PHONE_NUM:
        excpStr += "The format of phone number should be like this: {prefix - number}\n";
        break;
    }

    //Address results check
    switch (addrRes)
    {
    case WRONG_FORMAT_ADDRESS:
        excpStr += "The format of the address should be like this: (Street, Apartment, City). Apt can only has number.\n";
        break;
    }

    //Date results checking
    switch (dateRes)
    {
    case WRONG_FORMAT_DATE:
        excpStr += "The format of a born date should be like this: (DD.MM.YYYY), or with slash instead of  a point";
        break;
    case DATE_NOT_EXIST:
        excpStr += "This date does not exist. Are you sure you have born this day?\n";
        break;
    }

    return excpStr;
}
