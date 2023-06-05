#pragma once
#include "Room.h"

constexpr int OK_STATUS_CODE = 1;
const string ERROR_MSG = "ERROR";

//Base class
class Response
{

};

//Base struct
typedef struct ResponseWithStatus : Response
{
	unsigned int status;
} ResponseWithStatus;

typedef struct ResponseWithStatistics : Response
{
	vector<string> statistics;
} ResponseWithStatistics;

typedef struct ResponseWithPlayers : Response
{
	vector<string> players;
} ResponseWithPlayers;

//Response structs
typedef struct LoginResponse : ResponseWithStatus
{

} LoginResponse;

typedef struct SignUpResponse : ResponseWithStatus
{

} SignUpResponse;

typedef struct LogoutResponse : ResponseWithStatus
{

} LogoutResponse;

typedef struct GetRoomsResponse : ResponseWithStatus
{
	vector<RoomData> rooms;
} GetRoomsResponse;

typedef struct GetPlayersInRoomResponse : ResponseWithPlayers
{

} GetPlayersInRoomResponse;

typedef struct GetHighScoreResponse : ResponseWithStatus, ResponseWithStatistics
{

} GetHighScoreResponse;

typedef struct GetPersonalStatsResponse : ResponseWithStatus, ResponseWithStatistics
{

} GetPersonalStatsResponse;

typedef struct JoinRoomResponse : ResponseWithStatus 
{

} JoinRoomResponse;

typedef struct CreateRoomResponse : ResponseWithStatus
{

} CreateRoomResponse;



typedef struct ErrorResopnse : Response
{
	string message;
} ErrorResponse;



typedef struct CloseRoomResponse : ResponseWithStatus
{

}CloseRoomResponse;

typedef struct StartGameResponse : ResponseWithStatus
{

}StartGameResponse;

typedef struct LeaveRoomResponse : ResponseWithStatus
{

}LeaveRoomResponse;

typedef struct GetRoomStateResponse : ResponseWithStatus
{
	bool hasGameBegun;
	vector<string> players;
	unsigned int questionCount;
	unsigned int answerTimeOut;
}GetRoomStateResponse;