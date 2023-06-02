using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms.VisualStyles;
using Newtonsoft.Json;

namespace Client
{
    public class JsonResponsePacketDeserializer
    {
        /// <summary>
        /// The functon checks if the buffer is an ErrorResponse Buffer
        /// </summary>
        /// <param name="buffer"> The buffer to check</param>
        /// <returns> if its code is a ERROR_RESP_CODE</returns>
        private static bool CheckIfErrorResp(string buffer)
        {
            return RESPONSE_CODES.ERROR_RESP_CODE == int.Parse(buffer.Substring(0, 3));
        }

        /// <summary>
        /// The function getts the Json data from the buffer.
        /// </summary>
        /// <param name="buffer"> The buffer with the json's data.</param>
        /// <returns> THe json data only</returns>
        private static string GetDataFromBuffer(string buffer)
        {
            string data =  "";
            char currentChar = ' ';

            //Going over the string, starting from the data segment
            for (int i = Constants.SIZE_CODE_FIELD + Constants.SIZE_LENGTH_DATA_FIELD; i < buffer.Length; i++)
            {
                currentChar = buffer[i];
                //if the character is not a letter or a scope
                if (currentChar != Constants.NEW_LINE && currentChar != Constants.END_STR_SYMBOL)
                {
                    data += currentChar;
                }
            }
            return data;
        }
        /// <summary>
        /// The function desialize a reponse's buffer
        /// </summary>
        /// <typeparam name="T"> The object which comes out from the 
        /// Json in the buffer</typeparam>
        /// <param name="buffer"> The message to deserialize</param>
        /// <returns> The object from the buffer with the data.</returns>
        /// <exception cref="Exception"> If this is an ErrorResponse- its string is thrown.</exception>
        public static T DeserializeResponse<T>(string buffer)
        {
            string data = JsonResponsePacketDeserializer.GetDataFromBuffer(buffer);
            if(CheckIfErrorResp(buffer))
            {
                throw new Exception(JsonConvert.DeserializeObject<ErrorResopnse>(data).GetMessage());
            }
            return JsonConvert.DeserializeObject<T>(data);
        }

        /// <summary>
        /// The function deserialize a RoomResponse.
        /// </summary>
        /// <param name="buffer"> The buffer which contains the RoomResponse</param>
        /// <returns> The object from it</returns>
        public static GetRoomsResponse DeserializeRoomResponse(string buffer)
        {
            int status = 0;
            RoomData theCurrentRoom = null;
            Queue<RoomData> roomDatas = new Queue<RoomData>();
            string[] partsBuffer = null;
            string[] roomDataFields = null;
            string statusField = "";

            string currentData = "";
            int id = 0;
            string name = "";
            int maxPlayers = 0, numOfQuestionsInGame = 0;
            int timePerQuestion = 0, isActive = 0;

            //if this is an ErrorResponse
            if (CheckIfErrorResp(buffer))
            {
                buffer = GetDataFromBuffer(buffer);
                throw new Exception(JsonConvert.DeserializeObject<ErrorResopnse>(buffer).GetMessage());
            }

            //Making the buffer for splitings
            buffer = RemoveSuffixBuffer(buffer);
            partsBuffer = buffer.Split(Constants.SEPERATOR_ARGS);

            //The first part has the status, which is just before the SEPERATOR
            statusField = partsBuffer[0].Split(Constants.SEPERATOR)[0];
            status = int.Parse(statusField[statusField.Length - 1] + "");

            //The roomDatas are in the rest of the parts if the buffer
            for(int i = 1; i < partsBuffer.Length;i++)
            {
                roomDataFields = partsBuffer[i].Split(Constants.SEPERATOR);

                //Going over the current RoomData's Fields
                for (int j = 0; j < roomDataFields.Length; j++)
                {
                    currentData = roomDataFields[j].Split(Constants.COLON)[1];
                    //According to its index, we know what data it is
                    switch (j)
                    {
                        case 0:
                            id = int.Parse(currentData);
                            break;
                        case 1:
                            name = RemoveFormatString(currentData);
                            break;
                        case 2:
                            maxPlayers = int.Parse(currentData);
                            break;
                        case 3:
                            numOfQuestionsInGame = int.Parse(currentData);
                            break;
                        case 4:
                            timePerQuestion = int.Parse(currentData);
                            break;
                        case 5:
                            isActive = int.Parse(currentData);
                            break;
                    }
                }
                theCurrentRoom = new RoomData(id, name, maxPlayers, numOfQuestionsInGame, timePerQuestion, isActive);
                roomDatas.Enqueue(theCurrentRoom);

            }
            return new GetRoomsResponse(status, roomDatas);
        }
        /// <summary>
        /// The function removes all the garbege letters after
        /// the end of the json data.
        /// </summary>
        /// <param name="buffer"> The buffer from the server.</param>
        /// <returns> The buffer only without garbege letters.</returns>
        private static string RemoveSuffixBuffer(string buffer)
        {
            int endJsonIndex = 0;

            //Finding the index where the end of the json is
            for(int i = 0; i < buffer.Length;i++)
            {
                if(buffer[i] == Constants.END_JSON_SYMBOL)
                {
                    endJsonIndex = i;
                }
            }
            return buffer.Substring(0, endJsonIndex - 1);
        }

        /// <summary>
        /// The function removes the format of
        /// the json's string(with ').
        /// </summary>
        /// <param name="str"> The string of the json</param>
        /// <returns> The string no as a Json format.</returns>
        private static string RemoveFormatString(string str)
        {
            return str.Substring(1, str.Length - 2);
        }
    }
}
