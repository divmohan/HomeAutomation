import sys 
import subprocess
import telepot
import time
import os

def sendMessage(message):
        print 'Got message: %s' % message
        
        msgId = message.split(' ',1)[0]  
        print 'Received message Id: %s' % msgId

        #get the first 9 characters. that is the chat id
        ChatId = int (msgId[:9])
        print "Send Response to: %d" % ChatId

        msgIdLength = len(msgId)
        responseMessage = message[msgIdLength:]
     
        bot = telepot.Bot('322060273:AAHjszgt4w95TKgGMtpAN-I1Nbv7VA7-5Lg')
        bot.sendMessage(ChatId,responseMessage)

def checkMessage():
        print 'Got chatId: %s' % botChatId

def getChatId(chatId, receivedBot):
        global botChatId 
        global bot
        bot = receivedBot  
        botChatId = chatId
        print 'Got chatId: %s' % botChatId
