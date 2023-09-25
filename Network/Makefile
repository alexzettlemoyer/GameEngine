LIBS= -lzmq

server.o: Server.cpp
	g++ -c "Server.cpp" -o server.o

server: server.o
	g++ server.o -o server $(LIBS)

client.o: Client.cpp
	g++ -c "Client.cpp" -o client.o

client: client.o
	g++ client.o -o client $(LIBS)

clean:
	rm -f server.o client.o
	rm -f server client