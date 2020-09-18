# Multiplayer Word Guessing Game

An online hangman-type of game made using C++.

The game allows multiple people to join, and each take turns trying to guess a word.

The game is run from the command line, and players can connect to the server using `nc <server address> <port>`.

E.g. you can run the program via `wordsrv dictionary.txt`, and then connect to it in a different terminal via `nc localhost PORT`, depending on which port the server runs on.