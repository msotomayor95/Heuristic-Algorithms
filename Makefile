CC = g++
CCFLAGS = -std=c++11
RM = rm -f

TARGET = tp3

SOURCE = LogicalBoard.cpp

all: $(TARGET)

tp3:
	$(CC) $(CCFLAGS) -o $(TARGET) $(SOURCE)


clean:
	$(RM) *.o
	$(RM) $(TARGET)
