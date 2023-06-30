#include "./fph.h"
#include <stdlib.h>
#include <stdio.h>
#include <editline/readline.h>
#include <editline/history.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdbool.h>
#include "./custom_functions.h"
#include "./cons.h"
#include "./polish_notation.h"


// author: Felipe Pegoraro
// data..: 28/06/2023


// variável global de cons.h
int program_status = ok;
HashTable *map = NULL;
// char *previous_dir = getenv("$PWD");

int (*list_fn_custom[]) (char **) = {
  &felipe,
  &fph_echo,
  &fph_exit
};

char *list_fn_custom_name[] = {
  "felipe",
  "echo",
  "exit"
};



static void fph_initialize_map_exp(void)
{
  map = hs_create(sizeof(char)*4, sizeof(char)*256, 4);
  hs_insert(map, "HOME", getenv("HOME"));
  hs_insert(map, "USER", getenv("USER"));
  hs_insert(map, "PS1",  SH_PROMPT);
}


// utils ----------------------------------------
void fph_err_handle(const char* err_msg)
{
  fprintf(stderr, "%s: %s\n", SH_NAME, err_msg);
  exit(er);
}
// ---------------------------------------------


bool fph_if_has_polish_notation(const char *input, int *where) {
  int length = strlen(input);
  bool found = false;

  for (int i = 0; i < length; i++) {
    if (input[i] == '(' && input[i + 1] != '(') {
      found = true;
      *where = i;
      break;
    }
  }

  return found;
}


void fph_replace_polish_notation(char *input, int where)
{
  char *substring = strdup(input + where);
  int *result     = pn_evaluate(substring);

  if (result != NULL)
  {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", *result);
    strcpy(input + where, buffer);
  }

  free(substring);
  free(result);
}


void fph_polish_notation_handler(char *input)
{
  int where = 0;
  if (fph_if_has_polish_notation(input, &where)){
    fph_replace_polish_notation(input, where);
  }
}


char **fph_parse_args(char *line)
{
  int current_size = BUFSIZE_ARG;
  int buff_pos = 0;
  char **tokens = (char**) malloc(sizeof(char *)*current_size);
  char *token;

  if (!tokens) fph_err_handle("malloc");
  token = strtok(line, DELIMITER);

  while (token != NULL)
  {
    tokens[buff_pos++] = token;
    if (buff_pos > current_size)
    {
      tokens = realloc(tokens, (current_size+=BUFSIZE_ARG) * sizeof(char*));
      if (!tokens) fph_err_handle("realloc");
    }
    token = strtok(NULL, DELIMITER);
  }
  tokens[buff_pos] = NULL;
  return tokens;
}



int fph_exec_prompt(char **input)
{
  int status;

  pid_t pid = fork();
  if (pid < 0) fph_err_handle("fork");

  if (pid == 0) {
    execvp(input[0], input);
    fph_err_handle(strerror(errno));
  }
  else {
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) return WEXITSTATUS(status);
  }

  return er;
}


int fph_exec_command(char **args)
{
  if (args[0] == NULL) return er;

  size_t i;
  size_t list_fn_size = sizeof(list_fn_custom_name) / sizeof(char *);

  for (i = 0; i < list_fn_size; i++){
    if (strcmp(args[0], list_fn_custom_name[i]) == 0){
      program_status = (*list_fn_custom[i])(args);
      return program_status;
    }
  }

  program_status = fph_exec_prompt(args);
  return program_status;
}


input_line fph_process_input(void)
{
  input_line input;
  input.prompt_line    =  readline(SH_PROMPT);

  add_history(input.prompt_line);
  fph_polish_notation_handler(input.prompt_line);

  input.parsed_args    = fph_parse_args(input.prompt_line);
  input.program_status = fph_exec_command(input.parsed_args);

  return input;
}


status fph_main_loop(void)
{
  fph_initialize_map_exp();
  input_line input = {NULL, NULL, 0};

  do {
    input = fph_process_input();
  } while (1);

  hs_free(map);
  return input.program_status;
}
