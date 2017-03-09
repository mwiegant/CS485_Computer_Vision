```
###---------------------------------------
# CS 485 - Assignment 2                     
#
#  Coded, Designed, Documented by:
#
#        Max Wiegant
#
###---------------------------------------
```

# Navigating this Codebase

My codebase is setup with a folder for each of the separate problems in the assignment.

For example, to see code for Gaussian Smoothing, parts (a), (b), or (c), go to the folder
named '1_gaussian_smoothing'. Each of the 3 numbered folders has a 'driver.cpp' file which
provides a menu to gather inputs as required. Further, each of the 3 numbered folders has
a folder named 'src' which contains the actual code required to create masks, read and write
images, and perform the actual required operations (smoothing, edge detection, etc).

The main driver with the initial menu displayed in screen is located in the root level of
this project, at the same level as this README, and is named 'menu.cpp'. It's purpose is to
move control over to the appropriate functionality in this codebase, as dictated by the user
at runtime.

As a final note, the images used for this assignment are provided inside the 'img' folder.


# Running this Project
 
This project was built, ran, and compiled in a Windows machine running Windows 10. It may
have issues compiling on distributions of Linux. To run this project, navigate to the root
directory where this README is located, and run 'make' in the terminal. This should compile
and produce an executable named 'pa1'. To run the program, after the executable has been
created, run './pa1' in the terminal. The root menu for this program should appear.


# Notes about Assumptions

This project assumes that inputted images are square.