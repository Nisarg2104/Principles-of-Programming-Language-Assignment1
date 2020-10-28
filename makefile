CC = gcc

compiler: driver.o typeExprTable.o parser.o grammar.o lexer.o
	$(CC) -o compiler driver.o typeExprTable.o parser.o grammar.o lexer.o

driver.o: driver.c typeExprTable.h parser.h grammar.h lexer.h
	$(CC) -c driver.c typeExprTable.h parser.h grammar.h lexer.h

typeExprTable.o: typeExprTable.c typeExprTable.h parser.h grammar.h lexer.h
	$(CC) -c typeExprTable.c typeExprTable.h parser.h grammar.h lexer.h

parser.o: parser.c parser.h grammar.h lexer.h
	$(CC) -c parser.c parser.h grammar.h lexer.h

grammar.o: grammar.c grammar.h lexer.h
	$(CC) -c grammar.c grammar.h lexer.h
	
lexer.o: lexer.c lexer.h
	$(CC) -c lexer.c lexer.h

