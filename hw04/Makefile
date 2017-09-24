
CFLAGS = -std=c99 -Wall -Wextra -pedantic

all: bits encode_bits decode_bits getnum tobinary todecimal tooctal tohex

tohex: tohex.c getnum.o
	gcc $(CFLAGS) -o tohex tohex.c getnum.o -lm

tooctal: tooctal.c getnum.o
	gcc $(CFLAGS) -o tooctal tooctal.c getnum.o -lm

todecimal: todecimal.c getnum.o
	gcc $(CFLAGS) -o todecimal todecimal.c getnum.o -lm

tobinary: tobinary.c bits.o getnum.o
	gcc $(CFLAGS) -o tobinary tobinary.c bits.c getnum.o -lm

getnum: getnum.c
	gcc $(CFLAGS) -c getnum.c -lm

bits: bits.c
	gcc $(CFLAGS) -c bits.c

encode_bits: bits.o encode_bits.c
	gcc $(CFLAGS) -o encode_bits encode_bits.c bits.c

decode_bits: bits.o decode_bits.c
	gcc $(CFLAGS) -o decode_bits decode_bits.c bits.c

clean: 
	rm -f *.o encode_bits decode_bits getnum tobinary todecimal tooctal tohex
