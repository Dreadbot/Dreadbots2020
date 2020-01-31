import os
import cv2
import datetime
from base_camera import BaseCamera


class Camera(BaseCamera):
    video_source = 0

    def __init__(self):
        if os.environ.get('OPENCV_CAMERA_SOURCE'):
            Camera.set_video_source(int(os.environ['OPENCV_CAMERA_SOURCE']))
        super(Camera, self).__init__()

    @staticmethod
    def set_video_source(source):
        Camera.video_source = source

    @staticmethod
    def frames():
        camera = cv2.VideoCapture(Camera.video_source)
        if not camera.isOpened():
            raise RuntimeError('Could not start camera.')

        color = red = (0, 0, 255)

        while True:
            # read current frame
            _, img = camera.read()
            flipped = cv2.flip(img, 1) # Flip on the horizontal so it is like a mirror.
            now = datetime.datetime.now()
            cv2.putText( flipped, "%s" % now, (10,10) ,
                         cv2.FONT_HERSHEY_SIMPLEX, 0.35, color, 1 )


            # encode as a jpeg image and return it
            yield cv2.imencode('.jpg', flipped)[1].tobytes()
