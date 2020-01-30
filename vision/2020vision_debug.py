import numpy as np
import cv2
from matplotlib import pyplot as plt
import os, math
from networktables import NetworkTables
import threading
import datetime

imgdir = os.getenv( "IMGDIR", "/mnt/ramdisk/vision/" )
robotip = os.getenv( "ROBOT_IP", "10.36.56.2" )
cond = threading.Condition()
notified = [False]

def connectionListener(connected, info):
    print(info, '; Connected=%s' % connected)
    with cond:
        notified[0] = True
        cond.notify()

NetworkTables.initialize(server=robotip)
NetworkTables.addConnectionListener(connectionListener, immediateNotify=True)

with cond:
    print("Waiting for", robotip)
    if not notified[0]:
        cond.wait()

print("Connected to", robotip, "!")

#Set table variable
tbl = NetworkTables.getTable('SmartDashboard')

#Get feed from camera/img
cap = cv2.VideoCapture(0)

#Setting the exposure
cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.25)
cap.set(cv2.CAP_PROP_EXPOSURE, -100)

#Vals for inRange function, targeting green
hue = [25, 100]    #'''DONT TOUCH'''
sat = [75, 240]    #'''DONT TOUCH'''
lum = [17, 25]    #'''DONT TOUCH'''

#Iterations for the erode function
erode_iters = 0

#Iterations for the dialte function
dil_iters = 20

#Counter variable for the loop
counter = 0

#Dub window
dub_window = 30

#Center of the screen

lastimwrite=None
while(True):
    #Loop bools
    t_error = False
    i_error = False
    target_found = False

    big_dub_x = False
    big_dub_y = False
    mega_dub = False

    #Take input from camera
    ret, img = cap.read()

    #Set image dimensions
    img_h = img.shape[0]
    img_w = img.shape[1]


    u = int(img_w/2)
    v = int(img_h/2)

    #Change colorspace to HLS then threshold
    hls_img = cv2.cvtColor(img, cv2.COLOR_BGR2HLS)
    bw_img = cv2.inRange(hls_img, (hue[0], lum[0], sat[0]),  (hue[1], lum[1], sat[1]))

    #Erode noise
    #img_erode = cv2.erode(bw_img, None, erode_iters)

    #Dilate areas of high val then close holes
    img_dilate = cv2.dilate(bw_img, None, dil_iters)
    img_closing = cv2.morphologyEx(img_dilate, cv2.MORPH_CLOSE, None)


    print("Find contours")
    contours, hierarchy = cv2.findContours(img_closing, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

    #Loop through found contours
    for c in contours:
        #Set the contour bounding box dimensions
        bounds = cv2.boundingRect(c)
        x, y, w, h = bounds

	#Contour filtration
        if h > 20 and w > 50:
            print("X: ", x, "Y: ", y)
            print("Width", img_w)

	    #Draw the bounding box with a point in the center
            cv2.rectangle(img, (x,y), (x+w,y+h), (0,255,0), 2) #Thank you moth
            cv2.circle(img, (int(x+(w/2)),int(y+(h/2))), 5, (255,0,0))
            target = [int(x+(w/2)), int(y+(h/2))]

            if x > u-dub_window and x < u+dub_window:
                big_dub_x = True
            if y > v-dub_window and y < v+dub_window:
               big_dub_y = True

            #Calculate angle to turn to
            fin_angle = 100*(math.atan((target[0]-(img_w/2))/333.82))
            target_found = True

    cv2.circle(img, (int(img_w/2), int(img_h/2)), 5, (255, 0, 0))

    #Show results
    cv2.imshow('Thresh image', img)
    now = datetime.datetime.now()
    if lastimwrite == None or (now-lastimwrite).seconds > 0.1:
        lastimwrite = now
        imgfile = os.path.join( imgdir, "annotated.jpg" )
        print( "Writing %s" % imgfile)        
        cv2.imwrite( imgfile, img )
        print( "Wrote annotated.jpg" )
        import time
    

    if target_found:
        #Push final angle to shuffleboard
        tbl.putNumber("selectedAngle", fin_angle)
        tbl.putNumber("detectionCount", counter)
        counter += 1

    if big_dub_x and big_dub_y:
        cv2.circle(img, (u, v), 30, (255, 0, 0))
        mega_dub = True

    #Print statements
    os.system('clear')
    print("Loop #: ", counter)
    tbl.putBoolean("Mega dub?", mega_dub)
    if mega_dub:
        print("MEGA FREAKING DUB")

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
