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

#ifndef NUMBER_H
#define NUMBER_H

#include <stdbool.h>

#include "lib/common.h"

#define NUMBER_DEFAULT_CAPACITY 32

/**
 * Digit array is stored in reverse order
 */
typedef struct {
    u8 *digits;
    u32 length;
    u32 capacity;
    u32 decimal_point;
    bool negative;
} Number;

bool number_init(Number *number, const char *num_str);

bool result_init(Number *result);

void number_print(Number *number);

void number_println(Number *number);

void number_free(Number *number);

#endif /* NUMBER_H */