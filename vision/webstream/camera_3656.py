import time
from base_camera import BaseCamera
import os
import os.path

class Camera(BaseCamera):
    """An emulated camera implementation that streams an image from
       /mnt/ramdisk/vision/annotated.jpg if it exists."""
    imgdir = os.getenv( "IMGDIR", "/mnt/ramdisk/vision" )
    imgfile = os.path.join( imgdir, "annotated.jpg" )
    imgs = [open(f + '.jpg', 'rb').read() for f in ['1', '2', '3']]

    @staticmethod
    def frames():
        while True:
            if os.path.exists( Camera.imgfile ):
                yield open( Camera.imgfile, 'rb').read()
            else:
                yield Camera.imgs[int(time.time()) % 3]
            time.sleep(0.1)
                
