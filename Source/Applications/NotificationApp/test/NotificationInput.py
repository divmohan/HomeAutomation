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
        
        #HOST, PORT = "localhost", 50007
        socketHandle = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        socketHandle.connect(("localhost", 50005))
        socketHandle.send(command)
        
        
bot = telepot.Bot('322060273:AAHjszgt4w95TKgGMtpAN-I1Nbv7VA7-5Lg')
bot.message_loop(handle)
print 'I am listening ...'

while 1:
        time.sleep(10)

                                                                                                                  
