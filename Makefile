SRC_DIR = src/SDL2_game
CC = g++

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) src/glad.c
OBJ_NAME = play

INCLUDE_PATHS = -Iinclude -Iinclude/SDL2 -Iinclude/SDL2_image -Iinclude/glad
LIBERY_PATHS = -Llib/SDL2 -Llib/SDL2_image

CFLAGS = -std=c++11 -Wall -O0 -g
LDFLAGS = -lSDL2 -lSDL2_image -framework OpenGL

all:
	$(CC) $(SRC_FILES) -o $(OBJ_NAME) $(CFLAGS) $(INCLUDE_PATHS) $(LIBERY_PATHS) $(LDFLAGS)
