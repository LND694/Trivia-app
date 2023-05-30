using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace Client
{
    public class JsonResponsePacketDeserializer
    {
        private static bool CheckIfErrorResp(string buffer)
        {
            return RESPONSE_CODES.ERROR_RESP_CODE == int.Parse(buffer.Substring(0, 3));
        }

        private static string GetDataFromBuffer(string buffer)
        {
            string data =  "";
            char currentChar = ' ';

            //Going over the string, starting from the data segment
            for (int i = Constants.SIZE_CODE_FIELD + Constants.SIZE_LENGTH_DATA_FIELD + 2; i < buffer.Length; i++)
            {
                currentChar = data[i];
                //if the character is not a letter or a scope
                if (currentChar != Constants.SPACE && currentChar != Constants.NEW_LINE && currentChar != Constants.END_STR_SYMBOL)
                {
                    data += currentChar;
                }
            }
            return data;
        }
        public static T DeserializeResponse<T>(string buffer)
        {
            string data = JsonResponsePacketDeserializer.GetDataFromBuffer(buffer);
            if(CheckIfErrorResp(buffer))
            {
                throw new Exception(JsonConvert.DeserializeObject<ErrorResopnse>(data).GetMessage());
            }
            return JsonConvert.DeserializeObject<T>(data);
        }
        public static ResponseWithStatistics DeserializeResponse(string buffer)
        {
            return new ResponseWithStatistics(buffer[9], GetQueueFromString(buffer.Substring(10, buffer.Length - 1)));
        }
        private static Queue<string> GetQueueFromString(string strQueue)
        {
            string[] array = strQueue.Split(Constants.SEPERATOR);
            Queue<string> queue = new Queue<string>();

            //Going over the array of the strings
            for(int i = 0; i < array.Length; i++)
            {
                queue.Enqueue(array[i]);
            }
            return queue;
        }
    }
}
