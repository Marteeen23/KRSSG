# THE BIRMINGHAM GAMBLERS
## _Socket-Programming in CPP_

This is a Casino made using Socket-Programming and Multi-threading in CPP. In this game, their is the main server file, a client file which sets the game, i.e, sets the number of players and number of rounds, and lastly, another client file to play the game. 

## CODE DOCUMENTATION

First of all, the TheBirminghamGamblers.cpp file(server) accepts connection from the first client, i.e, the GameSetter.cpp file. This sets the number of players and the number of rounds for the game. After this, the connection with this client is closed. After that, as many players are there, that many terminals are opened and Gamblers.cpp file is run in each of them. This connection is established through multi-thraeding so that each of the clients are in a thread running parallely with the other threads, so that the card is provided to each player simultaneously and each of them return their cards simultaneously as well. The cards are generated randomly by the server for each round. This part is the most time-taking part of the program as each card should be different. After this, 3-each cards are sent to each player and they select the max out of their cards and send it back to server. The server chooses the maximum out of them and decides the winner of that round and shows it on server screen. After all rounds are completed, the winner is decided and the winner is shown on all the terminals.

## HOW TO RUN???

- Open a terminal and compile & run the server file (TheBirminghamGamblers.cpp) first.
- Suggested way : 
```sh
g++ -pthread -o server TheBirminghamGamblers.cpp
./server
```
- Next, run the setter client file (GameSetter.cpp) on a different terminal.
- Suggested way : 
```sh
g++ -o setter GameSetter.cpp
./setter
```
- Lastly, run the player client file (Gambler.cpp) on the required number of terminals.
- Suggested way : 
```sh
g++ -o gambler Gambler.cpp
./gambler
```
