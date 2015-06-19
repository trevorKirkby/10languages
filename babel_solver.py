from babel_network import *
import time

connection = socket(1234)
connection.connect("localhost")

def solution1():
	connection.send("hello")
	print "sent 'hello'\n"
	time.sleep(1)
	feedback = connection.recv()
	print "recieved "+str(feedback)
	if feedback != "\"hello\"\n":
		print "error, server has not responded correctly"
	time.sleep(1)
	connection.send("goodbye")
	print "sent 'goodbye'\n"
	time.sleep(1)
	feedback = connection.recv()
	print "recieved "+str(feedback)
	if feedback != "\"yes\"\n":
		print "error, server has not responded correctly"
	else:
		print "solution successful"
