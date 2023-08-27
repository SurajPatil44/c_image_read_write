#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra -std=c11 -Werror -pedantic -ggdb -I./stb_libs/"

SRC="./src/main.c"

cc $CFLAGS -o main $SRC -lm
