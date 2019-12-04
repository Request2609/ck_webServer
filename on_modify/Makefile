#
#  ======== makefile ========
#
CC = g++

CFLAGS = -lpthread -g -std=c++11 -w

TARGET = Server

SrcFile = $(wildcard *.cpp)

ObjFile = $(patsubst %.cpp,%.o,$(SrcFile))

%.o:%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET):$(ObjFile)
	@$(CC) $(ObjFile) -o $(TARGET) $(CFLAGS) 

.PHONY:
	clean

clean:
	rm -rf *.o Server
