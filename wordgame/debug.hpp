#ifndef DEBUG_H
#define DEBUG_H

//Debug print macro functions from here
//https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c

#define DEBUG 0
#include <cstdio>

using std::fprintf;
using std::vfprintf;
using std::getchar;

#define DEBUGDO if (DEBUG)

#define BUG(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#define BUGV(fmt, v_args) vfprintf(stderr, fmt, v_args)
#define BUGF(fmt, ...) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                              __LINE__, __func__, ##__VA_ARGS__)

#define FATAL(fmt, ...) do { BUG(fmt, ##__VA_ARGS__); \
exit(EXIT_FAILURE); } while(0)
#define FATALF(fmt, ...) do { BUGF(fmt, ##__VA_ARGS__); \
exit(EXIT_FAILURE); } while(0)
#define FATALV(fmt, v_args) do { BUGV(fmt, v_args); \
exit(EXIT_FAILURE); } while(0)

#define rip() exit(EXIT_FAILURE)
#define die() exit(EXIT_FAILURE)
#define LOG(fmt, ...) fprintf(stdout, fmt, ##__VA_ARGS__)

#define PAUSE() \
        do {  getchar(); } while (0)

#define INT() \
        do { \
    		LOG("press c to continue\n"); \
    		int c = getchar(); \
    		while(c != 'c') \
    		{ \
    			c = getchar(); \
    		} \
        } while(0) 

#define DPAUSE() \
        do { if (DEBUG) getchar(); } while (0)
#define DINT() \
        do { \
        	if (DEBUG) \
        	{ \
        		DLOG("press c to continue\n"); \
        		int c = getchar(); \
        		while(c != 'c') \
        		{ \
        			c = getchar(); \
        		} \
        	} \
        } while(0) 

#define DLOG(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)
#define DLOGF(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, ##__VA_ARGS__); } while (0)        

#endif /* DEBUG_H */