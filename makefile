SOURCES := $(wildcard src/*.cpp) $(wildcard src/slashcmd/*.cpp) $(wildcard src/msgcmd/*.cpp)

default:
	g++ -std=c++23 -o build/discord-bot $(SOURCES) -ldpp
