CC = gcc
CFLAGS = -I include
LDFLAGS = -L lib
LIBS = -lmingw32 -lSDL2main -lSDL2
LINKs = -mwindows

SRC_DIR = src
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
EXECUTABLE = $(BIN_DIR)/prog

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS) $(LDFLAGS) $(LIBS) $(LINKs)
	./$(EXECUTABLE)
clean:
	rm -f $(EXECUTABLE)
exe:
	./$(EXECUTABLE)
.PHONY: all clean
