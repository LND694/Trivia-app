#include "LoginRequestHandler.h"

/// <summary>
/// c`tor for login handler (currently empty)
/// </summary>
LoginRequestHandler::LoginRequestHandler()
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
/// The function checks a request  with the request info 
/// and handle it.
/// </summary>
/// <param name="requestInfo">The unformation about the request.</param>
/// <returns>a RequestInfo value- the results of the check</returns>
RequestResult& LoginRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
  
    if (requestInfo.id == SIGN_UP_REQS_CODE)
    {
        return signUp(requestInfo);//also handle it
    }
    return signIn(requestInfo);//also handle it
}

/// <summary>
/// the function  handles sign in request info and return the request result result
/// </summary>
/// <param name="requestInfo"> the info the client sent</param>
/// <returns> the result for the request</returns>
RequestResult& LoginRequestHandler::signIn(const RequestInfo& requestInfo)
{
    RequestResult* reqRes = new RequestResult();
    LoginRequest logReq;
    LoginResponse logResp;
    ErrorResopnse errResp;
    //if the request is not realevent
    if(!isRequestRelevent(requestInfo))
    {

        errResp.message = ERROR_MSG;
        reqRes->response = JsonResponsePacketSerializer::serializeResponse(errResp);//turn the error message into buffer
        reqRes->newHandler = this;//if there is a error the new handler will be the current
    }
    else
    {
        logReq = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);//get the details from the buffer
        this->m_handlerFactory->getLoginManager().login(logReq.username, logReq.password);//actual login
        logResp.status = OK_STATUS_CODE;
        reqRes->response = JsonResponsePacketSerializer::serializeResponse(logResp);//turn the response into buffer of the request result
        reqRes->newHandler = new MenuRequestHandler();//send a new menuHandler if the login was ssuccessful
    }
    return *reqRes;
}


/// <summary>
/// function that handles signUp requests
/// </summary>
/// <param name="requestInfo"> the info the client has sent</param>
/// <returns> the result of the given request</returns>
RequestResult& LoginRequestHandler::signUp(const RequestInfo& requestInfo)
{
    RequestResult* reqRes = new RequestResult();
    SignUpResopnse signUpResp;
    ErrorResopnse errResp;
    SignupRequest signUpReq;
    //if the request is not relevent
    if (!isRequestRelevent(requestInfo))
    {

        errResp.message = ERROR_MSG;
        reqRes->response = JsonResponsePacketSerializer::serializeResponse(errResp);
        reqRes->newHandler = this;//if there is a error the new handler will be the current
    }
    else
    {
        signUpReq = JsonRequestPacketDeserializer::desrializeSignupRequest(requestInfo.buffer);//get the sign up details to login manager
        this->m_handlerFactory->getLoginManager().signUp(signUpReq.email, signUpReq.password, signUpReq.username);//sign up (call database)
        signUpResp.status = OK_STATUS_CODE;
        reqRes->response = JsonResponsePacketSerializer::serializeResponse(signUpResp);//turn the response into buffer of the request result
        reqRes->newHandler = new MenuRequestHandler();//send a new menuHandler if the signUp was ssuccessful
    }
    return *reqRes;
}
