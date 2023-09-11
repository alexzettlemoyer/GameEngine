LIBS=-lsfml-graphics -lsfml-window -lsfml-system
SRC=src
BIN=bin

SRCFILES = $(SRC)/Time/TimeHandler.cpp \
		$(SRC)/Movement/Mover.cpp \
		$(SRC)/GraphicsObject/GraphicsObject.cpp \
		$(SRC)/GraphicsObject/Item.cpp \
		$(SRC)/GraphicsObject/Platform.cpp \
		$(SRC)/GraphicsObject/Character.cpp \
		$(SRC)/Movement/Collider.cpp \
		$(SRC)/Draw/Draw.cpp \
		$(SRC)/io/ioHandler.cpp \
		$(SRC)/main.cpp

OBJS = $(BIN)/time.o \
	   $(BIN)/mover.o \
	   $(BIN)/graphicsObject.o \
	   $(BIN)/item.o \
	   $(BIN)/platform.o \
	   $(BIN)/character.o \
	   $(BIN)/collider.o \
	   $(BIN)/draw.o \
	   $(BIN)/ioHandler.o \
	   $(BIN)/main.o

# main copilation
$(BIN)/main.o: $(SRC)/main.cpp
	g++ -c $< -o $@

# draw compilation
$(BIN)/draw.o: $(SRC)/Draw/draw.cpp
	g++ -c $< -o $@

# time
$(BIN)/time.o: $(SRC)/Time/TimeHandler.cpp
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

# Link all object files to create the executable
game: $(OBJS)
	g++ -g $(OBJS) -o game $(LIBS)


clean:
	rm -f $(BIN)/*.o 
	rm -f ./game
	