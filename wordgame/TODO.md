# CONVERT TO C++

- [ ] Change from pointers to references where possible
- [ ] Change from char * to strings
- [ ] Change the IO to streams
- [ ] Change arrays to vectors
- [ ] Change game_state to be class with class functions (mostly done)
- [ ] Refactor select and how sockets are read from
    - [ ] Remove allset global variable
    - [ ] Maybe move socket ops to their own class
- [ ] Split up code to be more modularised so it's not all annoying. Probably put game loop into a separate function or class/file.
- [ ] Change error handling to more C++ oriented
- [ ] 

# Finish implementing functionality

- [ ] Do basic outline
    - [ ] Set up listening on server
    - [ ] Accept client connection
        - [ ] Send welcome message
    - [ ] Ask for name
        - [ ] Wait for input kind of but go back to select in order to poll for other clients
- [ ] Have client/player struct/class to keep track of the state of the client's socket, whether they're entering name or guessing, so can store what's going on and then loop around and continue, so that I/O doesn't hang for other clients.
- [ ] If possible maybe change how FD stuff is read; may be different and cooler in C++
- [ ] 
- [ ]
- [ ] 