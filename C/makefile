CC=gcc

cn.exe:		main.o fcn.o
			${CC} -Wall main.o fcn.o -o cn.exe

fcn.o:		fcn.c		
			${CC} -Wall -c fcn.c -o fcn.o

main.o:		main.c
			${CC} -Wall -c main.c -o main.o
clean:
			rm *.o *.exe