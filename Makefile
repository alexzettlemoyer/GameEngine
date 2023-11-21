LIBS=-lsfml-graphics -lsfml-window -lsfml-system -lzmq -lv8
BIN=bin
SRC=src

OBJ_FILES = $(BIN)/time.o \
	   $(BIN)/event.o \
	   $(BIN)/eventhandler.o \
	   $(BIN)/mover.o \
	   $(BIN)/graphicsObject.o \
	   $(BIN)/spawnPoint.o \
	   $(BIN)/deathZone.o \
	   $(BIN)/sideBoundary.o \
	   $(BIN)/item.o \
	   $(BIN)/platform.o \
	   $(BIN)/character.o \
	   $(BIN)/collider.o \
	   $(BIN)/gameState.o \
	   $(BIN)/gameRunner.o \
	   $(BIN)/clientGameState.o \
	   $(BIN)/serverGameState.o \
	   $(BIN)/scriptManager.o

# time
$(BIN)/time.o: $(SRC)/Time/Timeline.cpp
	g++ -c $< -o $@

# event
$(BIN)/event.o: $(SRC)/Events/Event.cpp
	g++ -c $< -o $@

# event handler
$(BIN)/eventhandler.o: $(SRC)/Events/EventHandler.cpp
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

# SpawnPoint
$(BIN)/spawnPoint.o: $(SRC)/GraphicsObject/SpawnPoint.cpp
	g++ -c $< -o $@

# DeathZone
$(BIN)/deathZone.o: $(SRC)/GraphicsObject/DeathZone.cpp
	g++ -c $< -o $@

# DeathZone
$(BIN)/sideBoundary.o: $(SRC)/GraphicsObject/SideBoundary.cpp
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

# networking game
$(BIN)/gameState.o: $(SRC)/GameRunner/GameState.cpp
	g++ -c $< -o $@

$(BIN)/serverGameState.o: $(SRC)/GameRunner/ServerGameState.cpp
	g++ -c $< -o $@

$(BIN)/clientGameState.o: $(SRC)/GameRunner/ClientGameState.cpp
	g++ -c $< -o $@

$(BIN)/gameRunner.o: $(SRC)/GameRunner/GameRunner.cpp
	g++ -c $< -o $@

$(BIN)/server.o: $(SRC)/Network/Server.cpp
	g++ -c $< -o $@

$(BIN)/scriptManager.o: $(SRC)/Scripting/ScriptManager.cpp
	g++ -c $< -o $@

gameServer: $(OBJ_FILES) $(BIN)/server.o
	g++ $(OBJ_FILES) $(BIN)/server.o -o gameServer $(LIBS)

$(BIN)/client.o: $(SRC)/Network/Client.cpp
	g++ -c $< -o $@

gameClient: $(OBJ_FILES) $(BIN)/client.o
	g++ $(OBJ_FILES) $(BIN)/client.o -o gameClient $(LIBS)

debugServer: $(OBJ_FILES) $(BIN)/server.o
	g++ -g $(OBJ_FILES) $(BIN)/server.o -o gameServer $(LIBS)

debugClient: $(OBJ_FILES) $(BIN)/client.o
	g++ -g $(OBJ_FILES) $(BIN)/client.o -o gameClient $(LIBS)

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
	