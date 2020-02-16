import numpy as np
import cv2
from matplotlib import pyplot as plt
import os, math
from networktables import NetworkTables
import threading


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


tbl = NetworkTables.getTable('SmartDashboard')


cap = cv2.VideoCapture(0)


cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.25)
cap.set(cv2.CAP_PROP_EXPOSURE, -15)

#Vals for inRange function, targeting green
hue = [25, 90]    #'''DONT TOUCH'''
sat = [75, 255]    #'''DONT TOUCH'''
lum = [35, 150]    #'''DONT TOUCH'''

erode_iters = 0
dil_iters = 20

counter = 0

dub_window = 30
skip_val = 1

target_height = 72
cam_offset = 22
flength = 544

while(True):
    t_error = False
    i_error = False
    target_found = False

    big_dub_x = False
    big_dub_y = False
    mega_dub = False

    ret, img = cap.read()

    img_h = img.shape[0]
    img_w = img.shape[1]

    u = int(img_w/2)
    v = int(img_h/2)

    hls_img = cv2.cvtColor(img, cv2.COLOR_BGR2HLS)
    bw_img = cv2.inRange(hls_img, (hue[0], lum[0], sat[0]),  (hue[1], lum[1], sat[1]))

    img_dilate = cv2.dilate(bw_img, None, dil_iters)
    img_closing = cv2.morphologyEx(img_dilate, cv2.MORPH_CLOSE, None)

    _, contours, hierarchy = cv2.findContours(img_closing, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

    for c in contours:
        bounds = cv2.boundingRect(c)
        x, y, w, h = bounds
        
        if w > 40:
            pass
        else:
            continue
        
        tot_pixels = w*h
        filled_pixels = 0
        checked_pixels = 0

        infill = cv2.contourArea(c)/tot_pixels
        
        if w>30 and h>10:
            cv2.rectangle(img, (x,y), (x+w,y+h), (0,255,0), 2) #Thank you moth
            cv2.circle(img, (int(x+(w/2)),int(y+(h/2))), 5, (255,0,0))

            target = [int(x+(w/2)), int(y+(h/2))]

            fin_angle_hori = ((math.atan((target[0]-(img_w/2))/flength)))*(180/math.pi)

            distance = target_height / math.tan(((((math.atan((target[1]-(img_h/2))/flength)))*(180/math.pi)) + cam_offset)*(math.pi/180))
            target_found = True
        
    if target_found:
        tbl.putNumber("selectedAngle", fin_angle_hori)
        tbl.putNumber("detectionCount", counter)
        counter += 1
    
    cv2.circle(img, (int(img_w/2), int(img_h/2)), 5, (255, 0, 0))

    curRes_raw = "results/%s-raw.jpg" % counter
    curRes_bin = "results/%s-bin.jpg" % counter

    os.system('clear')
    print("Loop #: ", counter)

    if mega_dub:
        print("MEGA FREAKING DUB")

    if target_found:
        print("Turn to ", fin_angle_hori, " Distance:", distance)

    if t_error:
        print("TypeError")

    if i_error:
        print("IndexError")

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()