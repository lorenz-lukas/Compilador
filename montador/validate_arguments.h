#ifndef VALIDATE_ARGUMENTS_H_
#define VALIDATE_ARGUMENTS_H_

/** @brief Inclusão de bibliotecas e Declaração de funções de verificação.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void validate_arguments(int argc, char* argv[]);
void checkArgumentAmount(int argc);
void checkFileExtension(char* file_name);
int checkOutputOption(char* op);

#endif /* VALIDATE_ARGUMENTS_H_ */
