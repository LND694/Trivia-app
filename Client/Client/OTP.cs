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

        /// <summary>
        /// The function encryptes the message with the key.
        /// </summary>
        /// <param name="message"> the message to encrypt.</param>
        /// <param name="key"> The key to encrypt with.</param>
        /// <returns> The encrypted message.</returns>
        /// <exception cref="Exception"> The message must be smaller than
        /// the key, unless there is an exception.</exception>
        public override byte[] Encrypt(string message, byte[] key)
        {
            // Convert the plaintext and OTP key to byte arrays
            byte[] plainBytes = Encoding.UTF8.GetBytes(message);
            // Ensure that the key length matches the plaintext length
            if (plainBytes.Length > key.Length)
            {
                throw new Exception("Plaintext must be smaller or equal to OTP key");
            }

            // Perform XOR operation between the plaintext and OTP key
            byte[] encryptedBytes = new byte[plainBytes.Length];
            for (int i = 0; i < plainBytes.Length; i++)
            {
                encryptedBytes[i] = (byte)(plainBytes[i] ^ key[i]);
            }
            return encryptedBytes;
        }

        /// <summary>
        /// The function decryptes the message with the key.
        /// </summary>
        /// <param name="message"> the message to decrypt.</param>
        /// <param name="key"> The key to decrypt with.</param>
        /// <returns> The decrypted message.</returns>
        /// <exception cref="Exception"> The message must be smaller than
        /// the key, unless there is an exception.</exception>
        public override string Decrypt(byte[] message, byte[] key)
        {
            byte[] decryptedBytes = new byte[message.Length];

            for (int i = 0; i < message.Length; i++)
            {
                decryptedBytes[i] = (byte)(message[i] ^ key[i % key.Length]);
            }
            string text = Encoding.UTF8.GetString(decryptedBytes);
            return text;
        }
    }
}
