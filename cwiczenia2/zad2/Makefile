compile: 
	gcc -Wall -D st_mtim=st_mtimespec -o find find.c
	gcc -Wall -D st_mtim=st_mtimespec -o find_nftw find_nftw.c

test_implementations:
	./find . '=' '00:00:00 24-08-2001'
	./find_nftw . '=' '00:00:00 24-08-2001'
	# ./find . '>' '00:00:00 24-08-2001'
	# ./find_nftw . '>' '00:00:00 24-08-2001'
	# ./find . '<' '00:00:00 24-08-2021'
	# ./find_nftw . '<' '00:00:00 24-08-2021'

generate:
	mkdir test
	# windows xp release date
	touch -a -m -t 200108240000 test_file
