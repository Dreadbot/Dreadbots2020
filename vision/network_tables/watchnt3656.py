#!/usr/bin/env python

from __future__ import print_function

import time
import sys

# pip install pynetworktables

from networktables import NetworkTables

NetworkTables.enableVerboseLogging()
NetworkTables.initialize(server='roborio-3656-frc.local')

# It takes a minute for network tables to initialize
t1=time.time()
timeout=15
now=time.time()
while not(NetworkTables.isConnected()) and (now-t1)<timeout:
      #print("..")
      sys.stdout.write( '.' )
      sys.stdout.flush()
      #print( ".", endl="" )
      now=time.time()
      time.sleep( 1 )

print( "\n" )
sd = NetworkTables.getTable('SmartDashboard')
"""
stables = sd.getSubTables()
print( "= NetworkTables %s = " % len(stables) )
for st in stables:
    print( "- '%s'" % st )
print(" ")
"""

from nt3656 import Nt3656

nt3656 = Nt3656()

def ShowTable(nt3656):

    def showfield( nt3656, name ):
          print( "nt3656.%s=%s" % ( name, getattr( nt3656, name) ) )

    showfield( nt3656,"hsv_h_lo")
    showfield( nt3656, 'hsv_h_hi' )
    showfield( nt3656, 'hsv_s_lo' )
    showfield( nt3656, 'hsv_s_hi' )
    showfield( nt3656, 'hsv_v_lo' )
    showfield( nt3656, 'hsv_v_hi' )
    showfield( nt3656, 'vis_cam_brightness' )
    showfield( nt3656, 'yellowbox_mode' )
    showfield( nt3656, 'auton_mode' )


    print(" ")

ShowTable(nt3656)    
