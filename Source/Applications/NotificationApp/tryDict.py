import sys
import telepot
import os
import socket
import time

chatIdTable = {"ChatId1":["ChatId11","ChatId12"]}

def AddToTable( chat_id):
   # print "ChatIdTable value",chatIdTable["ChatId1"]
   # chat_id = "ChatId1";

    if chat_id in chatIdTable:     
      listValue = len(chatIdTable[chat_id]) + 1
      value = str(chat_id) + str(listValue); 
      chatIdTable[chat_id].append(value)    
    else:
      # create a new array in this slot
      value = str(chat_id) + str('1'); 
      chatIdTable[chat_id] = [value]     
    print "ChatIdTable value",chatIdTable[chat_id]
    return value
  
def handle(msg):
        chat_id = msg['chat']['id']
        command = msg['text']

        print 'Got command: %s' % command

        chatValue = AddToTable( chat_id );

        words = command.split()
        str = ' '.join(words[1:999])
        recvstr = str = ' '.join(words)
        print "Received string: %s" % recvstr
        #Append chat id
        inputMsg = chatValue + ' ' + recvstr
        print "Received string: %s" % inputMsg
        
        s.send(inputMsg)

TCP_IP   = '127.0.0.1'
TCP_PORT = 50010

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.connect((TCP_IP, TCP_PORT))

bot = telepot.Bot('322060273:AAHjszgt4w95TKgGMtpAN-I1Nbv7VA7-5Lg')
bot.message_loop(handle)
print 'I am listening ...'

while 1:
        time.sleep(10)
s.close()
