#include "argument.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "tools.h"

int parse_args(char argc, char ** argv, struct arguments * arguments) {
	uint8_t i;
	int real_set = 0, imag_set = 0;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--sr") == 0) { // String to real
			arguments->cx = hash_string(argv[i + 1], -2, 2);
			real_set = 1;
			i++;
		} else if (strcmp(argv[i], "--si") == 0) { // String to imaginary
			arguments->cy = hash_string(argv[i + 1], -0.038, 0.038);
			imag_set = 1;
			i++;
		} else if (strcmp(argv[i], "--real") == 0) { // Set real number
			arguments->cx = to_double(argv[i + 1]);
			real_set = 1;
			i++;
		} else if (strcmp(argv[i], "--imag") == 0) { // Set imaginary number
			arguments->cy = to_double(argv[i + 1]);
			imag_set = 1;
			i++;
		} else {
			illegal_argument(argv[i], argv[0]);
			return -1;
		}
	}

	if (!real_set || !imag_set) {
		printf("Wrong number of arguments\n");
		return -2;
	}

	return 0;
}

void illegal_argument(const char * argument, const char * command) {
	printf("Error: Illegal argument %s\n\nUSAGE: %s <cr> <ci>\n       %s -si <str>\n       %s -sr <str>\n", argument, command, command, command);
}
