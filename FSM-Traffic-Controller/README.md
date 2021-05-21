# FSM-Traffic-Controller
## _Finite State Machine implementation in CPP_

This is a Traffic Controller made by implementing the concept of Finite State Machines in CPP. There is a basic traffic controller which takes user input and relieves the traffic by implementing FSM and there is another set of server and client files which are connected by Socket Programming. The client generates random traffic and the Traffic Controller relieves that traffic.

## HOW TO RUN???

- Running the FMS_Basic.cpp file is straight forward. Open the terminal and compile & run it. After that give theinput it asks for.
- Suggested way : 
```sh
g++ FMS_Basic.cpp
./a.out
```
- To run the Traffic Generator and Traffic Controller, do the following.
- First, run the Controller server file (TrafficController.cpp) on a terminal.
- Suggested way : 
```sh
g++ -o server TrafficController.cpp
./server
```
- Next, run the Generator client file (TrafficGeneratorClient.cpp) on another terminal and give the input required.
- Suggested way : 
```sh
g++ -o client TrafficGeneratorClient.cpp.cpp
./client
```
