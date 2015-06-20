from babel_network import *
import math
import time

connection = socket(1234)
connection.connect("localhost")

def solution1():
	connection.send("hello")
	print "sent \"hello\"\n"
	feedback = connection.recv(False)
	print "recieved "+str(feedback)
	if feedback != "\"hello\"\n":
		print "error, server has not responded correctly"
	connection.send("goodbye")
	print "sent \"goodbye\"\n"
	feedback = connection.recv(False)
	print "recieved "+str(feedback)
	if feedback != "\"yes\"\n":
		print "error, server has not responded correctly"
	else:
		print "solution successful"

def solution2():
	connection.send("histogram")
	print "sent \"histogram\"\n"
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
	print "sent \""+str(answer)+"\"\n"
	feedback = connection.recv(False)
	print "recieved "+str(feedback)
	if feedback != "\"yes\"\n":
		print "error, server has not responded correctly"
	else:
		print "solution successful"

def dist(x,y):
	return math.sqrt(x*x+y*y)

def in_circle(point,circledata):
	if circledata[2] >= dist(point[0]-circledata[0],point[1]-circledata[1]):
		return True
	else:
		return False

def solution3():
	connection.send("circles")
	print "sent \"circles\"\n"
	feedback = connection.recv(False)
	print "recieved "+str(feedback)
	data = eval(feedback)
	outside = 0
	for coordx in range(1000):
		for coordy in range(1000):
			x = coordx/1000.
			y = coordy/1000.
			inacircle = False
			for circledat in data:
				if in_circle((x,y),circledat):
					inacircle = True
					break
			if not inacircle:
				outside += 1
	answer = outside/1000000.
	connection.send(answer)
	print "sent '"+str(answer)+"'\n"
	feedback = connection.recv(False)
	print "recieved "+str(feedback)
	if feedback != "\"yes\"\n":
		print "error, server has not responded correctly"
	else:
		print "solution successful"