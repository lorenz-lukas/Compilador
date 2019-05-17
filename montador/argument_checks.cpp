/** @brief Inclusão da biblioteca de verificação de argumentos */

#include "argument_checks.h"

/**  Instrução de uso correto do programa e escolha do tipo de output */
static void showUsage() {
	printf("Usage:    .\\tradutor assembly_file.asm\n");
}
void validateArguments(int argc, char* argv[]) {
	checkArgumentAmount(argc);
	checkFileExtension(argv[1]);
}

void checkArgumentAmount(int argc) {
	if (argc != 2 ) {
		fprintf(stderr, "[ERROR]: Invalid amount of arguments.\n");
		showUsage();
		exit(EXIT_FAILURE);
	}
}

/** Checagem da extensão do arquivo.
	@param[in] file_name Ponteiro do tipo char que aponta para o começo do arquivo.

*/
void checkFileExtension(char* file_name) {
	if (strstr(file_name, ".asm") == NULL) {
		fprintf(stderr, "[ERROr]: The file must have the extension '.asm'\n");
		showUsage();
		exit(EXIT_FAILURE);
	}
}

