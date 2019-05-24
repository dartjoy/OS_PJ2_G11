CC = g++
# SERVER_FOLDER = ./server
# CLIENT_FOLDER = ./client
# LIB_FOLDER = ./lib

# LINK_FILE = -l pthread
OUTPUT_DIR = output
CFLAGS = -Wall -ansi 
# CONFIG_FILE = config.h
# Wall: Show warning messages
# ansi: Enhance portability

FNAME = main_
SCH = fcfs sjr srtf priority rr
TAR = $(foreach n, $(SCH), $(FNAME)$(n))

all: $(TAR)

$(TAR): %: %.cpp
	$(CC) -o $(OUTPUT_DIR)/$@ $^




#$(LIB_FOLDER)/connection.o: $(LIB_FOLDER)/connection.h $(LIB_FOLDER)/connection.cpp 
#	$(CC) -c $(LIB_FOLDER)/connection.cpp 
 


clean:
	rm $(TAR)
