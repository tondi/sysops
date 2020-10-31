compile: 
	gcc -Wall -o program program.c
	# echo '' > wyniki

generate:
	./program generate dane1 2500 8192
	cp dane1 dane1_lib
	cp dane1 dane1_sys

sort_lib:
	# printf "sort lib 2000 1\n" >> wyniki
	time ./program sort dane1_lib 2500 8192 lib

sort_sys:
	# printf "sort sys\n" >> wyniki
	# printf "sort sys 2000 1\n" >> wyniki
	time ./program sort dane1_sys 2500 8192 sys

run_sort_lib:
	make compile
	make generate
	cp dane1 dane1_lib
	make sort_lib

run_sort_sys:
	make compile
	make generate
	cp dane1 dane1_sys
	make sort_sys