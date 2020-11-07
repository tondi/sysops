start: 
	# gcc -Wall -D st_mtim=st_mtimespec -o find find.c
	gcc -Wall -D st_mtim=st_mtimespec -o find_nftw find_nftw.c

generate:
	mkdir test
	# windows xp release date
	touch -a -m -t 200108240000 test_file
