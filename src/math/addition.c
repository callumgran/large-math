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

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lib/logger.h"
#include "math/addition.h"
#include "math/subtraction.h"

static void reverse_digits(u8 *digits, u32 length)
{
        u32 i = 0;
        u32 j = length - 1;

        while (i < j) {
                u8 temp = digits[i];
                digits[i] = digits[j];
                digits[j] = temp;

                i++;
                j--;
        }
}

static bool number_addition_no_decimal(const Number *a, const Number *b, Number *result)
{
        result->decimal_point = 0;
        
        u32 max_cap = MAX(a->capacity, b->capacity);

        // If the result is longer than the current capacity, reallocate
        if (result->capacity < max_cap) {
                result->capacity = max_cap;
                result->digits = realloc(result->digits, result->capacity * sizeof(u8));
                if (result->digits == NULL) {
                        LOG_ERR("Failed to reallocate memory for result");
                        return false;
                }
        }

        memset(result->digits, 0, result->capacity * sizeof(u8));

        
        u32 a_index = a->decimal_point;
        u32 b_index = b->length;

        bool a_is_longer = a_index > b_index;
        
        u32 res_idx = 0;
        
        // Add the digits before the decimal point
        while (a_index > 0 && b_index > 0) {
                u8 a_digit = a->digits[--a_index];
                u8 b_digit = b->digits[--b_index];
                u8 sum = a_digit + b_digit;
                if (sum > 9) {
                        result->digits[res_idx++] += sum - 10;
                        result->digits[res_idx] += 1;
                } else {
                        result->digits[res_idx++] += sum;
                }
        }

        // Add the remaining digits from the longer number
        if (a_is_longer) {
                while (a_index > 0) {
                        result->digits[res_idx++] += a->digits[a_index--];
                }
        } else {
                while (b_index > 0) {
                        result->digits[res_idx++] += b->digits[b_index--];
                }
        }

        result->length = res_idx;
        
        reverse_digits(result->digits, result->length);

        return true;
}

/* The first number is the one with the decimal point */
static bool number_addition_one_decimal(const Number *a, const Number *b, Number *result)
{        
        u32 max_cap = MAX(a->capacity, b->capacity);

        // If the result is longer than the current capacity, reallocate
        if (result->capacity < max_cap) {
                result->capacity = max_cap;
                result->digits = realloc(result->digits, result->capacity * sizeof(u8));
                if (result->digits == NULL) {
                        LOG_ERR("Failed to reallocate memory for result");
                        return false;
                }
        }

        memset(result->digits, 0, result->capacity * sizeof(u8));

        u32 a_index = a->decimal_point;
        u32 b_index = b->length;

        bool a_is_longer = a_index > b_index;
        
        u32 res_idx = 0;

        // Feed in the digits after the decimal point in reverse order
        for (u32 i = a->length - 1; i >= a->decimal_point; --i) {
                result->digits[res_idx++] = a->digits[i];
        }
        
        // Add the digits before the decimal point
        while (a_index > 0 && b_index > 0) {
                u8 a_digit = a->digits[--a_index];
                u8 b_digit = b->digits[--b_index];
                u8 sum = a_digit + b_digit;
                if (sum > 9) {
                        result->digits[res_idx++] += sum - 10;
                        result->digits[res_idx] += 1;
                } else {
                        result->digits[res_idx++] += sum;
                }
        }

        // Add the remaining digits from the longer number
        if (a_is_longer) {
                while (a_index > 0) {
                        result->digits[res_idx++] += a->digits[a_index--];
                }
        } else {
                while (b_index > 0) {
                        result->digits[res_idx++] += b->digits[b_index--];
                }
        }

        result->length = res_idx;
        result->decimal_point = res_idx - a->decimal_point;

        reverse_digits(result->digits, result->length);

        return true;
}

static bool number_addition_two_decimals(const Number *a, const Number *b, Number *result)
{
        return false;
}

bool number_addition(const Number *a, const Number *b, Number *result)
{
        if (a == NULL) {
                LOG_ERR("First input is NULL"); 
                return false;
        }

        if (b == NULL) {
                LOG_ERR("Second input is NULL");
                return false;
        }

        if (result == NULL) {
                LOG_ERR("Result is NULL at input");
                return false;
        }

        if (a->negative && b->negative) {
                result->negative = true;
        } else if (a->negative || b->negative) {
                if (a->negative) {
                        return number_subtraction(b, a, result);
                } else {
                        return number_subtraction(a, b, result);
                }
        }

        result->negative = false;

        bool a_no_decimal = a->decimal_point == 0;
        bool b_no_decimal = b->decimal_point == 0;

        if (a_no_decimal && b_no_decimal) {
                return number_addition_no_decimal(a, b, result);
        } else if (!a_no_decimal && !b_no_decimal) {
                return number_addition_two_decimals(a, b, result);
        } else if (b_no_decimal) {
                return number_addition_one_decimal(a, b, result);
        } else {
                return number_addition_one_decimal(b, a, result);
        }
}