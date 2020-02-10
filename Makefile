

srv: 
	gcc -o srv serv_02.c child_02.c

cli:
	gcc -o cli cliFTP.c

rm:
	rm cli srv

		
