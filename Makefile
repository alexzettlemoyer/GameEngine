LIBS=-lsfml-graphics -lsfml-window -lsfml-system
SRC=src
BIN=bin

# compile main.cpp
# g++ bin/main.o -o game -lsfml-graphics -lsfml-window -lsfml-system
$(BIN)/main.o: $(SRC)/main.cpp
	g++ -c "$(SRC)/main.cpp" -o $(BIN)/main.o

# draw compilation
$(BIN)/draw.o: $(SRC)/graphics/draw.cpp
	g++ -c "$(SRC)/graphics/draw.cpp" -o $(BIN)/draw.o

# collider
$(BIN)/collider.o: $(SRC)/Movement/Collider.cpp
	g++ -c "$(SRC)/Movement/Collider.cpp" -o $(BIN)/collider.o

# movement
$(BIN)/mover.o: $(SRC)/Movement/Mover.cpp
	g++ -c "$(SRC)/Movement/Mover.cpp" -o $(BIN)/mover.o

# GraphicsObject
$(BIN)/graphicsObject.o: $(SRC)/GraphicsObject/GraphicsObject.cpp
	g++ -c "$(SRC)/GraphicsObject/GraphicsObject.cpp" -o $(BIN)/graphicsObject.o

# character
$(BIN)/character.o: $(SRC)/GraphicsObject/Character.cpp
	g++ -c "$(SRC)/GraphicsObject/Character.cpp" -o $(BIN)/character.o

# platform
$(BIN)/platform.o: $(SRC)/GraphicsObject/Platform.cpp
	g++ -c "$(SRC)/GraphicsObject/Platform.cpp" -o $(BIN)/platform.o

# item
$(BIN)/item.o: $(SRC)/GraphicsObject/Item.cpp
	g++ -c "$(SRC)/GraphicsObject/Item.cpp" -o $(BIN)/item.o

# io handler
$(BIN)/ioHandler.o: $(SRC)/io/ioHandler.cpp
	g++ -c "$(SRC)/io/ioHandler.cpp" -o $(BIN)/ioHandler.o

# Link all object files to create the executable
game: 	$(BIN)/collider.o $(BIN)/mover.o $(BIN)/graphicsObject.o $(BIN)/item.o $(BIN)/platform.o $(BIN)/character.o $(BIN)/draw.o $(BIN)/ioHandler.o $(BIN)/main.o
	g++ -g $(BIN)/collider.o $(BIN)/mover.o $(BIN)/graphicsObject.o $(BIN)/item.o $(BIN)/platform.o $(BIN)/character.o $(BIN)/draw.o $(BIN)/ioHandler.o $(BIN)/main.o -o game $(LIBS)


clean:
	rm -f $(BIN)/main.o 
	rm -f $(BIN)/draw.o 
	rm -f $(BIN)/mover.o 
	rm -f $(BIN)/ioHandler.o
	rm -f $(BIN)/graphicsObject.o 
	rm -f $(BIN)/item.o 
	rm -f $(BIN)/character.o 
	rm -f $(BIN)/platform.o
	rm -f $(BIN)/collider.o 

	rm -f ./game


# g++ -c "src/main.cpp" -o bin/main.o
# g++ bin/main.o -o game -lsfml-graphics -lsfml-window -lsfml-system
