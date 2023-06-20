#include "CryptoAlgorithm.h"

Buffer& CryptoAlgorithm::convertToBuffer(string str)
{
    Buffer* buf = new Buffer(str.begin(), str.end());
    return *buf;
}

string CryptoAlgorithm::convertToString(const Buffer& buf)
{
    string str = "";
    for (const auto& i : buf)
    {
        str += i;
    }
    return str;
}
