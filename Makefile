build:
	gcc -Wall -std=gnu99 ./src/*.c -lSDL3 -o renderer

run:
	./renderer

debug:
	gcc -g -Wall -std=gnu99 ./src/*.c -lSDL3 -o renderer

clean:
	rm -f renderer
