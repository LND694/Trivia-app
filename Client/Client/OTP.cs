using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;
namespace Client
{
    public class OTP : CryptoAlgoritm
    {
        public OTP():
            base()
        {

        }
        private const int byteLength = 200;
        public override byte[] Encrypt(string message, string key)
        {
            // Convert the plaintext and OTP key to byte arrays
            byte[] plainBytes = Encoding.ASCII.GetBytes(message);
            byte[] keyBytes = Encoding.ASCII.GetBytes(key);
            // Ensure that the key length matches the plaintext length
            if (plainBytes.Length > keyBytes.Length)
            {
                throw new Exception("Plaintext must be smaller or equal to OTP key");
            }

            // Perform XOR operation between the plaintext and OTP key
            byte[] encryptedBytes = new byte[plainBytes.Length];
            for (int i = 0; i < plainBytes.Length; i++)
            {
                encryptedBytes[i] = (byte)(plainBytes[i] ^ keyBytes[i]);
            }
            return encryptedBytes;
        }
        public override string Decrypt(string message, string key)
        {
            byte[] ciphertextBytes = Encoding.ASCII.GetBytes(message);
            byte[] keyBytes = Encoding.ASCII.GetBytes(key);

            byte[] decryptedBytes = new byte[ciphertextBytes.Length];

            for (int i = 0; i < ciphertextBytes.Length; i++)
            {
                decryptedBytes[i] = (byte)(ciphertextBytes[i] ^ keyBytes[i % keyBytes.Length]);
            }

            return Encoding.Default.GetString(decryptedBytes);
        }
    }
}
