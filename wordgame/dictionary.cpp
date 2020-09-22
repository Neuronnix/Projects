#include <fstream>
#include <string>
#include <cassert>
#include <iostream>
#include <random>

#include "dictionary.hpp"


// dictionary::dictionary() : in(), size(0) {}

/**
 * @brief      Constructs a new instance
 *
 * @param[in]  dict_path  The dictionary path
 */
dictionary::dictionary(const std::string& dict_path)
: in(dict_path)
, size()
, rand_dev()
, generator(rand_dev())
, distr()
,
{
	if (!in.is_open())
	{
		throw std::runtime_error("Path to dictionary is invalid");
	}
    //Find size of dictionary
	init_size();
	init_random();
    //Pick random word from dictionary
    
    //dict pick random word
    
}


void dictionary::init_random()
{
	distr = std::uniform_int_distribution<int>(0, size);
}

/**
 * @brief      Return a random word from the dictionary
 *
 * @return     { description_of_the_return_value }
 */
const std::string dictionary::random_word()
{
	//Check in at beginning of file
	assert(in.tellg() == 0);
	const int index = distr(generator);
	std::string line;
	for (int i = 0; std::getline(in, line) && i < index; ++i)
		{;}

	if (i != index)
	{
		throw std::runtime_error("getline seemed to have failed before getting to index.");	
	}
	std::getline(in, line); //Get the word at the index.

	rewind_fstream(); //reset ifstream to beginning

	//return the word
	return line;
}



void dictionary::init_size()
{
    for (std::string line; std::getline(in, line); )
    {
        ++size;
    }
    rewind_fstream();
}

void dictionary::rewind_fstream()
{
	in.clear();
	in.seekg(0);
}