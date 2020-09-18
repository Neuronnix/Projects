PORT = 59532
FLAGS = -DPORT=$(PORT) -Wall -g

wordsrv : wordsrv.o socket.o gameplay.o
	g++ $(FLAGS) -o $@ $^

%.o : %.cpp socket.hpp gameplay.hpp
	g++ $(FLAGS) -c $<

clean : 
	rm *.o wordsrv
