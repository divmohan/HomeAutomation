#!/usr/bin/env python

import sys
import subprocess
import telepot
import time
import os
import socket
import sys


def handle(msg):
        chat_id = msg['chat']['id']
        command = msg['text']

        print 'Got command: %s' % command

        words = command.split()
        str = ' '.join(words[1:999])
        recvstr = str = ' '.join(words)
        
        #HOST, PORT = "localhost", 50007
        #socketHandle = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        #socketHandle.connect(("localhost", 50005))
        #socketHandle.send(command)
        s.send(recvstr)

TCP_IP   = '127.0.0.1'
TCP_PORT = 50014

BUFFER_SIZE = 100
MESSAGE = "14 nov Childrens day remind yearly"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.connect((TCP_IP, TCP_PORT))
#s.send(MESSAGE)

#data = s.recv(BUFFER_SIZE)

bot = telepot.Bot('322060273:AAHjszgt4w95TKgGMtpAN-I1Nbv7VA7-5Lg')
bot.message_loop(handle)
print 'I am listening ...'

while 1:
        time.sleep(10)
s.close()
