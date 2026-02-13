#include "tools.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOWER_TO_UPPER_CASE ('a' - 'A')
#define NUMBER_OF_LETTERS ('Z' - 'A' + 1)
#define LETTER_SCALE (1.0 / (double) NUMBER_OF_LETTERS)

double to_double(const char *str) {
  uint32_t i;
  double div = 1.0;

  uint32_t out_int = 0;
  uint32_t out_dec = 0;

  char *decimal_place = strchr(str, '.');

  out_int = atoi(str);

  if (decimal_place) {
    *decimal_place = 0;
    out_dec = atoi(decimal_place + 1);

    for (i = 0; i < strlen(decimal_place + 1); i++) {
      div /= 10.0;
    }
  }

  return out_int + out_dec * div;
}

double hash_string(const char *str, double min, double max) {
  double hash = 0;
  char chr;
  size_t str_len = strlen(str);
  size_t i;
  double current_scale = LETTER_SCALE;

  for (i = 0; i < str_len; i++) {
    chr = str[i];

    if (chr >= 'a' && chr <= 'z') {
      chr -= LOWER_TO_UPPER_CASE;
    } else if (!(chr >= 'A' && chr <= 'Z')) {
      continue;
    }

    hash += (chr - 'A') * current_scale;
    current_scale *= LETTER_SCALE;
  }

  return hash * (max - min) + min;
}
