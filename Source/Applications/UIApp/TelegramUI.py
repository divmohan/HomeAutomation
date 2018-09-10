import sys
import subprocess
import telepot
import time
import os

def handle(msg):
        chat_id = msg['chat']['id']
        command = msg['text']

        print 'Got command: %s' % command

        words = command.split()
        str = ' '.join(words[1:999])

        commands = set(('take', 'click', 'capture'))
        actions  = set(('photo', 'picture', 'snap'))


#!TODO:use dictionary here? req is for a set of lists. if it doesn match, get the meaning of words in each list and add the new word to the matching list

        filename = 'IMG_01' + '.jpg'

        if words[0] in commands and words[1] in actions:
            os.system('raspistill -o ' + filename)
         #   bot.sendMessage(chat_id, filename)
            img = open('IMG_01.jpg', 'rb')
            bot.sendPhoto(chat_id, img)
            img.close()
            bot.sendMessage(chat_id,"yayyyyyy!")

bot = telepot.Bot('322060273:AAHjszgt4w95TKgGMtpAN-I1Nbv7VA7-5Lg')
bot.message_loop(handle)
print 'I am listening ...'

while 1:
        time.sleep(10)

                                                                                                                   1,9           Top
