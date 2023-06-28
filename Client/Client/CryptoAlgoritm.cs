using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    public abstract class CryptoAlgoritm
    {
        public abstract string Decrypt(byte[] message, byte[] key);
        public abstract byte[] Encrypt(string message, byte[] key);

        /// <summary>
        /// The funciton generates a key for encryption/decryption.
        /// </summary>
        /// <returns> The randomized key.</returns>
        public virtual byte[] GenerateKey()
        {
            // Create a byte array to hold the generated OTP key
            byte[] otpKey = new byte[OTPHelpers.BYTE_LENGTH];

            // Generate the OTP key using RNGCryptoServiceProvider
            using (RNGCryptoServiceProvider rngCsp = new RNGCryptoServiceProvider())
            {
                rngCsp.GetBytes(otpKey);
            }
            return otpKey;
        }
        public virtual byte[] GetIV()
        {
            return new byte[5];
        }
    }
}
