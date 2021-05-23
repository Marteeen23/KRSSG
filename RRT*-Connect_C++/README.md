# PATH-PLANNING
## _RRTStar-Connect Algorithm in CPP_

This is a path-planning program which implements RRTStar-Connect Algorithm in C++. It has to implement the path found into the turtlesim_node in ROS.

## PRE-REQUISITES
- OpenCV for C++ should be installed in your system. Also, a CMakeLists.txt file has to be created in the same folder in which the code file is present. The contents of this CMakeLists.txt file is provided below. Also, put the image files in the same directory.
```sh
cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( DisplayImage rrt.cpp )
target_link_libraries( DisplayImage ${OpenCV_LIBS} )
```
- For the ROS-PID, a CMakeLists.txt file is already included in the repository. Just make sure to put in the same folder as the src folder which has the cpp file.

## HOW TO RUN???

- Open a terminal and go to the directory in which the files are present.
- Then do the following: 
```sh
make
./DisplayImage
```
- The ROS-PID is not implemented together with this algorithm as there is some issue with the code and published values are not showing up.
- To run ROS-PID code, follow the following steps:
- First, create a workspace directory. Then, create another directory in it and name it "src".
```sh
mkdir catkin_ws
cd catkin_ws
mkdir src
```
- Go inside this directory and use the catkin_create_pkg command to create a package and give its dependencies.
```sh
cd src
catkin_create_pkg path_find roscpp std_msgs
```
- This will create a package. Now, put the PID_ROS.cpp and PID_Header.h in the src directory of path_find directory.
- You will also see a CMakeLists.txt file in the path_find directory. Just replace its content with the contents of CMakeLists.txt
- After this, turn on three terminals and in each of them run the following code.
```sh
source /opt/ros/<ros-version-name>/setup.bash
```
- In one terminal, run roscore.
```sh
roscore
```
- In the second terminal run the turtlesim node.
```sh
rosrun turtlesim turtlesim_node
```
- In the third terminal, first go to the catkin_ws directory which was created above and run the following bits of code.
```sh
catkin_make
source devel/setup.bash
```
- Finally, in the third terminal run the following code.
```sh
rosrun path_find turtlesim
```
- The execution won't show any response on the turtlesim_node since there is some error in the code.
