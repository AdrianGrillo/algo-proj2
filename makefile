rwmain: main.cpp
	g++ -Wall -std=c++11 main.cpp -o main

clean:
	rm -rf main output.txt
