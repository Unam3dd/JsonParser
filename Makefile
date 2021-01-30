all:
	gcc -I includes src/jsonparser_memory.c src/jsonparser.c src/main.c -o main