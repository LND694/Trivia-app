import socket
import json

def main():

    host = socket.gethostname()  # as both code is running on same pc
    MIN_PORT = 1024
    MAX_PORT = 65535
    MAX_LEN = 5
    port = 8265  # socket server port number

    if port < MIN_PORT or port > MAX_PORT:
        exit()

    client_socket = socket.socket()  # instantiate
    client_socket.connect((host, port))  # connect to the server


    # Create a dictionary representing the JSON structure
    data = {
     "status" : 102,
     "username": "LND",
        "password": "123"
    }
    # Serialize the dictionary into a formatted JSON string
    formatted_json = json.dumps(data, indent=4)

    answer = input("If you want to send the message, please click enter\n")
    
    client_socket.send(formatted_json.encode())
    data = client_socket.recv(1024).decode()
    print('Received from server: ' + data)  # show in terminal


    client_socket.close()  # close the connection


if __name__ == "__main__":
    main()
