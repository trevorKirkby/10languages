from babel_network import *
import time

connection = socket(1234)
connection.connect("localhost")

def solution1():
	connection.send("hello")
	print "sent 'hello'\n"
	feedback = connection.recv(False)
	print "recieved "+str(feedback)
	if feedback != "\"hello\"\n":
		print "error, server has not responded correctly"
	connection.send("goodbye")
	print "sent 'goodbye'\n"
	feedback = connection.recv(False)
	print "recieved "+str(feedback)
	if feedback != "\"yes\"\n":
		print "error, server has not responded correctly"
	else:
		print "solution successful"

def solution2():
	connection.send("histogram")
	print "sent 'hello'\n"
	feedback = connection.recv(False)
	print "recieved "+str(feedback)
	data = eval(feedback)
	array = data[1]
	answer = []
	for zero in range(data[0][0]):
		answer.append(0)
	incriment = data[0][1]
	for item in array:
		index = item/incriment
		answer[index] += 1
	connection.send(answer)
	print "sent '"+str(answer)+"'\n"
	feedback = connection.recv(False)
	print "recieved "+str(feedback)
	if feedback != "\"yes\"\n":
		print "error, server has not responded correctly"
	else:
		print "solution successful"