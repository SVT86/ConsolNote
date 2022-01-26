CC=g++

cn.exe:		main.o fcn.o
			${CC} main.o fcn.o -o cn.exe

fcn.o:		fcn.c		
			${CC} -c fcn.c -o fcn.o

main.o:		main.c
			${CC} -c main.c -o main.o
clean:
			rm *.o *.exe