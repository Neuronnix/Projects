#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <iostream>

#include "debug.hpp"
#include "socket.hpp"
#include "gameplay.hpp"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#ifndef PORT
    #define PORT 59531
#endif
#define MAX_QUEUE 5


void add_player(Client **top, int& fd, struct in_addr& addr);
void remove_player(Client **top, int fd);

/* These are some of the function prototypes that we used in our solution 
 * You are not required to write functions that match these prototypes, but
 * you may find the helpful when thinking about operations in your program.
 */
/* Send the message in outbuf to all clients */
int broadcast(Game& game, char *outbuf);
void announce_turn(Game *game);
void announce_winner(Game *game, Client *winner);
/* Move the has_next_turn pointer to the next active client */
void advance_disconnect(Game& game);
void advance_turn(Game& game);

//My helper functions.
void notify_turn(Client *p, Game& game);
void invalid_guess(Client *p, Client **head);
void msg_player(Client *p, Client **head, const char *msg);
void notify_guess(Client *p, Client **head);
int read_active(Client *p, Client **head);
void status_to_client(Client *p, Game& game);
void welcome_player(Client *p, Game& game);
void notify_incorrect_name(Client *p, Client **new_players, const char *reason);
int read_input(Client *p, int max_chars, const char *err_msg);
int get_player_name(Client *p);
void checkError(const int value, const char *err_msg);
int read_wrapper(const int fd, void *buf, const size_t count, const char *msg);
void print_list_fd(Client **list);
void add_to_list(Client *p, Client **list);
void remove_from_list(Client *p, Client **list);
void add_player_to_game(Client *p, Client **new_players, Client **head, Client **next_turn);
int is_valid_guess(char *guess);
void not_your_turn(Client *p, Client **head);
void announce_guess(Client *p, Game& game);
/* The set of socket descriptors for select to monitor.
 * This is a global variable because we need to remove socket descriptors
 * from allset when a write to a socket fails.
 */

//So basically this is global because of the bad design of how we are doing it when socket reads fail
fd_set allset;



void advance_disconnect(Game& game)
{
    if (game.has_next_turn == NULL)
    {
        advance_turn(game);
    }
}

void advance_turn(Game& game)
{
    game.has_next_turn = game.head;
}

void status_to_client(Client *p, Game& game)
{
    //Function that sends status message to a single client.
    char msg[MAX_MSG];
    msg_player(p, &(game.head), game.status_message(msg));
}

int broadcast(Game& game, char *outbuf)
{
    Client *p = game.head;
    for ( ; p; p = p->next)
    {
        //Broadcast here;
        if (dprintf(p->fd, "%s\r\n", outbuf) < 0)
        {
            //Error writing to client.
            fprintf(stderr, "Broadcast to client %s failed\r\n", inet_ntoa(p->ipaddr));
            advance_disconnect(game);
            remove_player(&(game.head), p->fd);
            return -1;
            //Figure out how to finish broadcasting to rest of the players
            //that are still here without repeating messages.
        }
    }
    return 0;
}

/* Add a client to the head of the linked list
 */
void add_player(Client **top, int& fd, struct in_addr& addr) 
{
    Client *p = (Client*)malloc(sizeof(Client));

    if (!p) {
        perror("malloc");
        exit(1);
    }

    cout << "Adding client " << inet_ntoa(addr) << endl;

    p->fd = fd;
    p->ipaddr = addr;
    p->name[0] = '\0';
    p->in_ptr = p->inbuf;
    p->inbuf[0] = '\0';
    p->next = *top;
    *top = p;
}

/* Removes client from the linked list and closes its socket.
 * Also removes socket descriptor from allset 
 */
