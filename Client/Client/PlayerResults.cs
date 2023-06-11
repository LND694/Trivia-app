using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    public class PlayerResults
    {
        [JsonProperty("username")]
        private string username;

        [JsonProperty("correctAnswerCount")]
        private int correctAnswerCount;

        [JsonProperty("wrongAnswerCount")]
        private int wrongAnswerCount;

        [JsonProperty("averageAnswerTime")]
        private int averageAnswerTime;

        public PlayerResults(string username, int correctAnswerCount, int wrongAnswerCount, int averageAnswerTime)
        {
            this.username = username;
            this.correctAnswerCount = correctAnswerCount;
            this.wrongAnswerCount = wrongAnswerCount;
            this.averageAnswerTime = averageAnswerTime;
        }

        public string GetUsername()
        {
            return this.username;
        }

        public int GetCorrectAnswerCount()
        {
            return this.correctAnswerCount;
        }

        public int GetWrongAnswerCount()
        {
            return this.wrongAnswerCount;
        }

        public int GetAverageAnswerTime()
        {
            return this.averageAnswerTime;
        }
    }
}
