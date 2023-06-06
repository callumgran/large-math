/*
 *  Copyright (C) 2023 Callum Gran
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "math/number.h"
#include "lib/logger.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static inline bool is_valid_start_char(const char c)
{
    return isdigit(c) || c == '-';
}

static inline bool is_valid_number(const char *number_str)
{
    bool decimal_point_found = false;

    for (u8 *i = number_str; *i != '\0'; i++) {
	if (*i == '.') {
	    if (decimal_point_found) {
		return false;
	    } else {
		decimal_point_found = true;
	    }
	} else if (!isdigit(*i)) {
	    return false;
	}
    }

    return true;
}

static inline bool validate_digit_str(const char *digit_str)
{
    if (digit_str == NULL) {
	LOG_ERR("digit_str is NULL");
	return false;
    }

    if (digit_str[0] == '\0') {
	LOG_ERR("digit_str is empty");
	return false;
    }

    if (!is_valid_start_char(digit_str[0])) {
	LOG_ERR("digit_str does not start with a valid character");
	return false;
    }

    if (!is_valid_number(digit_str + 1)) {
	LOG_ERR("digit_str contains invalid characters");
	return false;
    }

    return true;
}

static inline u8 char_to_digit(const char c)
{
    return c - '0';
}

static inline char digit_to_char(const u8 digit)
{
    return digit + '0';
}

static void str_to_number(Number *number, const char *str)
{
    number->negative = str[0] == '-';

    u32 j = number->negative ? 1 : 0;

    for (u8 *i = str + j; *i != '\0'; i++) {
	ENSURE_CAP(sizeof(u8), j, number->capacity, number->digits);

	if (*i == '.') {
	    number->decimal_point = j;
	    continue;
	}

	number->digits[j++] = char_to_digit(*i);
    }

    number->length = j;
}

bool number_init(Number *number, const char *number_str)
{
    if (!validate_digit_str(number_str)) {
	return false;
    }

    number->digits = malloc(NUMBER_DEFAULT_CAPACITY * sizeof(u8));
    number->capacity = NUMBER_DEFAULT_CAPACITY;
    number->decimal_point = 0;

    str_to_number(number, number_str);

    return true;
}

void number_print(Number *number)
{
    bool has_decimal_point = number->decimal_point != 0;

    u32 str_len = number->length + has_decimal_point + number->negative;
    
    char *str = malloc(str_len + 1);
    
    u32 str_pos = 0;

    if (number->negative) {
	str[str_pos++] = '-';
    }

    for (u32 i = str_pos; i < number->length && str_pos < str_len; i++) {
	
	if (has_decimal_point && i == number->decimal_point) {
	    str[str_pos++] = '.';
	}
	
	str[str_pos++] = digit_to_char(number->digits[i]);
    }

    str[str_pos] = '\0';

    printf("%s\n", str);

    free(str);
}

void number_free(Number *number)
{
    free(number->digits);
}
