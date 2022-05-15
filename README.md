# tallerSockets
version 0: 
	servidor corre en 192.168.1.100	
	./bin/cliente 192.168.1.100 7640 "hola servidor saludame"
	./bin/servidor 192.168.1.100 7640
version 1:
	./bin/servidor 192.168.1.8 9500 ./
	./bin/cliente 192.168.1.8 9500
