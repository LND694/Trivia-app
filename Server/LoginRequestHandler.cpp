#include "LoginRequestHandler.h"

/// <summary>
/// C'tor of class LoginRequestHandler.
/// </summary>
LoginRequestHandler::LoginRequestHandler()
{
    //No fields yet
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

RequestResult& LoginRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    RequestResult* reqRes = new RequestResult();
    LoginResponse logResp;
    SignUpResopnse signUpResp;
    ErrorResopnse errResp;
    reqRes->newHandler = new LoginRequestHandler();

    //if the request is not relevent
    if (!isRequestRelevent(requestInfo))
    {

        errResp.message = ERROR_MSG;
        reqRes->response = JsonResponsePacketSerializer::serializeResponse(errResp);
    }
    else //the request is to login or to sign up
    {
        //The request is a sign up request
        if (SIGN_UP_REQS_CODE == requestInfo.id)
        {
            signUpResp.status = OK_STATUS_CODE;
            reqRes->response = JsonResponsePacketSerializer::serializeResponse(signUpResp);
        }
        else //The request is a login request
        {
            logResp.status = OK_STATUS_CODE;
            reqRes->response = JsonResponsePacketSerializer::serializeResponse(logResp);
        }
    }
    return *reqRes;
}
