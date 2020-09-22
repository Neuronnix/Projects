#ifndef DICT_HPP
#define DICT_HPP

#include <fstream>
#include <string>

// Information about the dictionary used to pick random word
class dictionary {
public:
    ifstream in;
    int size; //Stores number of words/lines in the dictionary

    //Random number generation for picking random word
    std::random_device rand_dev;
    std::mt19937 generator;
    std::uniform_int_distribution<int> distr;
    dictionary();
    dictionary(std::string dict_path);
    std::string random_word(void);
    int next_random_index(void);
private:
    void init_size(void);
    void init_random(void);
    void rewind_fstream(void);
};

#endif	