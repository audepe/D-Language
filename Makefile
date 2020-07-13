compile:
	$(MAKE) clean
	mkdir build
	$(MAKE) bison
	$(MAKE) flex
	$(MAKE) gcc

clean:
	rm -rf d_code ./build &> /dev/null
	
bison:
	bison -v -d -t flex_bison/d_bison.y
	mv *.tab.* ./build
	mv *.output ./build

flex:
	flex flex_bison/d_flex.l
	mv *.yy.c ./build

gcc:
	gcc -o d_code build/d_bison.tab.c build/lex.yy.c symbol_table/d_symbols.c -lfl -lm