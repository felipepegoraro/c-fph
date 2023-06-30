#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "./polish_notation.h"


// author: Felipe Pegoraro
// data..: 30/06/2023

// nao está funcionando 100%. Expressões do tipo a seguir
// são calculas incorretamente, basta inverter a posição.
// (- 10 (* (/ 12 3) 2)) deveria ser 2, mas é 6
// (- 10 (* 2 (/ 12 3))) temos o valor correto 2


static AST *ast_create(void)
{
  AST *new_ast = (AST*) malloc(sizeof(AST));
  if (new_ast == NULL) return NULL;
  
  new_ast->token = NULL;
  new_ast->left = NULL;
  new_ast->right = NULL;
  new_ast->type = NULL;
  
  return new_ast;
}


static void ast_free(AST *ast)
{
  if (ast == NULL) return;
  ast_free(ast->left);
  ast_free(ast->right);
  free(ast->type);
  free(ast);
}


static bool is_operator(const char token) {
  return (
    token == '+' ||
    token == '-' ||
    token == '*' ||
    token == '/'
  );
}


static bool is_parenthesis(const char token) {
  return (
    token == '(' ||
    token == ')'
  );
}


static char *process_delimiters(const char *input)
{
  int inputLength = strlen(input);
  char *result = malloc((2 * inputLength + 1) * sizeof(char));
  int j = 0;

  for (int i = 0; i < inputLength; i++)
  {
    if (input[i] == '(' || input[i] == ')')
    {
      result[j++] = ' ';
      result[j++] = input[i];
      result[j++] = ' ';
    }
    else {
      result[j++] = input[i];
    }
  }

  result[j] = '\0';
  return result;
}


static char **process_input(const char *input, int* size)
{
  const char *delimiters = " ";
  char *copy = strdup(process_delimiters(input));
  char *token = strtok(copy, delimiters);
  char **substrings = NULL;
  int count = 0;

  while (token != NULL)
  {
    substrings = realloc(substrings, (count + 1) * sizeof(char*));
    substrings[count] = strdup(token);
    count++;
    token = strtok(NULL, delimiters);
  }

  free(copy);
  *size = count;
  return substrings;
}


static AST *parse_expression(char **tokens, int *index)
{
  AST *to_return = ast_create();
  char *token = strdup(tokens[*index]);

  if (isdigit(token[0]))
  {
    (*index)++;

    to_return->left = NULL;
    to_return->right = NULL;
    to_return->token = strdup(token);
    to_return->type = strdup("number");
    return to_return;
  }

  if (is_operator(token[0]))
  {
    (*index)++;

    AST *left  = parse_expression(tokens, index);
    AST *right = parse_expression(tokens, index);

    to_return->left  = left;
    to_return->right = right;
    to_return->token = strdup(token);
    to_return->type = strdup("operator");
    return to_return;
  }

  if (is_parenthesis(token[0]) && token[0] == '(')
  {
    (*index)++;

    AST *left = parse_expression(tokens, index);
    if (left == NULL) return NULL;

    to_return->left = left;
    to_return->right = NULL;
    to_return->token = strdup(token);
    to_return->type = strdup("parenthesis");
    return to_return;
  }

  return NULL;
}


static AST *parse_tokens(char **tokens)
{
  int index=0;
  return parse_expression(tokens, &index);
}


static int *eval_ast(AST *ast)
{
  if (ast == NULL) return NULL;

  int *x = (int*) malloc(sizeof(int));

  // NUMERO ===================================
  if (strcmp(ast->type, "number")==0)
  {
    *x = atoi(ast->token);
    return x;
  }

  // OPERADOR =================================
  if (strcmp(ast->type, "operator") == 0)
  {
    int *left = eval_ast(ast->left);
    int *right = eval_ast(ast->right);

    if (strcmp(ast->token, "+") == 0){
      *x = (left == NULL ? 0 : *left) + (right == NULL ? 0 : *right);
      return x;
    }
    else if (strcmp(ast->token, "-") == 0){
      if (left == NULL && right == NULL) { return NULL; }
      if (left != NULL && right == NULL) { *x = -(*left); return x; }
      if (left == NULL && right != NULL) { *x = -(*right); return x; }
      if (left != NULL && right != NULL) { *x = *left - *right; return x; }
    }
    else if (strcmp(ast->token, "*") == 0){
      *x = (left == NULL ? 1 : *left) * (right == NULL ? 1 : *right);
      return x;
    } 
    else if (strcmp(ast->token, "/") == 0){
      *x = (left == NULL ? 1 : *left) / (right == NULL ? 1 : *right);
      return x;
    } 
    else {
      fprintf(stderr, "unkown operator\n");
      return NULL;
    }
  }

  // PARENTESES ===============================
  if (strcmp(ast->type, "parenthesis") == 0)
  {
    x = eval_ast(ast->left);
    return x;
  }

  return NULL;
}


int *pn_evaluate(const char *input)
{
  size_t ips = strlen(input);
  if (
    ips <= 2 || input[0] != '(' || input[ips-1] != ')' || (
    ips == 3 && !(isdigit(input[1]) || isdigit(input[2])))
  ){
    return NULL;
  }

  int size;

  char **tokens = process_input(input, &size);
  AST  *tree    = parse_tokens(tokens);
  int  *res     = eval_ast(tree);
  
  for (int i = 0; i < size; i++)
    free(tokens[i]);
  free(tokens);
  ast_free(tree);
  
  return res;
}

/*
int main(void)
{
  const char *input[] = {
    "(+ 2)",                 // 2    (valor correto)
    "(- 2)",                 // -2   (valor correto)
    "(2)",                   // 2    (valor correto)
    "(* 2 2)",               // 4    (valor correto)
    "(/ 4 2)",               // 2    (valor correto)
    "* 2 4",                 // erro (valor correto)
    "(* 2 3",                // erro (valor correto)
    "()",                    // erro (valor correto)
    "(",                     // erro (valor correto)
    ")",                     // erro (valor correto)
    "(*)",                   // erro (valor correto)
    "(+)",                   // erro (valor correto)
    "(/ 2)",                 // erro (valor correto)
    "(* 2)",                 // erro (valor correto)
    "(+ 2 (* 5 (* 2)))",     // 12   (valor correto)
    "(+ 10 (/ 20 5))",       // 14   (valor correto)
    "(* (- 8 4) 2)",         // 8    (VALOR INCORRETO)
    "(+ 1 (* 2 (/ 8 4)))",   // 5    (valor correto)
    "(- 10 (* (/ 12 3) 2))", // 6    (VALOR INCORRETO)
    "(- 10 (* 2 (/ 12 3)))", // 2    (valor correto)
  };

  for (int i=0; i<20; i++)
  {
    int *x = pn_evaluate(input[i]);
    if (x) printf("%s: %d\n", input[i], *x);
    else printf("%s: erro\n", input[i]);
    free(x);
  }

  return(EXIT_SUCCESS);
}
*/
