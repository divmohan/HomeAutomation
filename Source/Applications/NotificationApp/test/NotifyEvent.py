import sys 
import subprocess
import telepot
import time
import os

botChatId = 0
bot = 0

def sendMessage(message):
        print 'Got message: %s' % message
        print 'Got message: %d' % botChatId
        #bot = telepot.Bot('322060273:AAHjszgt4w95TKgGMtpAN-I1Nbv7VA7-5Lg')
        bot.sendMessage(botChatId,message)

def checkMessage():
        print 'Got chatId: %s' % botChatId

def getChatId(chatId, receivedBot):
        global botChatId 
        global bot
        bot = receivedBot  
        botChatId = chatId
        print 'Got chatId: %s' % botChatId
