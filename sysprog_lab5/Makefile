calculator: calculator.l calculator.y calculator.h
	bison -d -Wcounterexamples calculator.y
	flex -o calculator.lex.c calculator.l
	gcc -o $@ -Wno-implicit-function-declaration calculator.tab.c calculator.lex.c calculator.c -lfl

run:
	./calculator tests/1.txt
	./calculator tests/2.txt
	./calculator tests/3.txt
	./calculator tests/4.txt
	./calculator tests/5.txt

clean:
	rm -f calculator calculator.tab.c calculator.tab.h calculator.lex.c