import numpy as np
import cv2
from matplotlib import pyplot as plt
import os, math
from networktables import NetworkTables
import threading
from cscore import CameraServer

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

#Set table variable
tbl = NetworkTables.getTable('SmartDashboard')

#Set drivecam bool
tbl.putBoolean("driveCam", True)
#False = Vision camera
#True = Drive camera

#Get feed from camera/img
vision_cap = cv2.VideoCapture(0)
drive_cap = cv2.VideoCapture(2)

#Setting the exposure
vision_cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.25)
vision_cap.set(cv2.CAP_PROP_EXPOSURE, -15)

#Vals for inRange function, targeting green
hue = [25, 90]    #'''DONT TOUCH'''
sat = [75, 255]    #'''DONT TOUCH'''
lum = [35, 150]    #'''DONT TOUCH'''

#Iterations for the erode function
erode_iters = 0

#Iterations for the dialte function
dil_iters = 20

#Counter variable for the loop
counter = 0
ticks_since = 0

#Dub window
dub_window = 30

#Skip val
skip_val = 1

#Height to target  Inches / 12 for feet
target_height = 69

#Camera offset angle for subtraction
cam_offset = 14

#Focal length 
flength = 544

cs_str = input("Start cameraserver? (y/n): ")
if cs_str == 'y':
    cs_bool = True
else:
    cs_bool = False

if cs_bool:
    #Start cameraserver instance
    cs = CameraServer.getInstance()
    cs.enableLogging()

    #Open output stream
    outputStream = cs.putVideo("OpenCV Camera", 320, 580)

while(True):
    #Loop bools
    t_error = False
    i_error = False
    target_found = False

    #LED Pet project
    big_dub_x = False
    big_dub_y = False
    mega_dub = False

    #Take input from camera
    ret, img = vision_cap.read()
    drive_ret, drive_img = drive_cap.read()

    #Set image dimensions
    img_h = img.shape[0]
    img_w = img.shape[1]

    #Center of the image
    cx = int(img_w/2)
    cy = int(img_h/2)

    #Change colorspace to HLS then threshold
    hls_img = cv2.cvtColor(img, cv2.COLOR_BGR2HLS)
    bw_img = cv2.inRange(hls_img, (hue[0], lum[0], sat[0]),  (hue[1], lum[1], sat[1]))

    #Dilate areas of high val then close holes
    img_dilate = cv2.dilate(bw_img, None, dil_iters)
    img_closing = cv2.morphologyEx(img_dilate, cv2.MORPH_CLOSE, None)

    #Find contours
    _, contours, hierarchy = cv2.findContours(img_closing, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

    img_to_push = cv2.add(img, np.array([150.0]))
    #Loop through found contours
    for c in contours:
        #Set the contour bounding box dimensions
        bounds = cv2.boundingRect(c)
        x, y, w, h = bounds

        #Check if width of the contour is above 40, if not skip
    

        #Loop vars
        tot_pixels = w*h
        filled_pixels = 0
        checked_pixels = 0
        #Contour filtration
        if w>30 and w<300 and h>10 and h<150:
            confidence = 0
            wh_ratio = w/h
            wh_ratio_offset = abs(2.3 - wh_ratio)
            wh_ratio_rating = (-51.510*(wh_ratio_offset**2)) + (0.144*wh_ratio_offset) + 1.025
            if wh_ratio_rating > 0.2:
                blue = 0
                green = 255
                red = 0
            else:
                blue = 0
                green = 255
                red = 0

	    #Draw the bounding box with a point in the center
            #img_to_push = cv2.add(img, np.array([75.0]))

            cv2.rectangle(img_to_push, (x,y), (x+w,y+h), (red, green, blue), 2) #Thank you moth
            cv2.circle(img_to_push, (int(x+(w/2)),int(y+(h/2))), 5, (255,0,0))
            target = [int(x+(w/2)), int(y+(h/2))]

            cv2.line(img_to_push, (target[0]-10, target[1]), (target[0]+10, target[1]), (255, 255, 255))
            cv2.line(img_to_push, (target[0], target[1]-10), (target[0], target[1]+10), (255, 255, 255))

            if x > cx-dub_window and x < cx+dub_window:
                big_dub_x = True
            if y > cy-dub_window and y < cy+dub_window:
                big_dub_y = True

            #Calculate angle to turn to
            fin_angle_hori = ((math.atan((target[0]-(img_w/2))/flength)))*(180/math.pi)

            #Calculate vertical angle for distance calculations
	    #                 arctan(centerline - targety) * 180/pi     all + camera offset
            fin_angle_raw_rad = math.atan(((img_h/2) - y)/flength)
            fin_angle_deg = math.degrees(fin_angle_raw_rad) + cam_offset
            fin_angle_rad = math.radians(fin_angle_deg)
            distance = target_height / math.tan(fin_angle_rad)


            #fin_angle_vert = math.degrees(math.atan((img_h/2) - target[1])/flength) + cam_offset
            #distance = target_height * 1/ math.tan(radians(fin_angle_vert))
            target_found = True

        else:
            print("No contour")
            #img_to_push = cv2.add(img, np.array([75.0]))

        #BEGIN PC DETECTION
        #Values 

    if target_found:
        #Push final angle to shuffleboard
        tbl.putNumber("selectedAngle", fin_angle_hori)
        tbl.putNumber("selectedDistance", distance)
        tbl.putNumber("detectionCount", counter)
    counter += 1

    #Draw circle on center of screen
    cv2.circle(img, (cx, cy), 5, (255, 0, 0))

    if cs_bool:
        print("Drive bool:", tbl.getBoolean("driveCam", False))
        if tbl.getBoolean("driveCam", False) == True:
            scale_factor = 10

            width = int(drive_img.shape[1] * scale_factor / 100)
            height = int(drive_img.shape[0] * scale_factor / 100)
            dim = (width, height)
            drive_resized = cv2.resize(drive_img, dim, interpolation = cv2.INTER_AREA)

            outputStream.putFrame(drive_img)
        if tbl.getBoolean("driveCam", False) == False:
            scale_factor = 70

            width = int(img_to_push.shape[1] * scale_factor / 100)
            height = int(img_to_push.shape[0] * scale_factor / 100)
            dim = (width, height)

            img_to_push_resized = cv2.resize(img_to_push, dim, interpolation = cv2.INTER_AREA)
            outputStream.putFrame(img_to_push_resized)

    os.system('clear')
    print("Loop #: ", counter)
    tbl.putBoolean("Mega dub?", mega_dub)
    if mega_dub:
        print("MEGA FREAKING DUB")

    if target_found:
        print("Turn to ", fin_angle_hori, " Distance:", distance, " Calculated angle:", fin_angle_deg, " (X, Y):", target[0], target[1], " (CX, CY):", img_w/2, img_h/2)
        print("CONFIDENCE CRITERIA  w/h ratio conf:", wh_ratio_rating)
    if t_error:
        print("TypeError")

    if i_error:
        print("IndexError")

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
