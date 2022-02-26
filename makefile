all: compiler

compiler: server.cpp server.c
	@echo compilando piratas server...
	g++ server.cpp -o server -lwsock32