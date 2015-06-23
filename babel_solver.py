from babel_network import *
import math
import time
import gzip

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

def solution4():
	connection.send("plates")
	print "sent \"plates\"\n"
	feedback = connection.recv(False)
	print "recieved "+str(feedback)
	L1 = feedback[1].lower()
	L2 = feedback[2].lower()
	L3 = feedback[3].lower()
	f = gzip.open('wordlist.dat.gz','rb')
	dictionary = f.read()
	answer = ""
	solutions = {}
	for word in dictionary.split("\n"):
		points = 0
		firsts = [i for i, letter in enumerate(word) if letter == L1]
		if len(firsts):
			for first in firsts:
				remaining = word[first:]
				if remaining != word:
					points += 1
				remaining = remaining[1:]
				seconds = [i for i, letter in enumerate(remaining) if letter == L2]
				if len(seconds):
					for second in seconds:
						endsecond = first+1+second
						remaining2 = remaining[second:]
						if remaining2 != remaining:
							points += 1
						remaining2 = remaining2[1:]
						thirds = [i for i, letter in enumerate(remaining2) if letter == L3]
						if len(thirds):
							for third in thirds:
								endthird = first+1+second+1+third
								remaining3 = remaining2[third:]
								if remaining3 != remaining2:
									points += 1
								remaining3 = remaining3[1:]
								if remaining3 != "":
									points += 1
								solutions[points] = [word,first,endsecond,endthird]
								points = 0
						points = 0
				points = 0
	if len(solutions.keys()):
		data = solutions[max(solutions.keys())]
		answer = "".join(c.upper() if i in data[1:] else c for i, c in enumerate(data[0]))
		connection.send(answer)
		print "sent '"+str(answer)+"'\n"
		feedback = connection.recv(False)
		print "recieved "+str(feedback)
		if feedback != "\"yes\"\n":
			print "error, server has not responded correctly"
		else:
			print "solution successful"