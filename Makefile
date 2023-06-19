CFLAGS = -std=c99 -pedantic -Wall -Wextra -Werror
CPPFLAGS = -ldl
OBJ = src/main.o
BIN = $(OBJ:.o=)
TARGET = main
LIBOBJ = src/test.o
SHARED = libtest.so

all: $(BIN) $(SHARED)
	@mv $(BIN) $(TARGET)

$(BIN): $(OBJ)

$(SHARED): $(LIBOBJ)
	$(CC) -shared -fPIC -o $@ $^

reload:
	@rm src/test.o
	@mv src/test.c src/test
	@mv src/test_tmp.c src/test.c
	@mv src/test src/test_tmp.c
	@make libtest.so

clean:
	$(RM) $(TARGET) $(BIN) $(OBJ) $(SHARED) $(LIBOBJ)
