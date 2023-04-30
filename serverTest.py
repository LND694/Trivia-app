import socket
host = socket.gethostname()  # as both code is running on same pc
port = 8265  # socket server port number

if port < 1024 or port > 65535:
    exit()

client_socket = socket.socket()  # instantiate
client_socket.connect((host, port))  # connect to the server
data = client_socket.recv(1024).decode()
print('Received from server: ' + data)  # show in terminal
message = "hey boaz"
client_socket.send(message.encode())


client_socket.close()  # close the connection


