import socket
import json


def main():

    host = socket.gethostname()  # as both code is running on same pc
    MIN_PORT = 1024
    MAX_PORT = 65535
    SIZE_DATA_LENGTH_FIELD = 10
    SIGN_UP_REQS_CODE = 101
    LOGIN_REQS_CODE = 102
    EXIT = "exit"
    port = 8265  # socket server port number
    email = ''
    address = ''
    phone_num = ''
    born_date = ''
    answer = ''


    if port < MIN_PORT or port > MAX_PORT:
        exit()

    answer = input("If you want to send to exit write 'exit', everything else to continue\n")
    while(answer != EXIT):
        client_socket = socket.socket()  # instantiate
        client_socket.connect((host, port))  # connect to the server

        login_or_sign_up = input("Hi client! Do you want to login(0) or to sign up(anything else)?\n")
        if login_or_sign_up == '0':
            code = LOGIN_REQS_CODE
        else:
            code = SIGN_UP_REQS_CODE
            # Getting the information from the client about the user
            email = input("Please enter your email: \n")
            address = input("Please enter your address in this format-{street,apartment,city}:\n")
            phone_num = input("Please enter your phone number in this format-{prefix-num}:\n")
            born_date = input("Please enter your born date in this format-{DD.MM.YYYY} or with '/' instead of with '.':\n")

        # Getting the username and the password of the user
        username = input("Please enter your username:\n")
        password = input("Please enter your password:\n")


        # Buliding the json dictionary
        data = {"username": username, "password": password}
        if login_or_sign_up != '0': # Sign up
            data.update({"email": email, "address": address, "phone":phone_num, "date":born_date})

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

        
        client_socket.send(formatted_json.encode())
        data = client_socket.recv(1024).decode()
        print('The message received from server: ' + data)  # show in terminal
        answer = input("If you want to send to exit write 'exit', everything else to continue\n")

        client_socket.close()  # close the connection


if __name__ == "__main__":
    main()
