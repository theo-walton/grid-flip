all:
	g++ *.cpp -o interpret -I . -std=c++17 -Wall -Werror -g -fsanitize=undefined -fsanitize=address
clean:
	rm interpret
