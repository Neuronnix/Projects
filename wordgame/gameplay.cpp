#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>

#include "debug.hpp"
#include "gameplay.hpp"


Game::Game(const std::string& dict_name)
    : dict(dict_name)
    , word(dict.random_word())
    , guess(word.length(), '-')
    , letters_guessed{}
    , guesses_left(MAX_GUESSES)
    , head(nullptr)
    , has_next_turn(nullptr)
{
    
}

/* Return a status message that shows the current state of the game.
 * Assumes that the caller has allocated MAX_MSG bytes for msg.
 */
const char* Game::status_message(char *msg) {
    sprintf(msg, "***************\r\n"
           "Word to guess: %s\r\nGuesses remaining: %d\r\n"
           "Letters guessed: \r\n", guess.c_str(), guesses_left);
    for(int i = 0; i < 26; i++){
        if(letters_guessed[i]) {
            int len = strlen(msg);
            msg[len] = (char)('a' + i);
            msg[len + 1] = ' ';
            msg[len + 2] = '\0';
        }
    }
    strncat(msg, "\r\n***************\r\n", MAX_MSG);
    return msg;
}

#if 0
/* Initialize the gameboard: 
 *    - initialize dictionary
 *    - select a random word to guess from the dictionary file
 *    - set guess to all dashes ('-')
 *    - initialize the other fields
 * We can't initialize head and has_next_turn because these will have
 * different values when we use init_game to create a new game after one
 * has already been played
 */
void Game::init_game(Game &game, std::string dict_name) {
    char buf[MAX_WORD];
    if(game->dict.fp != NULL) {
        rewind(game->dict.fp);
    } else {
        game->dict.fp = fopen(dict_name, "r");
        if(game->dict.fp == NULL) {
            perror("Opening dictionary");
            exit(1);
        }
    } 

    int index = random() % game->dict.size;
    printf("Looking for word at index %d\n", index);
    for(int i = 0; i <= index; i++) {
        if(!fgets(buf, MAX_WORD, game->dict.fp)){
            fprintf(stderr,"File ended before we found the entry index %d",index);
            exit(1);
        }
    } 

    // Found word
    if(buf[strlen(buf) - 1] == '\n') {  // from a unix file
        buf[strlen(buf) - 1] = '\0';
    } else {
        fprintf(stderr, "The dictionary file does not appear to have Unix line endings\n");
    }
    strncpy(game->word, buf, MAX_WORD);
    game->word[MAX_WORD-1] = '\0';
    for(size_t j = 0; j < strlen(game->word); j++) {
        game->guess[j] = '-';
    }
    game->guess[strlen(game->word)] = '\0';

    for(int i = 0; i < NUM_LETTERS; i++) {
        game->letters_guessed[i] = 0;
    }
    game->guesses_left = MAX_GUESSES;

}
#endif