sockets:
	gcc -o server server.c commlib.c daemonqueues.c commsockets.c marshalling.c databasecomm.c databasemarshalling.c semaphores.c 
	gcc -o client client.c commlib.c commsockets.c marshalling.c
	gcc -o database database.c sqlite3.c databasecomm.c databasemarshalling.c semaphores.c -lpthread -ldl
	gcc -o loggingdaemon commlib.c daemonqueues.c daemonserver.c

fifos:
	gcc -o server server.c commlib.c daemonqueues.c commfifos.c marshalling.c databasecomm.c databasemarshalling.c semaphores.c 
	gcc -o client client.c commlib.c commfifos.c marshalling.c
	gcc -o database database.c sqlite3.c databasecomm.c databasemarshalling.c semaphores.c -lpthread -ldl
	gcc -o loggingdaemon commlib.c daemonqueues.c daemonserver.c

clean: 
	rm -f client server database loggingdaemon database.db
