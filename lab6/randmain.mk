/*
Assignment 6: Dynamic Linking
Grade Received: 90/100
If you found this file helpful, please consider reaching out to me:
	Website: faithtwardzik.com
	Insta: @faithtwardzik
*/

randcpuid.o: randcpuid.h randcpuid.c
	$(CC) $(CFLAGS) -c randcpuid.c -o randcpuid.o

randmain.o: randmain.c randcpuid.h
	$(CC) $(CFLAGS) -c randmain.c -o randmain.o

randlibsw.o: randlibsw.c randlib.h
	$(CC) $(CFLAGS) -c -fPIC randlibsw.c -o randlibsw.o
randlibsw.so: randlibsw.o
	$(CC) $(CFLAGS) -shared randlibsw.o -o randlibsw.so

randlibhw.o: randlibhw.c randlib.h
	$(CC) $(CFLAGS) -c -fPIC randlibhw.c -o randlibhw.o
randlibhw.so: randlibhw.o
	$(CC) $(CFLAGS) -shared randlibhw.o -o randlibhw.so

randmain: randmain.o randcpuid.o 
	ar -cvq rand.a randcpuid.o 
	$(CC) $(CFLAGS) -o randmain randmain.o rand.a -ldl -Wl,-rpath=$PWD 