void remove_player(Client **top, int fd) {
    Client **p;

    for (p = top; *p && (*p)->fd != fd; p = &(*p)->next)
        ;
    // Now, p points to (1) top, or (2) a pointer to another client
    // This avoids a special case for removing the head of the list
    if (*p) {
        Client *t = (*p)->next;
        printf("Removing client %d %s\n", fd, inet_ntoa((*p)->ipaddr));
        FD_CLR((*p)->fd, &allset);
        close((*p)->fd);
        free(*p);
        *p = t;
    } else {
        BUGF("Trying to remove fd %d, but I don't know about it\n",
                 fd);
    }
}

void setup_sighandler()
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if(sigaction(SIGPIPE, &sa, NULL) == -1) 
    {
        BUGF();
        perror("sigaction");
        exit(1);
    }
}

int main(int argc, const char **argv) {
    int clientfd;
    int nready;

    //Check arguments are correct
    if(argc != 2) {
        FATAL("Usage: %s <dictionary filename>\n", argv[0]);
    }

    //Setting up signal handler for SIGPIPE why?

    setup_sighandler();
    
    Game game((std::string(argv[1])));

    Client *new_players = nullptr;

    //Initialize the server.
    Server server(PORT, MAX_QUEUE);

    while (true) 
    {

        //Do select
        nready = server.select();
        if (nready == -1) 
        {
            perror("select");
            continue;
        }
        //
        if (server.has_new_connection())
        {

            //Accept new client and get the socket/fd for that client
            struct sockaddr_in peer;
            peer.sin_family = AF_INET;
            clientfd = server.accept_connection(peer);

            // Client c = new Client(clientfd, peer);
            //welcome_message()

            //Instantiate a player here with the clientfd.


            if(write(clientfd, WELCOME_MSG, strlen(WELCOME_MSG)) == -1) {
                BUGF("Write to client %s failed\n", inet_ntoa(peer.sin_addr));
            };
            add_player(&new_players, clientfd, peer.sin_addr);
        }

        //Loop through fds and check state of each client.
    }
    return 0;
}




void not_your_turn(Client *p, Client **head)
{
    if (strcmp(p->inbuf, ""))
    {
        msg_player(p, head, "It's not your turn to guess.");
    }
}

int is_valid_guess(char *guess)
{
    return strcmp(guess, "") && strlen(guess) == 1 
            && *guess >= 'a' && *guess <= 'z';
}

int read_active(Client *p, Client **head)
{
    int ret;
    if ((ret = read_input(p, MAX_BUF, "read: active player")))
    {
        switch (ret)
        {
            case -1:
                fprintf(stderr, "Error reading client [%d] name\r\n", p->fd);
                remove_player(head, p->fd);
                break;
            case 1:
                printf("[%d] No network new line\n", p->fd);
                break;
            case -2:
                fprintf(stderr, "[%d] EOF; client dc\n", p->fd);
                remove_player(head, p->fd);
        }
    }
    return ret;
}

//Welcome player p to game.
//Broadcast their name and tell them the game status.
void welcome_player(Client *p, Game& game)
{
    char msg[MAX_MSG];
    strcpy(msg, p->name);
    broadcast(game, strcat(msg, " has just joined.\r\n"));
    printf("%s\n", msg);
    status_to_client(p, game);

    notify_guess(game.has_next_turn, &(game.head));
    if (p != game.has_next_turn)
    {
        notify_turn(p, game);
    }
}

void invalid_guess(Client *p, Client **head)
{
    msg_player(p, head, "Please enter a single lowercase character.");
}

void notify_guess(Client *p, Client **head)
{
    msg_player(p, head, "Your guess?");
}

void announce_guess(Client *p, Game& game)
{
    char msg[MAX_MSG];
    sprintf(msg, "%s guessed %c", p->name, *p->inbuf);
    broadcast(game, msg);
}

void announce_turn(Game &game)
{
    char msg[MAX_MSG];
    sprintf(msg, "It's %s's turn.", game.has_next_turn->name);
    broadcast(game, msg);
}

void notify_turn(Client *p, Game& game)
{
    char msg[MAX_MSG];
    sprintf(msg, "It's %s's turn.", game.has_next_turn->name);
    msg_player(p, &(game.head), msg);
}

