# CSC481: Game Engine Foundations

## REQUREMENTS:

This project requires various libraries, follow the instructions below to install

- gcc: `sudo apt install gcc`
- v8: `sudo apt-get install libv8-dev`
- zmq: `sudo apt-get install libzmq3-dev`

## COMPILE & RUN INSTRUCTIONS: 

** Game **
* from the project directory run `make gameServer`
* to start the game server enter ./gameServer

* from the project directory run `make gameClient`
* to start any game client enter ./gameClient


## USAGE:

* player movement is input using the arrow keys and space bar to jump
* pressing key `P` pauses all movement
* pressing key `1` sets game speed to 0.5
* pressing key `2` sets game speed to 1.0 (default)
* pressing key `3` sets game speed to 2.0


## VERSIONS:
this game is being developed on
* Ubuntu Virtual Machine `22.04.3 LTS (GNU/Linux 5.15.0-83-generic aarch64)`
* SFML version `2.5.1`
* Lib ZEROmq `4.3.4`


## SECOND GAME:
* navigate to the tetris folder to see the second game
* a README.md file in that directory explains compilation and usage
