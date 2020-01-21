import numpy as np
import cv2
from matplotlib import pyplot as plt
import os, math

#Get feed from camera/img
cap = cv2.VideoCapture(1)
cap.set(cv2.CAP_PROP_EXPOSURE, -13)


#Vals for inRange function, targeting green
hue = [60, 170]
sat = [70, 255]
lum = [20, 25]

#Iterations for the Erode function
erode_iters = 0

#Counting loops
counter = 0

#Params for finding lines
rho = 1
theta = np.pi / 180
threshold = 10
min_line_length = 50
max_line_gap = 20

#Just color lists so that the lines can alternate colors
loopclrs = [(255, 0, 0), (0, 255, 0), (0, 0, 255), (255, 0, 255), (255, 255, 255)]

def findCenter(p1, p2):
    avg = (p1 + p2) / 2
    return avg

def calculateDist(x1, y1, x2, y2):
    return math.sqrt((x2-x1)**2 + (y2-y1)**2)

while(True):
    t_error = False
    i_error = False
    ret, img = cap.read()

    #Change colorspace to HLS & Threshold image just to green vals, then erode image to kill noise
    hls_img = cv2.cvtColor(img, cv2.COLOR_BGR2HLS)
    bw_img = cv2.inRange(hls_img, (hue[0], lum[0], sat[0]),  (hue[1], lum[1], sat[1]))
    img_erode = cv2.erode(bw_img, None, None, None, erode_iters)
    
    # cv2.imshow('Raw image w/ results', img)
    cv2.imshow('Thresh image', bw_img)

    #Start prints
    os.system('cls') #Cls for windows, clear for anything else
    counter += 1
    print("Loop #: ", counter)

    if t_error:
        print("TypeError")

    if i_error:
        print("IndexError")
    
    #End of loop operations

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
