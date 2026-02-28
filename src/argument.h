#pragma once

struct arguments {
	double cx, cy;
};

int parse_args(char argc, char ** argv, struct arguments * arguments);
void illegal_argument(const char * argument, const char * command);
