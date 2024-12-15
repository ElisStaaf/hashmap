BIN = test
SRC = test.cpp

OBJ = $(SRC:.cpp=.o)
RM = rm -f
all: $(OBJ) 
		g++ $(OBJ) -g -o $(BIN)

clean:
		-$(RM) *.o
		-$(RM) *.core
