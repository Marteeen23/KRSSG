# FSM-Traffic-Controller
## _Finite State Machine implementation in CPP_

This is a Traffic Controller made by implementing the concept of Finite State Machines in CPP. There is a basic traffic controller which takes user input and relieves the traffic by implementing FSM and there is another set of server and client files which are connected by Socket Programming. The client generates random traffic and the Traffic Controller relieves that traffic.

## Code Documentation

First, the 20 different traffic states are enumerated, so that they can be used with switch-case while deciding what actions to take in different states. In the main function of the TrafficController.cpp(server), first of all a connection is established and user input is asked for timesteps. This info is sent to TrafficGeneratorClient.cpp(client), which randomly generates traffic for each timestep and sends it to the server in an online manner. Then, the transition function is called which optimally decides the transition to choose and then sends the respective transition state to the traffic function which then takes the action according to the state it has received.

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
