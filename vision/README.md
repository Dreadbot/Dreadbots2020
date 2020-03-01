Startup

Step 1: Make sure the robot is on

Step 2: Make sure the Pi is on
Check for the red light
Light is on the end opposite of the ports

Step 3: Run through Pi Checklist
Ensure there’s a card in it
Check for connectivity light on ethernet port
Check the camera’s connected (Look for blue light on the camera)

Step 4: Open a terminal on a laptop connected to the robot

Step 5: SSH into the Pi
On the command line: ssh pi@10.36.56.11

Step 6: Change directory to the vision directory
On the command line: cd vision

Step 7: Execute the vision script
On the command line: ./start.sh

Step 7.5: Enable/Disable camera server
You will be prompted “Would you like to start the cameraserver”
Unless you’re going to the field (in which case use drive coach discretion) type y

Step 8: Assure setup is complete
Terminal should’ve read out something confirming connection
Terminal should now be blank or reading out the angle

Should be ready to go! If it’s not, give the troubleshooting a look