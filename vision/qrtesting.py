import numpy as np
import cv2, PIL
from cv2 import aruco
import matplotlib.pyplot as plt
import matplotlib as mpl
import pandas as pd


cap = cv2.VideoCapture(0)
x = 0
y = 0

while True:
    ret, img = cap.read()
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    aruco_dict = aruco.Dictionary_get(aruco.DICT_6X6_250)
    params = aruco.DetectorParameters_create()
    corners, ids, rejectedImgPoints = aruco.detectMarkers(img_gray, aruco_dict, parameters=params)
    #print("IDS: ",ids, "CORNERS: ", corners)
    
    if ids is not None:
        for c in corners:
            for i in ids:
                x = c[0][0][0]
                y = c[0][0][1]
                cv2.circle(img, (x, y), 50, (255, 143, 24))
    cv2.imshow('Frame', img)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()