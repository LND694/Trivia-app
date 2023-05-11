#include "LoginRequestHandler.h"

/// <summary>
/// c`tor for login handler (currently empty)
/// </summary>
LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory reqHandFac):
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
    bool error = false;
    RequestResult* reqRes = new RequestResult();
    LoginRequest logReq;
    LoginResponse logResp;
    ErrorResopnse errResp;

    //if the request is not relevent
    if(!isRequestRelevent(requestInfo))
    {
        createErrorResponse(ERROR_MSG, reqRes);
    }
    else
    {
        logReq = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);//get the details from the buffer
        try
        {
            this->m_handlerFactory.getLoginManager().login(logReq.username, logReq.password);//actual login

        }
        catch (const std::exception& excp)
        {
            createErrorResponse(excp.what(), reqRes);
            error = true;
        }
        if (!error) //if there was no any error in the login process
        {
            logResp.status = OK_STATUS_CODE;
            reqRes->response = JsonResponsePacketSerializer::serializeResponse(logResp);//turn the response into buffer of the request result
            reqRes->newHandler = new MenuRequestHandler();//send a new menuHandler if the login was ssuccessful
        }
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
    bool error = false;
    RequestResult* reqRes = new RequestResult();
    SignUpResopnse signUpResp;
    ErrorResopnse errResp;
    SignupRequest signUpReq;
    //if the request is not relevent
    if (!isRequestRelevent(requestInfo))
    {
        createErrorResponse(ERROR_MSG, reqRes);
    }
    else
    {
        signUpReq = JsonRequestPacketDeserializer::desrializeSignupRequest(requestInfo.buffer);//get the sign up details to login manager
        try
        {
            this->m_handlerFactory.getLoginManager().signUp(signUpReq.email, signUpReq.password, signUpReq.username);//sign up (call database)
        }
        catch (const std::exception& excp)
        {
            createErrorResponse(excp.what(), reqRes);
            error = true;
        }
        if (!error) //if there was no any error in the sign up process
        {
            signUpResp.status = OK_STATUS_CODE;
            reqRes->response = JsonResponsePacketSerializer::serializeResponse(signUpResp);//turn the response into buffer of the request result
            reqRes->newHandler = new MenuRequestHandler();//send a new menuHandler if the signUp was successful
        }
    }
    return *reqRes;
}

/// <summary>
/// The function creates an ErrorResponse struct and saves it in
/// the RequestResult variable.
/// </summary>
/// <param name="errMsg"> The message of the error.</param>
/// <param name="reqRes"> The RequestResult to put  the ErrorResponse in.</param>
void LoginRequestHandler::createErrorResponse(const string errMsg, RequestResult* reqRes)
{
    ErrorResopnse errResp;
    errResp.message = errMsg;
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(errResp);//turn the error message into buffer
    reqRes->newHandler = this;//if there is a error the new handler will be the current
}
