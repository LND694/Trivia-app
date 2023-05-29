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
        private int id;
        private string name;
        private int maxPlayers;
        private int numOfQuestionsInGame;
        private int timePerQuestion;
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
