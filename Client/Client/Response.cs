using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;



namespace Client
{
    public class RESPONSE_CODES
    {
        public const int SIGN_UP_RESP_CODE = 201;
        public const int LOGIN_RESP_CODE = 202;
        public const int LOGOUT_RESP_CODE = 203;
        public const int GET_ROOMS_RESP_CODE = 204;
        public const int GET_PLAYERS_IN_ROOM_RESP_CODE = 205;
        public const int JOIN_ROOM_RESP_CODE = 206;
        public const int CREATE_ROOM_RESP_CODE = 207;
        public const int GET_HIGH_SCORE_RESP_CODE = 208;
        public const int GET_PERS_STATS_RESP_CODE = 209;
        public const int ERROR_RESP_CODE = 255;
    }

    //Base class
    public class Response
    {
        public Response()
        {

        }
    };

    //Base struct
    public class ResponseWithStatus : Response
    {
        [JsonProperty("status")]
        private int status;

        public ResponseWithStatus(int status):
            base()
        {
            this.status = status;
        }
        public int GetStatus()
        {
            return this.status;
        }
        public void SetStatus(int status)
        {
            this.status = status;
        }
    };

    public class ResponseWithStatistics : ResponseWithStatus
    {
        [JsonProperty("statistics")]
        private Queue<string> statistics;

        public ResponseWithStatistics(int status, Queue<string> statistics):
            base(status)
        {
            this.statistics = new Queue<string>(statistics);
        }
        public Queue<string> GetStatistics()
        {
            return new Queue<string>(this.statistics);
        }
    }

    public class ResponseWithPlayers : Response
    {
        private Queue<string> players;

        public ResponseWithPlayers(Queue<string> players):
            base()
        {
            this.players = players;
        }

        public Queue<string> GetPlayers()
        {
            return new Queue<string>(this.players);
        }
    }

    //Response structs
    public class LoginResponse : ResponseWithStatus
    {
        public LoginResponse(int status):
            base(status)
        {

        }
    }

    public class SignUpResponse : ResponseWithStatus
    {
        public SignUpResponse(int status) :
            base(status)
        {

        }
    }

    public class LogoutResponse : ResponseWithStatus
    {
        public LogoutResponse(int status) :
            base(status)
        {

        }
    }

    public class GetRoomsResponse : ResponseWithStatus
    {
        Queue<RoomData> rooms;

        public GetRoomsResponse(int status, Queue<RoomData> rooms):
            base(status)
        {
            this.rooms = rooms;
        }

        public Queue<RoomData> GetRoomDatas()
        {
            return new Queue<RoomData>(this.rooms);
        }
    }

    public class GetPlayersInRoomResponse : ResponseWithPlayers
    {
        public GetPlayersInRoomResponse(Queue<string> players):
            base(players)
        {

        }
    }

    public class GetHighScoreResponse : ResponseWithStatistics
    {
        public GetHighScoreResponse(int status, Queue<string> statistics):
            base(status, statistics)
        {

        }
    }

    public class GetPersonalStatsResponse : ResponseWithStatistics
    {
        public GetPersonalStatsResponse(int status, Queue<string> statistics) :
            base(status, statistics)
        {

        }
    }

    public class JoinRoomResponse : ResponseWithStatus
    {
        public JoinRoomResponse(int status) :
            base(status)
        {

        }
    }

    public class CreateRoomResponse : ResponseWithStatus
    {
        public CreateRoomResponse(int status) :
            base(status)
        {

        }
    }



    public class ErrorResopnse : Response
    {
        [JsonProperty("message")]
        private string message;


        public ErrorResopnse(string message):
            base()
        {
            this.message = message;
        }

        public string GetMessage()
        {
            return this.message;
        }
    }
}
