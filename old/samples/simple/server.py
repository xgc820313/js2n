import socket

HOST = ''                 # Symbolic name meaning the local host
PORT = 64446              # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
print "Waiting for incoming connection...";
conn, addr = s.accept()
print 'Incoming connection from ', addr
fs = conn.makefile()
for i in range(1,5):
	#data = conn.recv(1024)
	line = fs.readline()
	if line:
		print line.rstrip()
		conn.send("Hi There " + str(i) + "\n" )
conn.close()
s.close()