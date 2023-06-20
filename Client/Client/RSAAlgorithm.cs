using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;
using System.IO;
using Org.BouncyCastle.Crypto;
using Org.BouncyCastle.OpenSsl;
using Org.BouncyCastle.Crypto.Parameters;
using Org.BouncyCastle.Security;

namespace Client
{
    public class RSAAlgorithm : CryptoAlgoritm
    {
        private RSAParameters privateKey;
        private RSAParameters publicKey;
        private const string DIRECTORY_PATH_SERVER_KEY = "C:/Keys/ServerKey.pem";
        private RSAParameters serverPublicKey;

        public RSAAlgorithm():
            base()
        {
            RSACryptoServiceProvider rsa = new RSACryptoServiceProvider();
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
            using (RSACryptoServiceProvider rsa = new RSACryptoServiceProvider())
            {
                rsa.ImportParameters(this.serverPublicKey);

                // Convert the message to bytes
                byte[] messageBytes = Encoding.UTF8.GetBytes(message);

                // Encrypt the message using RSA
                byte[] encryptedBytes = rsa.Encrypt(messageBytes, true);

                return encryptedBytes;
            }
        }

        public override string GenerateKey()
        {
            ////Getting the string of the public key
            //RSACryptoServiceProvider rsa = new RSACryptoServiceProvider();
            //rsa.ImportParameters(this.publicKey);
            //return rsa.ToXmlString(false);

            StringBuilder sb = new StringBuilder();

            // Add the PEM header
            sb.AppendLine("-----BEGIN PUBLIC KEY-----");

            // Encode the modulus and exponent in base64
            string modulusBase64 = Convert.ToBase64String(publicKey.Modulus);
            string exponentBase64 = Convert.ToBase64String(publicKey.Exponent);

            // Add the modulus and exponent lines
            sb.AppendLine(modulusBase64);
            sb.AppendLine(exponentBase64);

            // Add the PEM footer
            sb.AppendLine("-----END PUBLIC KEY-----");

            return sb.ToString();
        }

        private static RSAParameters ReadPublicKeyFromFile(string filePath)
        {
            string pemContents = File.ReadAllText(filePath);

            using (var stringReader = new StringReader(pemContents))
            {
                var pemReader = new PemReader(stringReader);
                var publicKeyObject = pemReader.ReadObject();

                if (publicKeyObject is RsaKeyParameters rsaKeyParameters)
                {
                    var rsaParameters = DotNetUtilities.ToRSAParameters(rsaKeyParameters);
                    return rsaParameters;
                }
                else
                {
                    throw new InvalidOperationException("Invalid RSA public key format.");
                }
            }
        }
    }
}
