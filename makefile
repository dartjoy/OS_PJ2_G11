CC = g++
# SERVER_FOLDER = ./server
# CLIENT_FOLDER = ./client
# LIB_FOLDER = ./lib

# LINK_FILE = -l pthread
CFLAGS = -Wall -ansi 
# CONFIG_FILE = config.h
# Wall: Show warning messages
# ansi: Enhance portability

#all:
#	g++ -o $(SERVER_FOLDER)/server $(SERVER_FOLDER)/server.cpp -l jsoncpp -o $(CLIENT_FOLDER)/client $(CLIENT_FOLDER)/client.cpp -l jsoncpp

all: main

main: main.cpp command.cpp scheduler_fcfs.cpp
	$(CC) -o main main.cpp

#$(LIB_FOLDER)/connection.o: $(LIB_FOLDER)/connection.h $(LIB_FOLDER)/connection.cpp 
#	$(CC) -c $(LIB_FOLDER)/connection.cpp 
 


clean:
	rm main
