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
bin/listar_dir:
	gcc -Wall ejemplos/listar_contenido_directorio.c -o bin/listar_dir #para resultado de funcionamiento
run_servidor:
	./bin/servidor 127.0.0.1 9500 ./data/
run_cliente:
	./bin/cliente 127.0.0.1 9500
.PHONY: clean
clean: 
	rm -f obj/* bin/*
