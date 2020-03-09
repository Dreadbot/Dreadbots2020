import cv2
from networktables import NetworkTables
from cscore import CameraServer
import threading

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.25)
cap.set(cv2.CAP_PROP_EXPOSURE, -15)

hue = [0, 0]
sat = [0, 0]
lum = [0, 0]

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

tbl.putNumber("Hue lower", 0)
tbl.putNumber("Hue upper", 0)
tbl.putNumber("Sat lower", 0)
tbl.putNumber("Sat upper", 0)
tbl.putNumber("Lum lower", 0)
tbl.putNumber("Lum upper", 0)

tbl.putBoolean("Stop", False)

cs = CameraServer.getInstance()
cs.enableLogging()

outputStream = cs.putVideo("OpenCV Camera", 320, 580)

while True:
    ret, img = cap.read()

    hue = [tbl.getNumber('Hue lower', 0), tbl.getNumber('Hue upper', 0)]
    sat = [tbl.getNumber('Sat lower', 0), tbl.getNumber('Sat upper', 0)]
    lum = [tbl.getNumber('Lum lower', 0), tbl.getNumber('Lum upper', 0)]

    hls_img = cv2.cvtColor(img, cv2.COLOR_BGR2HLS)
    bw_img = cv2.inRange(hls_img, (hue[0], lum[0], sat[0]), (hue[1], lum[1], sat[1]))

    img_closing = cv2.morphologyEx(bw_img, cv2.MORPH_CLOSE, None)

    outputStream.putFrame(img_closing)

    if tbl.getBoolean('stop', False):
        break

cap.release()
cv2.destroyAllWindows()

print('HUE:', hue)
print('SAT:', sat)
print('LUM:', lum)
