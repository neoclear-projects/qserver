OBJ = server.o src/db_access.o src/sqlite3.o src/sock_pool.o src/transmit.o		\
	  src/codec.o src/util.o src/response.o src/handle.o src/io.o

server: $(OBJ)
	clang++ -lpthread -lsqlite3 -ldl -o server $(OBJ)

server.o: server.cpp

src/db_access.o: src/db_access.cpp

src/sqlite3.o: src/sqlite3.c

src/sock_pool.o: src/sock_pool.cpp

src/transmit.o: src/transmit.cpp

src/codec.o: src/codec.cpp

src/util.o: src/util.cpp

src/response.o: src/response.cpp

src/handle.o: src/handle.cpp

src/io.o: src/io.cpp

.PHONY:
	clean

clean:
	-rm -rf app **.o server src/*.o