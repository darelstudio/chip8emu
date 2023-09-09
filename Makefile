FLAGS= -g

all:
	g++ ${FLAGS} -Iinclude src/*.cpp -Llib -lmingw32 -lSDL2main -lSDL2 -o bin/Main