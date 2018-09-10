import sys
import subprocess
import telepot
import time
import os

def readUserInput(msg):
print 'Listening for input messages'

        chat_id = msg['chat']['id']
        command = msg['text']

        print 'Got command: %s' % command

        words = command.split()
        str = ' '.join(words[1:999])

bot = telepot.Bot('322060273:AAHjszgt4w95TKgGMtpAN-I1Nbv7VA7-5Lg')
bot.message_loop(handle)

#while 1:
 #       time.sleep(10)

                                                                                                                   1,9           Top
