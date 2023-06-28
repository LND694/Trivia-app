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
        public const int CLOSE_RESP_CODE = 210;
        public const int START_GAME_RESP_CODE = 211;
        public const int LEAVE_ROOM_RESP_CODE = 212;
        public const int GET_ROOM_STATE_RESP_CODE = 213;
        public const int LEAVE_GAME_RESP_CODE = 214;
        public const int GET_QUESTION_RESP_CODE = 215;
        public const int SUBMIT_ANSWER_RESP_CODE = 216;
        public const int GET_GAME_RES_RESP_CODE = 217;
        public const int ADD_QUESTION_RESP_CODE = 218;
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
        private readonly Queue<string> statistics;

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
        private readonly Queue<string> players;

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
        [JsonProperty("Rooms")]
        private readonly Queue<RoomData> roomDatas;

        public GetRoomsResponse(int status, Queue<RoomData> rooms):
            base(status)
        {
            this.roomDatas = rooms;
        }

        public Queue<RoomData> GetRoomDatas()
        {
            return new Queue<RoomData>(this.roomDatas);
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

    public class CloseRoomResponse : ResponseWithStatus
    {
        public CloseRoomResponse(int status) : base(status)
        {

        }
    }


    public class StartGameResponse : ResponseWithStatus
    {
        public StartGameResponse(int status) : base(status)
        {

        }
    }
    public class LeaveRoomResponse : ResponseWithStatus
    {
        public LeaveRoomResponse(int status) : base(status)
        {

        }
    }

    public class GetRoomStateResponse : ResponseWithStatus
    {
        [JsonProperty("hasGameBegun")]
        private readonly bool hasGameBegun;


        [JsonProperty("players")]
        private readonly Queue<string> players;

        [JsonProperty("questionCount")]
        private readonly int questionCount;

        [JsonProperty("answerTimeOut")]
        private readonly int answerTimeOut;

        public GetRoomStateResponse(int status, bool hasGameBegun, Queue<string> players, int answerCount, int answerTimeOut) : base(status)
        {
            this.hasGameBegun = hasGameBegun;
            this.players = players;
            this.questionCount = answerCount;
            this.answerTimeOut = answerTimeOut;
        }

        public bool GetHasGameBegun()
        {
            return this.hasGameBegun;
        }
        public Queue<string> GetPlayers()
        {
            return new Queue<string>(this.players);
        }
        public int GetQuestionCount()
        {
            return this.questionCount;
        }
        public int GetAnswerTimeOut()
        {
            return this.answerTimeOut;
        }
    }

    public class GetGameResultsResponse : ResponseWithStatus
    {
        [JsonProperty("results")]
        private readonly Queue<PlayerResults> results;

        public GetGameResultsResponse(int status, Queue<PlayerResults> results):
            base(status)
        {
            this.results = new Queue<PlayerResults>(results);
        }

        public Queue<PlayerResults> GetPlayerResults()
        {
            return new Queue<PlayerResults>(this.results);
        }
    }

    public class LeaveGameResponse : ResponseWithStatus
    {
        public LeaveGameResponse(int status) :
            base(status)
        {

        }
    }

    public class SubmitAnswerResponse : ResponseWithStatus
    {
        [JsonProperty("correctAnswerId")]
        private readonly int correctAnswerId;

        public SubmitAnswerResponse(int status, int correctAnswerId) :
            base(status)
        {
            this.correctAnswerId = correctAnswerId;
        }

        public int GetCorrectAnswerId()
        {
            return this.correctAnswerId;
        }
    }

    public class GetQuestionResponse : ResponseWithStatus
    {
        [JsonProperty("question")]
        private readonly string question;

        [JsonProperty("answers")]
        private readonly Dictionary<int, string> answers;

        public GetQuestionResponse(int status, string question, Dictionary<int, string> answers):
            base(status)
        {
            this.question = question;
            this.answers = new Dictionary<int, string>(answers);
        }

        public string GetQuestion()
        {
            return this.question;
        }

        public Dictionary<int, string> GetAnswers()
        {
            return new Dictionary<int, string>(this.answers);
        }
    }

    public class AddQuestionResponse : ResponseWithStatus
    {
        public AddQuestionResponse(int status) :
            base(status)
        {

        }
    }

    public class ErrorResopnse : Response
    {
        [JsonProperty("message")]
        private readonly string message;


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
