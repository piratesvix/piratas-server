all: compiler

compiler: server.cpp
	@echo compilando Piratas Server...
	g++ server.cpp -o piratas-server-1.0-chat -lwsock32
# gcc server.c -c <nome-do-executável>