using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    public class Request
    {
    }
    public class REQUEST_CODES
    {
        public const int SIGN_UP_REQS_CODE = 101;
        public const int LOGIN_REQS_CODE = 102;
        public const int LOGOUT_REQS_CODE = 103;
        public const int GET_ROOMS_REQS_CODE = 104;
        public const int GET_PLAYERS_IN_ROOM_REQS_CODE = 105;
        public const int JOIN_ROOM_REQS_CODE = 106;
        public const int CREATE_ROOM_REQS_CODE = 107;
        public const int GET_HIGH_SCORE_REQS_CODE = 108;
        public const int GET_PERS_STATS_REQS_CODE = 109;
        public const int CLOSE_ROOM_REQS_CODE = 110;
        public const int START_GAME_REQS_CODE = 111;
        public const int GET_ROOM_STATE_REQS_CODE = 112;
        public const int LEAVE_ROOM_REQS_CODE = 113;
        public const int LEAVE_GAME_REQS_CODE = 114;
        public const int GET_QUESTION_REQS_CODE = 115;
        public const int SUBMIT_ANSWER_REQS_CODE = 116;
        public const int GET_GAME_RESULT_REQS_CODE = 117;
        public const int ERROR_REQS_CODE = 255;
    }
    public class LoginRequest : Request
    {
        [JsonProperty("username")]
        private readonly string username;

        [JsonProperty("password")]
        private readonly string password;

        public LoginRequest(string username, string password) :
            base()
        {
            this.username = username;
            this.password = password;
        }

        public string GetUsername()
        {
            return this.username;
        }
        public string GetPassword()
        {
            return this.password;
        }
    }

    public class SignupRequest : LoginRequest
    {
        [JsonProperty("email")]
        private readonly string email;

        [JsonProperty("address")]
        private readonly string address;

        [JsonProperty("phone")]
        private readonly string phoneNum;

        [JsonProperty("date")]
        private readonly string bornDate;

        public SignupRequest(string username, string password, string email, string address, string phoneNum, string bornDate) :
            base(username, password)
        {
            this.email = email;
            this.address = address;
            this.phoneNum = phoneNum;
            this.bornDate = bornDate;
        }

        public string GetEmail()
        {
            return this.email;
        }
        public string GetAddress()
        {
            return this.address;
        }
        public string GetPhoneNum()
        {
            return this.phoneNum;
        }
        public string GetBornDate()
        {
            return this.bornDate;
        }
    }


    public class RequestWithRoomId : Request
    {
        [JsonProperty("roomId")]
        private readonly int roomId;

        public RequestWithRoomId(int roomId) :
            base()
        {
            this.roomId = roomId;
        }
    }

    public class GetPlayersInRoomRequest : RequestWithRoomId
    {
        public GetPlayersInRoomRequest(int roomId) :
            base(roomId)
        {

        }
    }

    public class JoinRoomRequest : RequestWithRoomId
    {
        public JoinRoomRequest(int roomId) :
            base(roomId)
        {

        }
    }


    public class CreateRoomRequest : Request
    {
        [JsonProperty("roomName")]
        private readonly string roomName;

        [JsonProperty("maxUsers")]
        private readonly int maxUsers;

        [JsonProperty("questionCount")]
        private readonly int questionCount;

        [JsonProperty("answerTimeout")]
        private readonly int answerTimeout;

        public CreateRoomRequest(string roomName, int maxUsers, int questionCount, int answerTimeout) :
            base()
        {
            this.roomName = roomName;
            this.maxUsers = maxUsers;
            this.questionCount = questionCount;
            this.answerTimeout = answerTimeout;
        }

        public string GetRoomName()
        {
            return this.roomName;
        }
        public int GetMaxUsers()
        {
            return this.maxUsers;
        }
        public int GetQuestionCount()
        {
            return this.questionCount;
        }
        public int GetAnswerTimeout()
        {
            return this.answerTimeout;
        }

    }
    public class SubmitAnswerRequest
    {
        [JsonProperty("answerId")]
        private int answerId;
        public SubmitAnswerRequest(int answerId)
        {
            this.answerId = answerId;
        }
    }
}
