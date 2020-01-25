import numpy as np
import cv2
from matplotlib import pyplot as plt
import os, math
from networktables import NetworkTables
import threading

#TODO
#Recalibrate vals
#Test nt

cond = threading.Condition()
notified = [False]

def connectionListener(connected, info):
    print(info, '; Connected=%s' % connected)
    with cond:
        notified[0] = True
        cond.notify()

NetworkTables.initialize(server='10.36.56.2')
NetworkTables.addConnectionListener(connectionListener, immediateNotify=True)

with cond:
    print("Waiting")
    if not notified[0]:
        cond.wait()

print("Connected!")

#Get feed from camera/img
cap = cv2.VideoCapture(0)

#Setting the exposure
cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.25)
cap.set(cv2.CAP_PROP_EXPOSURE, -15)

#Vals for inRange function, targeting green
hue = [75, 250]    #'''DONT TOUCH'''
sat = [45, 255]    #'''DONT TOUCH'''
lum = [32.5, 230]    #'''DONT TOUCH'''

#Iterations for the dialte function
dil_iters = 20

#Counter variable for the loop
counter = 0

while(True):
    #Loop bools
    t_error = False
    i_error = False
    target_found = False

    #Take input from camera
    ret, img = cap.read()

    #Set image dimensions
    img_h = img.shape[0]
    img_w = img.shape[1]

    #Change colorspace to HLS then threshold
    hls_img = cv2.cvtColor(img, cv2.COLOR_BGR2HLS)
    bw_img = cv2.inRange(hls_img, (hue[0], lum[0], sat[0]),  (hue[1], lum[1], sat[1]))

    #Dilate areas of high val then close holes
    img_dilate = cv2.dilate(bw_img, None, dil_iters)
    img_closing = cv2.morphologyEx(img_dilate, cv2.MORPH_CLOSE, None)


    #Find contours
    _, contours, hierarchy = cv2.findContours(img_closing, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

    #Loop through found contours
    for c in contours:
        #Set the contour bounding box dimensions
        bounds = cv2.boundingRect(c)
        x, y, w, h = bounds

	#Contour filtration
        if h > 20:
            print("X: ", x, "Y: ", y)
            print("Width", img_w)

	    #Draw the bounding box with a point in the center
            cv2.rectangle(img, (x,y), (x+w,y+h), (0,255,0), 2) #Thank you moth
            cv2.circle(img, (int(x+(w/2)),int(y+(h/2))), 5, (255,0,0))
            target = [int(x+(w/2)), int(y+(h/2))]

            #Calculate angle to turn to
            fin_angle = 100*(math.atan((target[0]-(img_w/2))/333.82))
            target_found = True

    #Show results
    cv2.imshow('Thresh image', img_closing)

    #Print statements
    os.system('clear')
    counter += 1
    print("Loop #: ", counter)
    if target_found: 
        print("Turn to ", fin_angle)

    if t_error:
        print("TypeError")

    if i_error:
        print("IndexError")
    
    #End of loop operations
#    if counter > 100:
#        break

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
#version 1
