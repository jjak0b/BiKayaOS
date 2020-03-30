#ifndef H_TYPES
#define H_TYPES

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef void (*function_t) (void *arg);

typedef unsigned char byte;

typedef unsigned int word;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#endif