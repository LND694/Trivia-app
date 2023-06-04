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
    }
}
