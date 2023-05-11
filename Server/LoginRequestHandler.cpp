#include "LoginRequestHandler.h"







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

RequestResult& LoginRequestHandler::signIn(const RequestInfo& requestInfo)
{
    RequestResult* reqRes = new RequestResult();
    LoginRequest logReq;
    LoginResponse logResp;
    ErrorResopnse errResp;
    if(!isRequestRelevent(requestInfo))
    {

        errResp.message = ERROR_MSG;
        reqRes->response = JsonResponsePacketSerializer::serializeResponse(errResp);
        reqRes->newHandler = this;//if there is a error the new handler will be the current
    }
    else
    {
        logReq = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);
        this->m_handlerFactory->getLoginManager().login(logReq.username, logReq.password);
        logResp.status = OK_STATUS_CODE;
        reqRes->response = JsonResponsePacketSerializer::serializeResponse(logResp);//get the sign in details to login manager
        reqRes->newHandler = new MenuRequestHandler();
    }
    return *reqRes;
}

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
        reqRes->response = JsonResponsePacketSerializer::serializeResponse(signUpResp);
        reqRes->newHandler = new MenuRequestHandler();
    }
    return *reqRes;
}
