#ifndef POLISH_NOTATION
#define POLISH_NOTATION

typedef struct ast {
  struct ast *left;
  struct ast *right;
  char *type;
  char *token;
} AST;

int *pn_evaluate(const char *input);

#endif // !POLISH_NOTATION


/*
 * primeiro fiz em typecript, a idea era usar esse ADT:
 *
 * type OperatorType =  "+" | "-" | "*" | "/";
 * type ParenthesisType = "(" | ")";
 * type TokenType = OperatorType | ParenthesisType | number;

 * type AST = {
 *   token: TokenType;
 *   left:  AST | null;
 *   right: AST | null;
 *   diff:  "operator" | "parenthesis" | "number"
 * }
 *
 * funcionou. mas passando para C deu muito trabalho para
 * usar enums, unions e structs. por fim, deixei apenas
 * como char* mesmo.
 *
*/
