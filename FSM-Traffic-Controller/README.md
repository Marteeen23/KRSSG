# FSM-Traffic-Controller
This is an FSM-based approach at solving a problem statement that had four roads and three traffic lights each with one straight green, one right green and the third one red.
There are three cpp files. FSM_Basic.cpp has just a simple input format, wherein user enters the input data and the query is handled in an online manner.
The FSM used has 20 states in total in which any state can transition into the other. This transition occurs in transition(<args>) function using the approach of max traffic in a road.
There are 12 states wherein two lights can be in the green state out of the total 8 lights and the rest 8 states are those where only one light is green at a time.
The rest two files form one package where there is a client file which generates traffic randomly and there is a server which handles that traffic using the approach in FSM_Basic.cpp.
This server-client relation is done using socket programming.
