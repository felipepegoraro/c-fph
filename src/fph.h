#include <stdlib.h>

#ifndef FPH

#define FPH

typedef enum status {
  ok=EXIT_SUCCESS,
  er=EXIT_FAILURE
} status;

typedef struct input_line {
  char *prompt_line;
  char **parsed_args;
  int program_status;
} input_line;

status fph_main_loop(void);

#endif // !FPH
