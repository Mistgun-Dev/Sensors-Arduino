#!/usr/bin/env python2.7

import sys
import os
import serial
import numpy as np
from Quartz.CoreGraphics import CGEventCreateMouseEvent
from Quartz.CoreGraphics import CGEventPost
from Quartz.CoreGraphics import kCGEventMouseMoved
from Quartz.CoreGraphics import kCGEventLeftMouseDown
from Quartz.CoreGraphics import kCGEventLeftMouseDown
from Quartz.CoreGraphics import kCGEventLeftMouseUp
from Quartz.CoreGraphics import kCGMouseButtonLeft
from Quartz.CoreGraphics import kCGHIDEventTap


BUTTON_1   =  17
BUTTON_2   =  18
BUTTON_3   =  19
BUTTON_4   =  20
BUTTON_5   =  21
BUTTON_6   =  22
BUTTON_7   =  23
BUTTON_8   =  24
BUTTON_9   =  25
BUTTON_UP      =   0
BUTTON_RIGHT   =   2
BUTTON_DOWN    =   1
BUTTON_LEFT    =   3
BUTTON_MIDDLE  =   68

SHORTCUT_BUTTON_1 = "open -a safari http://www.google.com"
SHORTCUT_BUTTON_2 = "open -a mail"
SHORTCUT_BUTTON_3 = "open -a safari http://www.facebook.fr"
SHORTCUT_BUTTON_4 = "open -a safari https://www.univ-paris8.fr"
SHORTCUT_BUTTON_5 = "open -a safari http://www.google.com"
SHORTCUT_BUTTON_6 = "open -a safari http://www.google.com"
SHORTCUT_BUTTON_7 = "open -a safari http://www.google.com"
SHORTCUT_BUTTON_8 = "open -a safari http://www.google.com"
SHORTCUT_BUTTON_9 = "open -a safari http://www.google.com"

windowWidth = 1440
windowHeight = 900
mouseX = 0
mouseY = 0
serialport = serial.Serial("/dev/cu.usbmodem1421", 9600)
code = -1

def mouseEvent(type, posx, posy):
	theEvent = CGEventCreateMouseEvent(
	                                   None,
	                                   type,
	                                   (posx,posy),
	                                   kCGMouseButtonLeft)
	CGEventPost(kCGHIDEventTap, theEvent)

def mousemove(posx,posy):
	if(posx >= windowWidth):
		posx = windowWidth
	if(posx <= 0):
		posx = 0
	if(posy >= windowHeight):
		posy = windowHeight
	if(posy <= 0):
		posy = 0

	mouseEvent(kCGEventMouseMoved, posx,posy)

def mouseclick(posx,posy):
	# uncomment this line if you want to force the mouse
	# to MOVE to the click location first (I found it was not necessary).
	#mouseEvent(kCGEventMouseMoved, posx,posy);
	mouseEvent(kCGEventLeftMouseDown, posx,posy);
	mouseEvent(kCGEventLeftMouseUp, posx,posy);


speedMouse = 7

while 1:
	while serialport.in_waiting:  # Or: while ser.inWaiting():
		command = serialport.readline()
		

		if(len(command) >= 3):

			try:
				code = int(command)
			except:
				print command
			
			if(code != -1):

				if(code == BUTTON_1):
					print "Lancement de Google.com"
					os.system(SHORTCUT_BUTTON_1)	

				elif(code == BUTTON_2):
					print "Lancement de l application Mail"
					os.system(SHORTCUT_BUTTON_2)	

				elif(code == BUTTON_3):
					os.system(SHORTCUT_BUTTON_3)	

				elif(code == BUTTON_4):
					os.system(SHORTCUT_BUTTON_4)

				elif(code == BUTTON_5):
					os.system(SHORTCUT_BUTTON_5)

				elif(code == BUTTON_6):
					os.system(SHORTCUT_BUTTON_6)

				elif(code == BUTTON_7):
					os.system(SHORTCUT_BUTTON_7)

				elif(code == BUTTON_8):
					os.system(SHORTCUT_BUTTON_8)

				elif(code == BUTTON_9):
					os.system(SHORTCUT_BUTTON_9)

				elif(code == BUTTON_UP):
					mouseY -= speedMouse
					mousemove(mouseX,mouseY)

				elif(code == BUTTON_RIGHT):
					mouseX += speedMouse
					mousemove(mouseX,mouseY)

				elif(code == BUTTON_DOWN):
					mouseY += speedMouse
					mousemove(mouseX,mouseY)

				elif(code == BUTTON_LEFT):
					mouseX -= speedMouse
					mousemove(mouseX,mouseY)

				elif(code == BUTTON_MIDDLE):
					mouseclick(mouseX,mouseY)

				code = -1

    		
    
    
