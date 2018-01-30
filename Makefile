shell: main.o func.o
	gcc main.o func.o -o shell

main.o: main.c func.h
	gcc main.c -c -o main.o

func.o: func.c func.h
	gcc func.c -c -o func.o

clean: 
	rm -f shell main.o func.o