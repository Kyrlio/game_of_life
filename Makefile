build:
	gcc -Wall -std=c99 ./src/*c -o game -lSDL2 -lm

run:
	./game

clean:
	rm game