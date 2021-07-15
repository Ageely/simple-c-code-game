.DEFAULT_GOAL:= game

.PHONY:clean

main.o:prog.c
	gcc -c prog.c -o main.o

game:main.o
	gcc -o game main.o

clean:
	rm game main.o
