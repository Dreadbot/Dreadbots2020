import numpy as np
import cv2
from matplotlib import pyplot as plt
import os

cap = cv2.VideoCapture(0)




#Vals for inRange function, targeting green
hue = [0, 255]
sat = [150, 255]
lum = [200, 255]

#Thresholds for Canny edge detection
low_thresh = 50
high_thresh = 150

#Iterations for the Erode function
erode_iters = 1

#Counting loops
counter = 0

#Params for finding lines
rho = 1
theta = np.pi / 180
threshold = 150
min_line_length = 300
max_line_gap = 100

#Just color lists so that the lines can alternate colors
loopclrs = [(255, 0, 0), (0, 255, 0), (0, 0, 255), (255, 0, 255), (255, 255, 255)]

def findCenter(p1, p2):
    avg = (p1 + p2) / 2
    return avg

while(True):
    t_error = False
    i_error = False
    ret, img = cap.read()
    #img = cv2.medianBlur(img,5)

    #Create and clear xs and ys lists
    xs = []
    ys = []

    #Change colorspace to HLS & Threshold image just to green vals, then erode image to kill noise
    hls_img = cv2.cvtColor(img, cv2.COLOR_BGR2HLS)
    bw_img = cv2.inRange(hls_img, (hue[0], lum[0], sat[0]),  (hue[1], lum[1], sat[1]))
    img_erode = cv2.erode(bw_img, None, None, None, erode_iters)
    img_edges = cv2.Canny(img_erode, low_thresh, high_thresh)
    

    #Detect lines using Hough Transform
    line_image = np.copy(img) * 0
    try:
        lines_raw = cv2.HoughLinesP(img_edges, rho, theta, threshold, np.array([]),
                        min_line_length, max_line_gap)
        lines = list(lines_raw)

                #LINES LIST SYNTAX
            #NAME NUMBER    NOT SURE   SELECT X (0) OR Y (1)
            #lines [0]      [1]        [x/y]
            
            #Draw lines on image
        for i in range(0, len(lines)):
            cur_line_center = (int(findCenter(lines[i][0][0], lines[i][0][2])), int(findCenter(lines[i][0][1], lines[i][0][3])))

            #Drawing functions
            cv2.line(img, (lines[i][0][0], lines[i][0][1]), (lines[i][0][2], lines[i][0][3]), loopclrs[i], 1)
            cv2.circle(img, cur_line_center, 1, (0,0,255))
            

    except TypeError:
        t_error = True
        lines = []
    except IndexError:
        lines = []
        i_error = True

    
    
    # cv2.imshow('Raw image w/ results', img)
    cv2.imshow('Thresh image', img)

    #Start prints
    os.system('clear')
    counter += 1
    print("Loop #: ", counter)

    if t_error:
        print("TypeError")

    if i_error:
        print("IndexError")

    for i in range(0, len(lines)):
        print("Line #", i, " : ", lines[i][0])
    
    #End of loop operations

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
