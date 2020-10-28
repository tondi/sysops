#!/usr/bin/env bash
gcc program.c -o program -Wall

echo '' > wyniki.txt

function report {
    printf "######################\n$1 rekordów $2 bajty" >> wyniki.txt
    ./program generate dane $1 $2
    cp dane dane.bak
    printf "\n| sort sys" >> wyniki.txt
    { time ./program sort dane $1 $2 sys; } 2>> wyniki.txt
    cp dane.bak dane
    printf "\n| sort lib" >> wyniki.txt
    { time ./program sort dane $1 $2 lib; } 2>> wyniki.txt
    cp dane.bak dane
    printf "\n| copy sys" >> wyniki.txt
    { time ./program copy dane dane_cp_sys $1 $2 sys; } 2>> wyniki.txt
    printf "\n| copy lib" >> wyniki.txt
    { time ./program copy dane dane_cp_lib $1 $2 lib; } 2>> wyniki.txt
    printf "\n\n" >> wyniki.txt
    rm dane_cp_sys dane_cp_lib dane dane.bak
}

report 10 4
#report 2000 4
#report 1000 512
#report 2000 512
#report 500 4096
#report 1000 4096
#report 500 8192
#report 1000 8192
