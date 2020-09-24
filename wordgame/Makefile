PORT = 59532
FLAGS = -DPORT=$(PORT) -Wall -g

wordsrv : wordsrv.o socket.o gameplay.o dictionary.o
	g++ $(FLAGS) -o $@ $^

%.o : %.cpp socket.hpp gameplay.hpp dictionary.hpp debug.hpp
	g++ $(FLAGS) -c $<

clean : 
	rm *.o wordsrv
