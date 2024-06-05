all: myshell looper 

myshell: myshell.o LineParser.o
	gcc -g -Wall -m32 -o myshell myshell.o LineParser.o
    
myshell.o: myshell.c LineParser.h
	gcc -g -Wall -m32 -c -o myshell.o myshell.c

LineParser.o: LineParser.c LineParser.h
	gcc -g -Wall -m32 -c -o LineParser.o LineParser.c

looper: looper.o
	gcc -g -Wall -m32 -o looper looper.o

looper.o: looper.c
	gcc -g -Wall -m32 -c -o looper.o looper.c

.PHONY: clean

clean:
	rm -f *.o myshell looper
