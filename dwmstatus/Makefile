CC = tcc
LIBS = -lX11
FLAGS = -O3 -Wall -pipe -s
NAME = dwmstatus

# all: $(EXECS)

dwmstatus: $(NAME).c
	$(CC) $(FLAGS) -o $@ $? $(LIBS)

clean:
	rm $(NAME)

install:
	cp -i ./$(NAME) /usr/local/bin
