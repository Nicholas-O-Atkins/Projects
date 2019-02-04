from socket import *

serverPort = 12000
serverSocket = socket(AF_INET, SOCK_STREAM)
serverAddress = ('', serverPort)
print('starting up on {} port {}'.format(*serverAddress))
serverSocket.bind(serverAddress)
serverSocket.listen(1)
print('The server is ready to receive')
while 1:
    connectionSocket, addr = serverSocket.accept()
    sentence = connectionSocket.recv(1024).decode('utf-8')
    sentence = sentence.split('host:12000')[1]
    print(sentence)
    if(sentence == ("/index.html")):
        print("Here is the index.html")
    elif(sentence == ("/index.txt")):
        print("Here is the index.txt")
    elif(sentence == ("/favicon.jpg")):
        print("Here is your pic")
    else:
        print("Not the right response")
    connectionSocket.sendall(bytes(sentence,'UTF-8'))
    connectionSocket.close()