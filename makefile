myshell: myshell.o 
	gcc -m32 -g -Wall -o myshell myshell.o 

myshell.o: myshell.c LineParser.c LineParser.h
	gcc -g -Wall -m32 -c -o myshell.o myshell.c

.PHONY: clean 

clean:
	rm -f *.o myshell