compile: 
	gcc -Wall -o program program.c

generate:
	./program generate dane1 1000000 8192
	cp dane1 dane1_lib
	cp dane1 dane1_sys

sort_lib:
	time ./program sort dane1_lib 1000000 8192 lib

sort_sys:
	time ./program sort dane1_sys 1000000 8192 sys

copy_lib:
	time ./program copy dane1_lib dane2_lib 1000000 8192 lib

copy_sys:
	time ./program copy dane1_sys dane2_sys 1000000 8192 sys
