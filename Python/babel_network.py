import socket as basic_socket
import json
import select

class socket():
	def __init__(self,port):
		self.port = int(port)
		self.sock = basic_socket.socket(basic_socket.AF_INET, basic_socket.SOCK_STREAM)
		self.sock.setsockopt(basic_socket.SOL_SOCKET, basic_socket.SO_REUSEADDR, 1)
	def connect(self,host):
		try:
			self.host = str(host)
			self.sock.connect((self.host,self.port))
			return 1
		except Exception, e:
			return e
	def send(self,data):
		if socket == None:
			return 0
		try:
			data = json.dumps(data)
			data += "\n"
			self.sock.send(data)
			return 1
		except Exception, e:
			return e
	def recv(self,poll=True):
		if socket == None:
			return 0
		try:
			if poll:
				ready = select.select([self.sock], [], [], 0)
				if ready[0]:
					data = ""
					while data[-1:] != "\n":
						data += self.sock.recv(1)
					return data
				else:
					return None
			else:
				data = ""
				while data[-1:] != "\n":
					data += self.sock.recv(1)
				return data
		except Exception, e:
			return e