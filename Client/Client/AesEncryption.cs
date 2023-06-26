using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;
using System.IO;
using System.Windows.Forms;
using System.Runtime.CompilerServices;

namespace Client
{
    public class AesEncryption : CryptoAlgoritm
    {
        private readonly byte[] _iv;


        /// <summary>
        /// c`tor for aes encry[tion
        /// </summary>
        public AesEncryption()
        {
            using (AesCryptoServiceProvider aes = new AesCryptoServiceProvider())
            {
                aes.GenerateIV();
                this._iv = aes.IV;//set the iv
            }
        }

        /// <summary>
        /// generates aes key
        /// </summary>
        /// <returns> the kye in bytes array</returns>
        public override byte[] GenerateKey()
        {
            using (AesCryptoServiceProvider aes = new AesCryptoServiceProvider())
            {
                aes.KeySize = 256; // Set the key size to 256 bits (32 bytes)
                aes.GenerateKey();
                return aes.Key;
            }
        }
        /// <summary>
        /// encrypt given message with a key and iv
        /// </summary>
        /// <param name="plaintext"> the message to encrypt</param>
        /// <param name="key"> the encryption key </param>
        /// <returns> the encrypted message in bytes </returns>
        public override byte[]  Encrypt(string plaintext, byte[] key)
        {
            using (AesCryptoServiceProvider aes = new AesCryptoServiceProvider())
            {
                aes.Key = key;
                aes.IV = this._iv;
                aes.Mode = CipherMode.CBC;
                aes.Padding = PaddingMode.PKCS7; // Specify PKCS7 padding

                ICryptoTransform encryption = aes.CreateEncryptor(aes.Key, aes.IV);

                byte[] plaintextBytes = Encoding.Default.GetBytes(plaintext);

                byte[] ciphertext;

                using (MemoryStream ms = new MemoryStream())
                {
                    using (CryptoStream cs = new CryptoStream(ms, encryption, CryptoStreamMode.Write))
                    {
                        cs.Write(plaintextBytes, 0, plaintextBytes.Length);//write the crypto stream the plain text bytes
                    }
                    ciphertext = ms.ToArray();//write to the byte array
                }

                return ciphertext;
            }
        }

        /// <summary>
        /// decrypt given message
        /// </summary>
        /// <param name="ciphertext"> the message to decrypt</param>
        /// <param name="key"> the decryption key</param>
        /// <returns> the decrypted message as string</returns>
        public override string Decrypt(byte[] ciphertext, byte[] key)
        {
            using (AesCryptoServiceProvider aes = new AesCryptoServiceProvider())
            {
                aes.Key = key;
                aes.IV = this._iv;
                aes.Mode = CipherMode.CBC;
                aes.Padding = PaddingMode.PKCS7;

                ICryptoTransform decryption = aes.CreateDecryptor(aes.Key, aes.IV);

                byte[] plaintextBytes;

                using (MemoryStream ms = new MemoryStream())
                {
                    using (CryptoStream cs = new CryptoStream(ms, decryption, CryptoStreamMode.Write))
                    {
                        cs.Write(ciphertext, 0, ciphertext.Length);//write the ciphertext to the crypto stream
                    }

                    plaintextBytes = ms.ToArray();//convert the stream to bytes array
                }

                string plaintext = Encoding.UTF8.GetString(plaintextBytes);
                return plaintext;
            }
        }

        /// <summary>
        /// get the IV
        /// </summary>
        /// <returns> the iv</returns>
        public override byte[] GetIV()
        {
            return this._iv;
        }
    }
}
