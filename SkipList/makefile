OBJS	= tests.o main.o
SOURCE	= main.cpp tests.cpp
OUT	= skiplist
CC	 = g++
FLAGS	 = -g -c -Wall -std=c++17

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

tests.o: tests.cpp
	$(CC) $(FLAGS) tests.cpp 

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 


clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT)