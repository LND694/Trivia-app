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
        static private readonly int port = 8265;
        static private readonly CryptoAlgoritm crypto = new RSAAlgorithm();
        static private string key;
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
            SendKey(key);
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
            socket.Send(crypto.Encrypt(data, key));
        }

        /// <summary>
        /// The function getts the response from the Server.
        /// </summary>
        /// <returns> The last message from the server.</returns>
        public string GetResponseFromServer()
        {
            string text;
            byte[] buffer = new byte[Constants.BUFFER_SIZE];
            socket.Receive(buffer);
            text = crypto.Decrypt(Encoding.ASCII.GetString(buffer), key);
            return text;
        }

        /// <summary>
        /// send the otp key to the server before encrypt the messages
        /// </summary>
        /// <param name="key"> the generated key </param>
        public void SendKey(string key)
        {
            socket.Send(Encoding.ASCII.GetBytes(key));
        }

    }
}
