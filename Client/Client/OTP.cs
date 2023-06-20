using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;
namespace Client
{
    public class OTP
    {
        private const int byteLength = 511;
        public string GenerateKey()
        {
            // Create a byte array to hold the generated OTP key
            byte[] otpKey = new byte[byteLength];

            // Generate the OTP key using RNGCryptoServiceProvider
            using (RNGCryptoServiceProvider rngCsp = new RNGCryptoServiceProvider())
            {
                rngCsp.GetBytes(otpKey);
            }

            // Convert the byte array to a hexadecimal string representation
            string otpKeyHex = BitConverter.ToString(otpKey).Replace("-", "");
            return otpKeyHex;
        }
        public string Encrypt(string plainText, string otpKey)
        {
            // Convert the plaintext and OTP key to byte arrays
            byte[] plainBytes = Encoding.UTF8.GetBytes(plainText);
            byte[] keyBytes = Encoding.UTF8.GetBytes(otpKey);

            // Ensure that the key length matches the plaintext length
            if (plainBytes.Length != keyBytes.Length)
            {
                throw new ArgumentException("Plaintext and OTP key must have the same length.");
            }

            // Perform XOR operation between the plaintext and OTP key
            byte[] encryptedBytes = new byte[plainBytes.Length];
            for (int i = 0; i < plainBytes.Length; i++)
            {
                encryptedBytes[i] = (byte)(plainBytes[i] ^ keyBytes[i]);
            }

            // Convert the encrypted bytes to a base64 string
            string encryptedText = Convert.ToBase64String(encryptedBytes);
            return encryptedText;
        }

    }
}
