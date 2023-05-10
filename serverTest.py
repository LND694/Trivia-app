import socket
import json


def main():

    host = socket.gethostname()  # as both code is running on same pc
    MIN_PORT = 1024
    MAX_PORT = 65535
    SIZE_DATA_LENGTH_FIELD = 10
    SIGN_UP_REQS_CODE = 101
    LOGIN_REQS_CODE = 102
    port = 8265  # socket server port number
    email = ''

    if port < MIN_PORT or port > MAX_PORT:
        exit()

    client_socket = socket.socket()  # instantiate
    client_socket.connect((host, port))  # connect to the server

    login_or_sign_up = input("Hi client! Do you want to login(0) or to sign up(anything else)?\n")
    if login_or_sign_up == '0':
        code = LOGIN_REQS_CODE
    else:
        code = SIGN_UP_REQS_CODE
        email = input("Please enter your email: \n")

    username = input("Please enter your username:\n")
    password = input("Please enter your password:\n")

    data = {"username": username, "password": password}
    if login_or_sign_up != '0':
        data.update({"email": email})

    # Getting the length of the dictionary data
    len_data = 0
    for key, value in data.items():
        len_data += len(key) + len(value)

    # Padding the length of the data dictionary
    len_data = str(len_data)
    while len(len_data) < SIZE_DATA_LENGTH_FIELD:
        len_data = '0' + len_data

    # Serialize the dictionary into a formatted JSON string
    formatted_json = json.dumps(code) + json.dumps(len_data) + json.dumps(data, indent=4)

    answer = input("If you want to send the message, please click enter\n")
    
    client_socket.send(formatted_json.encode())
    data = client_socket.recv(1024).decode()
    print('THe message received from server: ' + data)  # show in terminal

    client_socket.close()  # close the connection


if __name__ == "__main__":
    main()
