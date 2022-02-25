all: compiler

compiler: server.cpp
	@echo compilando piratas server...
	g++ server.cpp -o server -lwsock32