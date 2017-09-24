all: encode.c decode.c hashtable.c 
	gccx -o encode encode.c hashtable.c
	gccx -o decode decode.c 

encode: encode.c hashtable.c 
	gccx -o encode encode.c hashtable.c

decode: decode.c
	gccx -o decode decode.c

clean:
	rm encode decode 
