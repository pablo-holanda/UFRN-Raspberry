import socket


TCP_IP = '10.4.5.123'
TCP_PORT = 56414
BUFFER_SIZE = 1024
MESSAGE = "10010110010"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
s.send(MESSAGE)
data = s.recv(BUFFER_SIZE)
s.close()

print "received data:", data