LDFLAGS += $(shell pkg-config --libs jansson libcurl)
CFLAGS += -Wall -Wextra -Wpedantic -s -O3 -pipe --std=c99 -D_DEFAULT_SOURCE -g
EXEC_NAME = forecast

SRC = src/curl.c src/args.c src/json.c src/main.c

PREFIX = /usr/local

all: clean options ${EXEC_NAME}

options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

${EXEC_NAME}:
	${CC} ${SRC} ${CFLAGS} ${LDFLAGS} -o ${EXEC_NAME}

clean:
	rm -f ${EXEC_NAME}
	rm -f ./*.o

install:
	cp -i ${EXEC_NAME} ${PREFIX}/bin

uninstall:
	rm -i ${PREFIX}/bin/${EXEC_NAME}
