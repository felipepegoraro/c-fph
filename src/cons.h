#include "./hash_table.h"

#ifndef CONS
#define CONS

#define BUFSIZE 2048
#define BUFSIZE_ARG BUFSIZE/32
#define DELIMITER " \a\n\r\t"
#define SH_NAME "fph"
#define SH_PROMPT SH_NAME"> "

extern int program_status;
extern HashTable *map;
// extern char *previous_dir;

#endif // !CONS
