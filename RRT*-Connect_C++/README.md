# PATH-PLANNING
## _RRTStar-Connect Algorithm in CPP_

This is a path-planning program which implements RRTStar-Connect Algorithm in C++.

## PRE-REQUISITES
OpenCV for C++ should be installed in your system. Put the CMakeLists.txt file in the same folder as the src folder which has the cpp file. Also, put the image files in the same directory.

## HOW TO RUN???

- Open a terminal and go to the directory in which the files are present.
- Then do the following: 
```sh
make
./DisplayImage
```
- The ROS-PID is not implemented together with this algorithm as there is some issue with the code and published values are not showing up.
- To run ROS-PID code use the CMakeLists.txt file given.
- Run roscore in first terminal.
```sh
roscore
```
- In the second terminal run the turtlesim node.
```sh
rosrun turtlesim turtlesim_node
```
- In the third terminal run the code. Here, "path_find" is the folder in which "src" is present which has the codes.
```sh
rosrun path_find turtlesim
```
