

all: 
	gcc -o srv srv_02.c child_02.c send_file.c
	gcc -o cli cli_02.c send_file.c

srv: 
	gcc -o srv srv_02.c child_02.c send_file.c

cli:
	gcc -o cli cli_02.c send_file.c

rm:
	rm cli srv

		
