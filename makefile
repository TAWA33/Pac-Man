CC = gcc
CFLAGS = -Wall -Werror -std=c99 $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf)
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf) -lm
TARGET = pacman
SRC = main.o algo.o affichage.o generate_lab.o

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC) $(TARGET)

.PHONY: all clean