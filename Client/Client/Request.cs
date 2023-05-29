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
        public const int ERROR_REQS_CODE = 255;
    }
    public class LoginRequest : Request
    {
        private string username;
        private string password;

        public LoginRequest(string username, string password):
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
        private string email;
        private string address;
        private string phoneNum;
        private string bornDate;

        public SignupRequest(string username, string password, string email, string address, string phoneNum, string bornDate):
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
        private int roomId;

        public RequestWithRoomId(int roomId):
            base()
        {
            this.roomId = roomId;
        }
    }

    public class GetPlayersInRoomRequest : RequestWithRoomId
    {
        public GetPlayersInRoomRequest(int roomId):
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
        private string roomName;
        private int maxUsers;
        private int questionCount;
        private int answerTimeout;

        public CreateRoomRequest(string roomName, int maxUsers, int questionCount, int answerTimeout):
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
}
