using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace Client
{
    internal class JsonRequestPacketSerializer
    {
        public static string SerializeRequest<T>(T request, int reqCode)
        {
            string json = "", req = "" + reqCode;
            if(request == null) // only code request
            {
                req += GetPaddedNum(0, Constants.SIZE_LENGTH_DATA_FIELD);
            }
            else
            {
                json = JsonConvert.SerializeObject(request);
                req += GetPaddedNum(json.Length, Constants.SIZE_LENGTH_DATA_FIELD) + json;
            }
            return req;
        }
        private static string GetPaddedNum(int num, int length)
        {
            string numStr = "" + num;
            int initLen = numStr.Length;

            for(int i = initLen; i < length;i++)
            {
                numStr = Constants.ZERO_CHAR + numStr;
            }
            return numStr;
        }
    }
}
