CC = gcc
CFLAGS = -I utility
TARGETS = client server
OBJ_DIR = obj

all: $(TARGETS)

$(OBJ_DIR)/helper.o: utility/helper.c utility/helper.h | $(OBJ_DIR)
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJ_DIR)/server_helper.o: utility/server_helper.c utility/server_helper.h | $(OBJ_DIR)
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJ_DIR)/client.o: src/client.c utility/helper.h | $(OBJ_DIR)
	$(CC) -c $< -o $@

client: $(OBJ_DIR)/client.o $(OBJ_DIR)/helper.o
	$(CC) $^ -o $@

$(OBJ_DIR)/server.o: src/server.c utility/helper.h utility/server_helper.h | $(OBJ_DIR)
	$(CC) -c $< -o $@

server: $(OBJ_DIR)/server.o $(OBJ_DIR)/helper.o $(OBJ_DIR)/server_helper.o
	$(CC) $^ -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGETS)