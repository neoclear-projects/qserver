server: server.o src/db_access.o src/sqlite3.o src/sock_pool.o
	clang++ -lpthread -lsqlite3 -ldl -o server server.o src/db_access.o src/sqlite3.o src/sock_pool.o

server.o: server.cpp

src/db_access.o: src/db_access.cpp

src/sqlite3.o: src/sqlite3.c

src/sock_pool.o: src/sock_pool.cpp

.PHONY:
	clean

clean:
	-rm -rf app **.o