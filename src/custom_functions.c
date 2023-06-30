#include "./custom_functions.h"
#include "./cons.h"
#include "./hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>


static inline void i32_str(int number, char *s)
{
  sprintf(s, "%d", number);
}


int felipe(char **args)
{
  (void) args;
  printf("teste\n");
  return 0;
}


int fph_echo(char **args)
{
  char program_stt[] = {};
  i32_str(program_status, program_stt);

  void *result = NULL;
  for (size_t i = 1; args[i] != NULL; ++i)
  {
    if (args[i][0] == '$')
    {
      if (args[i][1] == '?') {
        printf("%s ", program_stt);
      } else {
        result = hs_search(map, args[i]+1);
        if (result != NULL) printf("%s ", (char*)result);
      }
    } 
    else if (args[i][0] == '~'){
      result = hs_search(map, "HOME");
      printf("%s ", (char*)result);
    }
    else {
      printf("%s ", args[i]);
    }
  }
  puts("");
  return 0;
}


int fph_exit(char **args)
{
  if (args[1] == NULL) exit(EXIT_SUCCESS);
  exit(isdigit(*args[1]) ? atoi(args[1]) : EXIT_FAILURE);
}
