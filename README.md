# Line-Follower-Robot
This repo contains the code for a Line Follower Robot . The robot was built using arduino Uno for the navigation and a Raspberry pi for the line detection .
the robot utilizes rotary encoders coupled with a PID (Proportional-Integral-Derivative) algorithm to precisely control its speed ,it's also  equipped with an ultrasonic sensor, to check for obstacles using Timer 2,
See [Arduino code](main_program.ino). 

The vision aspect of the project is realized through the integration of a camera using OpenCV. Since we're working with black lines only , [the code](open_cv.py) is pretty straightforward :
    Convert the frame to the HSV color space,
    Define a lower and upper threshold for detecting black color,
    Create a binary mask (black_mask) using cv2.inRange to extract the black regions,
    Sum the values along the vertical axis of the binary mask to create a histogram,
    the position of the line  is set to the position of the maximum value in the histogram.
