using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;
using System.IO;

namespace Client
{
    public class RSAAlgorithm : CryptoAlgoritm
    {
        private RSACryptoServiceProvider rsa;
        private RSAParameters privateKey;
        private RSAParameters publicKey;
        private const string DIRECTORY_PATH_SERVER_KEY = "C:/Keys/ServerKey.txt";
        private RSAParameters serverPublicKey;

        public RSAAlgorithm():
            base()
        {
            this.rsa = new RSACryptoServiceProvider();
            this.privateKey = rsa.ExportParameters(true);
            this.publicKey = rsa.ExportParameters(false);

            this.serverPublicKey = ReadPublicKeyFromFile(DIRECTORY_PATH_SERVER_KEY);
        }

        public override string Decrypt(string message, string key)
        {
            // Create an RSA object
            RSA rsa = RSA.Create();

            // Import the private key parameters
            rsa.ImportParameters(this.privateKey);

            // Decrypt the encrypted data
            byte[] decryptedData = rsa.Decrypt(Encoding.ASCII.GetBytes(message), RSAEncryptionPadding.OaepSHA256);

            // Convert the decrypted data to a string
            string decryptedText = Encoding.UTF8.GetString(decryptedData);

            return decryptedText;
        }

        public override byte[] Encrypt(string message, string key)
        {
            // Convert the message to bytes
            byte[] messageBytes = Encoding.UTF8.GetBytes(message);

            // Encrypt the message using RSA encryption
            byte[] encryptedBytes = this.rsa.Encrypt(messageBytes, true);

            return encryptedBytes;
        }

        public override string GenerateKey()
        {
            //Getting the string of the public key
            RSACryptoServiceProvider rsa = new RSACryptoServiceProvider();
            rsa.ImportParameters(this.publicKey);
            return rsa.ToXmlString(false);
        }

        private static RSAParameters ReadPublicKeyFromFile(string filePath)
        {
            try
            {
                // Read the contents of the public key file
                string publicKeyXml = File.ReadAllText(filePath);

                // Create an RSA object
                RSA rsa = RSA.Create();

                // Import the public key from the XML string
                rsa.FromXmlString(publicKeyXml);

                // Extract the RSA parameters
                RSAParameters publicKeyParams = rsa.ExportParameters(false);

                return publicKeyParams;
            }
            catch (Exception ex)
            {
                // Handle any exceptions
                Console.WriteLine("Error reading public key from file: " + ex.Message);
                return default(RSAParameters);
            }
        }
    }
}
