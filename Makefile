LIBS=-lsfml-graphics -lsfml-window -lsfml-system -lzmq
SRC=src
BIN=bin

OBJS = $(BIN)/time.o \
	   $(BIN)/thread.o \
	   $(BIN)/mover.o \
	   $(BIN)/graphicsObject.o \
	   $(BIN)/item.o \
	   $(BIN)/platform.o \
	   $(BIN)/character.o \
	   $(BIN)/collider.o \
	   $(BIN)/draw.o \
	   $(BIN)/ioHandler.o \

OBJ_FILES = $(BIN)/time.o \
	   $(BIN)/thread.o \
	   $(BIN)/mover.o \
	   $(BIN)/graphicsObject.o \
	   $(BIN)/item.o \
	   $(BIN)/platform.o \
	   $(BIN)/character.o \
	   $(BIN)/collider.o \
	   $(BIN)/draw.o \
	   $(BIN)/ioHandler.o \
	   $(BIN)/gameState.o

# main copilation
$(BIN)/main.o: $(SRC)/main.cpp
	g++ -c $< -o $@

# draw compilation
$(BIN)/draw.o: $(SRC)/Draw/draw.cpp
	g++ -c $< -o $@

# time
$(BIN)/time.o: $(SRC)/Time/Timeline.cpp
	g++ -c $< -o $@

# thread
$(BIN)/thread.o: $(SRC)/Time/Thread.cpp
	g++ -c $< -o $@

# collider
$(BIN)/collider.o: $(SRC)/Movement/Collider.cpp
	g++ -c $< -o $@

# movement
$(BIN)/mover.o: $(SRC)/Movement/Mover.cpp
	g++ -c $< -o $@

# GraphicsObject
$(BIN)/graphicsObject.o: $(SRC)/GraphicsObject/GraphicsObject.cpp
	g++ -c $< -o $@

# character
$(BIN)/character.o: $(SRC)/GraphicsObject/Character.cpp
	g++ -c $< -o $@

# platform
$(BIN)/platform.o: $(SRC)/GraphicsObject/Platform.cpp
	g++ -c $< -o $@

# item
$(BIN)/item.o: $(SRC)/GraphicsObject/Item.cpp
	g++ -c $< -o $@

# io handler
$(BIN)/ioHandler.o: $(SRC)/io/ioHandler.cpp
	g++ -c $< -o $@

# single client game
# Link all object files to create the executable
game: $(OBJS) $(BIN)/main.o
	g++ $(OBJS) $(BIN)/main.o -o game $(LIBS)	


# networking game
$(BIN)/gameState.o: $(SRC)/GameRunner/GameState.cpp
	g++ -c $< -o $@

$(BIN)/gameRunner.o: $(SRC)/GameRunner/GameRunner.cpp
	g++ -c $< -o $@

$(BIN)/server.o: $(SRC)/Network/Server.cpp
	g++ -c $< -o $@

gameServer: $(OBJ_FILES) $(BIN)/server.o
	g++ $(OBJ_FILES) $(BIN)/server.o -o gameServer $(LIBS)


$(BIN)/client.o: $(SRC)/Network/Client.cpp
	g++ -c $< -o $@

gameClient: $(OBJ_FILES) $(BIN)/gameRunner.o $(BIN)/client.o
	g++ $(OBJ_FILES) $(BIN)/gameRunner.o $(BIN)/client.o -o gameClient $(LIBS)

# debugging game
# game -d: $(OBJS)
# 	g++ -g $(OBJS) -o game $(LIBS)

# Compilation
# g++ -c "src/main.cpp" -o bin/main.o
# g++ bin/main.o -o game -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f $(BIN)/*.o 
	rm -f ./game 
	rm -f ./gameServer 
	rm -f ./gameClient
	