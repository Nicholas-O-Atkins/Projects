from socket import *
import time

def fresh_head(code, file_type):
    h = ''
    if code == 200:
        h = 'HTTP:/1.1 200 OK\n'
    elif code == 404:
        h = 'HTTP:/1.1 404 Not Found\n'
    date = time.strftime("%a,%d,%b %Y %H:%M:%S", time.localtime())
    h += 'Date: ' + date + '\n'
    h += 'Server: COMP2006-Assignment3-HTTP-Server\n'
    """
    if file_type == 'html':
        h += 'Content-Type: text/html'
    if file_type == 'txt':
        h += 'Content-Type: text/plain'
    """
    h += 'Connection: close\n\n'
    return h


serverPort = 12000
serverSocket = socket(AF_INET, SOCK_STREAM)
serverAddress = ('localhost', serverPort)
print('starting up on {} port {}'.format(*serverAddress))
serverSocket.bind(serverAddress)
serverSocket.listen(1)
print('The server is ready to receive')
while 1:
    connectionSocket, addr = serverSocket.accept()
    sentence = connectionSocket.recv(1024).decode('utf-8')
    print(sentence)
    if(sentence.startswith('GET')):
        sentence = sentence.split('GET /')[1]
        sentence = sentence.split(' ')[0]
    print(sentence)
    if(sentence == ("index.html")):
        print("Here is the index.html")
        head = fresh_head(200, 'html')
        head = head.encode()
        opened = open('index.html', 'rb')
        red = opened.read()
        head += red
        connectionSocket.sendall(head)
    elif(sentence == ("index.txt")):
        print("Here is the index.txt")
        head = fresh_head(200, 'txt')
        head = head.encode()
        opened = open('index.txt', 'rb')
        red = opened.read(1024)
        head += red
        connectionSocket.sendall(head)
    elif(sentence == ("favicon.jpg")):
        print("Here is your pic")
        head = fresh_head(200, 'jpg')
        head = head.encode()
        opened = open('favicon.jpg', 'rb')
        red = opened.read()
        head += red
        connectionSocket.sendall(head)
    elif(sentence == ("404.jpg")):
        print("Here is your pic")
        head = fresh_head(200, 'jpg')
        head = head.encode()
        opened = open('404.jpg', 'rb')
        red = opened.read()
        head += red
        connectionSocket.sendall(head)
    else:
        print("Not the right response")
        print('The sentence is: ', sentence)
        head = fresh_head(404, 'html')
        head = head.encode()
        opened = open('404.html', 'rb')
        red = opened.read()
        head += red
        connectionSocket.sendall(head)
    connectionSocket.close()