void msg_player(Client *p, Client **head, const char *msg)
{
    //Send message to the socket of the player.
    //Something about checking if socket is closed
    if (dprintf(p->fd, "%s\r\n", msg) < 0)
    {
        fprintf(stderr, "[%d] Failed to msg client %s\nMessage: %s", p->fd, inet_ntoa(p->ipaddr), msg);
        remove_player(head, p->fd);
    }
    memset(p->inbuf, 0, MAX_BUF);
}

void add_to_list(Client *p, Client **head)
{
    if (!*head)
    {
        //List empty: set p as head.
        *head = p;
    }
    else
    {
        //More logic.
        p->next = *head;
        *head = p;
    }
}

void print_list_fd(Client **list)
{
    for ( ; *list ; list = &(*list)->next)
    {
        printf("[%d] Client \n", (*list)->fd);
    }
    printf("End\n");
}

void remove_from_list(Client *p, Client **list)
{
    //printf("Removing a client from list\n");
    //Client **tmp = list;
    //print_list_fd(tmp);
    for ( ; *list && *list != p; list = &(*list)->next)
        ;

    if (*list)
    {
        //Hopefully this works lol I don't have a clue.
        Client *t = (*list)->next;
        *list = t;
    }

    //printf("New list\n");
    //print_list_fd(tmp);

}

void add_player_to_game(Client *p, Client **new_players, Client **head, Client **next_turn)
{
    //printf("Adding player to game.\n");
    remove_from_list(p, new_players);
    //Client **tmp = head;
    //printf("Active players\n");
    //print_list_fd(tmp);
    if (!*head)
    {
        //First player to enter; has next turn and is head.
        *head = p;
        *next_turn = p;
        p->next = NULL;
    }
    else
    {
        //More logic.
        p->next = *head;
        *head = p;
    }
    //printf("Added player to active players.\n");
    //printf("New active players\n");
   // print_list_fd(tmp);
    //Then we want to broadcast the message that
}

void notify_incorrect_name(Client *p, Client **new_players, const char *reason)
{
    if (dprintf(p->fd, "Invalid name: %s\r\n"
        "Please enter a valid name\r\n", reason) < 0)
    {
        //Error writing to client.
        fprintf(stderr, "Write to client %s failed: notify incorrect name\r\n", inet_ntoa(p->ipaddr));
        remove_player(new_players, p->fd);
    }
}


int read_input(Client *p, int max_chars, const char *err_msg)
{
    int num_chars = 0, err;
    char *rn_found; //if \r\n is in the buffer.
    p->inbuf[num_chars] = '\0';
    //printf("Reading input from [%d] \n", p->fd);
    while((rn_found = strstr(p->inbuf, "\r\n")) == NULL && num_chars < max_chars)
    {
        //
        err = read_wrapper(p->fd, &p->inbuf[num_chars], max_chars-num_chars, err_msg);
        printf("[%d] Read %d bytes\n", p->fd, err);
        if (err == -1)
        {
            return -1;
        }
        else if (err == 0)
        {
            return -2;
        }
        num_chars += err;
        p->inbuf[num_chars] = '\0';
    }

    if (rn_found)
    {
        //Terminate before the network newline.
        p->inbuf[(rn_found-p->inbuf)] = '\0';
    }
    else
    {
        //No network newline: invalid name.
        memset(p->inbuf, 0, MAX_BUF);
        return 1;
    }
    return 0;
}

int get_player_name(Client *p)
{
    return read_input(p, MAX_NAME, "read: name");
}

/*
Boilerplate for perror. Value is passed as a boolean: functions that 
return 0 as success as be passed as is, but pointers must be inverted.
*/

void checkError(const int value, const char *err_msg)
{
    if (value)
    {
        perror(err_msg);
    }
}

int read_wrapper(const int fd, void *buf, const size_t count, const char *msg)
{
    int ret = read(fd, buf, count);
    checkError(ret == -1, msg);
    return ret;
}

