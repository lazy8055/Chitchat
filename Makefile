CC = gcc
CFLAGS = -I utility
TARGETS = client server
OBJ_DIR = obj

all: $(TARGETS)

$(OBJ_DIR)/helper.o: utility/helper.c utility/helper.h
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJ_DIR)/client.o: src/client.c utility/helper.h
	$(CC) -c $< -o $@

client: $(OBJ_DIR)/client.o $(OBJ_DIR)/helper.o
	$(CC) $^ -o $@

$(OBJ_DIR)/server.o: src/server.c utility/helper.h
	$(CC) -c $< -o $@

server: $(OBJ_DIR)/server.o $(OBJ_DIR)/helper.o
	$(CC) $^ -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGETS)
