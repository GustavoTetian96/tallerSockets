all: bin/cliente bin/servidor

bin/cliente: obj/cliente.o
	mkdir -p bin
	gcc -Wall obj/cliente.o -o bin/cliente
bin/servidor: obj/servidor.o
	gcc -Wall obj/servidor.o -o bin/servidor
obj/cliente.o: src/cliente.c
	mkdir -p obj
	gcc -Wall -c src/cliente.c -o obj/cliente.o
obj/servidor.o: src/servidor.c
	gcc -Wall -c src/servidor.c -o obj/servidor.o
.PHONY: clean
clean: 
	rm -f obj/* bin/*
