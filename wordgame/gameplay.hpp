#ifndef GAME_HPP
#define GAME_HPP

#include <netinet/in.h>
#include <string>
#include <fstream>

#include "dictionary.hpp"

#define MAX_NAME 30  
#define MAX_MSG 128
#define MAX_WORD 20
#define MAX_BUF 256
#define MAX_GUESSES 4
#define NUM_LETTERS 26
#define WELCOME_MSG "Welcome to our word game. What is your name?\r\n"



class Game {
public:
    class dictionary dict;
    std::string word;      // The word to guess
    std::string guess;     // The current guess (for example '-o-d')
    int letters_guessed[NUM_LETTERS]; // Index i will be 1 if the corresponding
                                      // letter has been guessed; 0 otherwise
    int guesses_left;         // Number of guesses remaining
    
    struct client *head;
    struct client *has_next_turn;

    void init_game(Game &game, std::string dict_name);
    int get_file_length(std::string filename);
    std::string status_message(std::string msg, Game &game);

    Game();
    Game(std::string);
};

struct client {
    int fd;
    struct in_addr ipaddr;
    struct client *next;
    char name[MAX_NAME];
    char inbuf[MAX_BUF];  // Used to hold input from the client
    char *in_ptr;         // A pointer into inbuf to help with partial reads
};
typedef struct client Client;

#endif