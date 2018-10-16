#!/usr/bin/python

import cgi
import cgitb
import json
import socket

cgitb.enable()

HOST = '192.168.1.204'
PORT = 20060
RECV_LENGTH = 24

form = cgi.FieldStorage()
code = form.getfirst("code", "")

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
s.sendall(code+'\n')
status = s.recv(RECV_LENGTH)
s.close()

print "Content-Type: text/plain"
print
print status

