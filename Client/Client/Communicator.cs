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
        private Socket socket;
        static private IPAddress ipAddress = IPAddress.Parse("127.0.0.1"); // Replace with your desired IP address
        static private int port = 8265;

        /// <summary>
        /// connect to the server
        /// </summary>
        public void connect()
        {
            this.socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            // Specify the IP address and port number to connect
            // Connect to the server
            socket.Connect(ipAddress, port);

            if(socket.Connected)
            {
                Console.WriteLine("connection ssuccessful!");
            }
        }

        /// <summary>
        /// disconnect from the server
        /// </summary>
        public void disconnect()
        {
            if(this.socket.Connected)
            {
                this.socket.Disconnect(true);//the socket can be reused
            }
        }

        /// <summary>
        /// send a serialized request to the server
        /// </summary>
        public void sendRequestToServer()
        {
            byte[] buffer = File.ReadAllBytes("serialized.bin");
            socket.Send(buffer);
        }

    }
}
