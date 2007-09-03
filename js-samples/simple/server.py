import socket
while 1:
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
		print "before readline"
		line = fs.readline()
		print "after readline"
		if not line: break
		print line.rstrip()
		conn.send("Hi There " + str(i) + "\n" )
	fs.close()
	conn.close()
	s.close()