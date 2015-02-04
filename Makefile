objects = main.o lex.o recursyn.o llsyn.o public.o stack.o
CFLAGS = -Wall -g 
main: $(objects)
	cc $(CFLAGS) $(objects) -o main
main.o: snl.h
lex.o: snl.h
recursyn.o: snl.h
llsyn.o: snl.h
public.o: snl.h
stack.o: snl.h
clean:
	rm $(objects) main 
