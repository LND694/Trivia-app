using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    public class Constants
    {
        public const int OK_STATUS_CODE = 1;
        public const int ROOM_NOT_FOUND_ID = -1;
        public const int BUFFER_SIZE = 1024;
        public const int SIZE_CODE_FIELD = 3;
        public const int SIZE_LENGTH_DATA_FIELD = 10;
        public const int DATE_LENGTH = 10;
        public const char SPACE = ' ';
        public const char SEPERATOR = ',';
        public const char COLON = ':';
        public const char SEPERATOR_ARGS = '|';
        public const char NEW_LINE = '\n';
        public const char END_STR_SYMBOL = '\0';
        public const char ZERO_CHAR = '0';
        public const char END_JSON_SYMBOL= '}';
    }
}
