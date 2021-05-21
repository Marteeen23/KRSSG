# THE BIRMINGHAM GAMBLERS
## _Socket-Programming in CPP_

This is a Casino made using Socket-Programming and Multi-threading in CPP. In this game, their is the main server file, a client file which sets the game, i.e, sets the number of players and number of rounds, and lastly, another client file to play the game. 

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