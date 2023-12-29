all: serverG server client
serverG: serverG.c server.h
	g++ serverG.c -o serverG
server: serverC.c server.h
	g++ serverC.c -o server
client: client.c server.h
	g++ client.c -o client
clean:
	rm -f server
	rm -f serverG
	rm -f client