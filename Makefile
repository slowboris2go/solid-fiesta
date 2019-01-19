
expander: src/main.c src/uba/uba.c src/tokenizer/tokenizer.c src/interpreter/interpreter.c
	gcc -o expander src/main.c src/uba/uba.c src/tokenizer/tokenizer.c src/interpreter/interpreter.c -I.

.PHONY: clean

clean:
	rm expander
