using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Net;
using System.IO;

namespace Client
{
    internal class Communicator
    {
        private static Socket socket;
        static private readonly IPAddress ipAddress = IPAddress.Parse("127.0.0.1"); // Replace with your desired IP address
        static private readonly int  port = 8265;
        static private readonly CryptoAlgoritm crypto = new OTP();
        static private byte[] aesIV;//in case of aes
        static private byte[] key;
        /// <summary>
        /// connect to the server
        /// </summary>
        public void Connect()
        {
            socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            // Specify the IP address and port number to connect
            // Connect to the server
            socket.Connect(ipAddress, port);
            key = crypto.GenerateKey();
            aesIV = crypto.GetIV();
            SendEncryptionData(key);
            //System.Threading.Thread.Sleep(200);
            //SendEncryptionData(aesIV);
        }

        /// <summary>
        /// disconnect from the server
        /// </summary>
        public void Disconnect()
        {
            if(socket.Connected)
            {
                socket.Disconnect(true);//the socket can be reused
            }
        }

        /// <summary>
        /// The function sends some data to the server.
        /// </summary>
        /// <param name="data"> The data to send</param>
        public void SendRequestToServer(string data)
        {
            socket.Send(crypto.Encrypt(data,key));
        }

        /// <summary>
        /// The function getts the response from the Server.
        /// </summary>
        /// <returns> The last message from the server.</returns>
        public string GetResponseFromServer()
        {
            string text;
            byte[] buffer = new byte[Constants.BUFFER_SIZE];
            int len = socket.Receive(buffer);
            Array.Resize(ref buffer, len);//resize the buffer to its actual length and not 1024
            text = crypto.Decrypt(buffer, key);
            return text;
        }

        /// <summary>
        /// send the Encrpytion data such as key or IV
        /// </summary>
        /// <param name="data"> the encryption data to Send </param>
        public void SendEncryptionData(byte[] data)
        {
            socket.Send(data);
        }


    }
}
