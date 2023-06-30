## FPH

FPH is a C program that emulates a Unix shell. It includes custom commands
implemented as C functions, a generic hash table implementation for use in
the custom commands, and a Polish notation parser that runs on the shell.

Example:
The following prompt example demonstrates the usage of the `echo` (`fph_echo`) command: 

- `echo $HOME (+ 12 (* 5 (/ 18 3)))`. 

It outputs `/home/felipe 42`.

### Todo List

- Hash Table
    - Collision resolution: The hash table currently has a weak mechanism for resolving collisions.
    - (Maybe) Delete function: Although not yet implemented, it will not be used.
- Polish Notation
    - Duplicates: Having two or more Polish notations in a prompt may lead to bugs.
    - Order: Please refer to the `polish_notation.c` file for details.
- (Maybe) Shell Operators
    - Logical operators: `&&`, `||`
    - Additional operators: `=`, `{n..m}`
- Commands: 
    - `fph_cd`: and update `extern char *previous_dir`

### About

- Author: Felipe Pegoraro
- Date: 28/06/2023
