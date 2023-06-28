using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    public class RoomData
    {
        [JsonProperty("id")]
        private int id;

        [JsonProperty("name")]
        private string name;

        [JsonProperty("maxPlayers")]
        private int maxPlayers;

        [JsonProperty("numOfQuestionsInGame")]
        private int numOfQuestionsInGame;

        [JsonProperty("timePerQuestion")]
        private int timePerQuestion;

        [JsonProperty("isActive")]
        private int isActive;

        public RoomData(int id, string name, int maxPlayers, int numOfQuestionsInGame, int timePerQuestion, int isActive)
        {
            this.id = id;
            this.name = name;
            this.maxPlayers = maxPlayers;
            this.numOfQuestionsInGame = numOfQuestionsInGame;
            this.timePerQuestion = timePerQuestion;
            this.isActive = isActive;
        }

        public int GetId()
        {
            return this.id;
        }
        public string GetName()
        {
            return this.name;
        }
        public int GetMaxPlayers()
        {
            return this.maxPlayers;
        }
        public int GetNumOfQuestionsInGame()
        {
            return this.numOfQuestionsInGame;
        }
        public int GetTimePerQuestion()
        {
            return this.timePerQuestion;
        }
        public int GetIsActive()
        {
            return this.isActive;
        }
    }
}
