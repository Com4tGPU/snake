CXX ?= c++

main: main.cpp
	$(CXX) -Wall -Wextra main.cpp -o main

clean:
	rm -f main

.PHONY: clean
