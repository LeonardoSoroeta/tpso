sockets:
	gcc -o server server.c commsockets.c marshalling.c databasecomm.c databasemarshalling.c semaphores.c 
	gcc -o client client.c commsockets.c marshalling.c
	gcc -o database database.c sqlite3.c databasecomm.c databasemarshalling.c semaphores.c -lpthread -ldl

fifos:
	gcc -o server server.c commfifos.c marshalling.c databasecomm.c databasemarshalling.c semaphores.c 
	gcc -o client client.c commfifos.c marshalling.c
	gcc -o database database.c sqlite3.c databasecomm.c databasemarshalling.c semaphores.c -lpthread -ldl

clean: 
	rm -f client server database
