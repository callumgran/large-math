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

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "lib/logger.h"
#include "math/addition.h"
#include "math/subtraction.h"

static inline void reallocate_result_cap(const u32 cap_a, const u32 cap_b, Number *result)
{
    u32 max_cap = MAX(cap_a, cap_b);

    // If the result is longer than the current capacity, reallocate
    if (result->capacity < max_cap) {
	result->capacity = max_cap;
	result->digits = realloc(result->digits, result->capacity * sizeof(u8));
    }

    memset(result->digits, 0, result->capacity * sizeof(u8));
}

static inline u8 sum_of_three(const Number *a, const Number *b, const Number *c, const u32 a_idx,
			      const u32 b_idx, const u32 c_idx)
{
    return a->digits[a_idx] + b->digits[b_idx] + c->digits[c_idx];
}

static inline bool main_addition_loop(const Number *a, const Number *b, Number *result, u32 *a_idx,
				      u32 *b_idx, u32 *res_idx)
{
    bool carry = false;

    u32 local_res_idx = *res_idx;
    u32 local_a_idx = *a_idx;
    u32 local_b_idx = *b_idx;
    while (local_a_idx < a->length) {
	u8 sum = sum_of_three(a, b, result, local_a_idx++, local_b_idx++, local_res_idx);
	if (sum > 9) {
	    result->digits[local_res_idx++] = sum - 10;
	    result->digits[local_res_idx] += 1;
	    carry = true;
	} else {
	    result->digits[local_res_idx++] = sum;
	    carry = false;
	}
    }

    *res_idx = local_res_idx;
    *a_idx = local_a_idx;
    *b_idx = local_b_idx;
    return carry;
}

static inline bool carry_addition_loop(const Number *a, Number *result, u32 *a_idx, u32 *res_idx,
				       bool carry)
{
    bool local_carry = carry;

    u32 local_res_idx = *res_idx;
    u32 local_a_idx = *a_idx;
    while (local_a_idx < a->length) {
	u8 sum = result->digits[local_res_idx] + a->digits[local_a_idx++];
	if (sum > 9) {
	    result->digits[local_res_idx++] += sum - 10;
	    result->digits[local_res_idx] += 1;
	    local_carry = true;
	} else {
	    result->digits[local_res_idx++] += sum;
	    local_carry = false;
	}
    }

    *res_idx = local_res_idx;
    return local_carry;
}

static bool number_addition_no_decimal(const Number *a, const Number *b, Number *result)
{
    result->decimal_point = 0;

    reallocate_result_cap(a->capacity, b->capacity, result);

    bool a_is_longer = a->length > b->length;

    u32 res_idx = 0;
    u32 a_idx = 0;
    u32 b_idx = 0;
    bool carry = false;

    if (a_is_longer) {
	carry = main_addition_loop(b, a, result, &b_idx, &a_idx, &res_idx);

	carry = carry_addition_loop(a, result, &a_idx, &res_idx, carry);
    } else {
	carry = main_addition_loop(a, b, result, &a_idx, &b_idx, &res_idx);

	carry = carry_addition_loop(b, result, &b_idx, &res_idx, carry);
    }

    if (carry) {
	ENSURE_CAP(sizeof(u8), res_idx, result->capacity, result->digits);
	result->digits[res_idx++] = 1;
    }

    result->length = res_idx;

    return true;
}

/* The first number is the one with the decimal point */
static bool number_addition_one_decimal(const Number *a, const Number *b, Number *result)
{
    result->decimal_point = a->decimal_point;

    reallocate_result_cap(a->capacity, b->capacity, result);

    u32 a_len = a->length - a->decimal_point;

    bool a_is_longer = a_len > b->length;

    u32 res_idx = 0;
    u32 a_idx = 0;
    u32 b_idx = 0;
    bool carry = false;

    while (a_idx < a->decimal_point) {
	result->digits[res_idx++] = a->digits[a_idx++];
    }

    if (a_is_longer) {
	carry = main_addition_loop(b, a, result, &b_idx, &a_idx, &res_idx);

	carry = carry_addition_loop(a, result, &a_idx, &res_idx, carry);
    } else {
	result->decimal_point += b->length - a_len;
	carry = main_addition_loop(a, b, result, &a_idx, &b_idx, &res_idx);

	carry = carry_addition_loop(b, result, &b_idx, &res_idx, carry);
    }

    if (carry) {
	ENSURE_CAP(sizeof(u8), res_idx, result->capacity, result->digits);
	result->digits[res_idx++] = 1;
	result->decimal_point++;
    }

    result->length = res_idx;

    return true;
}

static bool number_addition_two_decimals(const Number *a, const Number *b, Number *result)
{
    result->decimal_point = a->decimal_point;

    reallocate_result_cap(a->capacity, b->capacity, result);

    u32 a_dec_len = a->length - a->decimal_point;
    u32 b_dec_len = b->length - b->decimal_point;

    u32 pre_len = a_dec_len - b_dec_len;

    bool a_is_longer = a->length - a_dec_len > b->length - b_dec_len;

    u32 res_idx = 0;
    u32 a_idx = 0;
    u32 b_idx = 0;
    bool carry = false;

    while (a_idx < pre_len) {
	result->digits[res_idx++] = a->digits[a_idx++];
    }

    if (a_is_longer) {
	carry = main_addition_loop(b, a, result, &b_idx, &a_idx, &res_idx);

	carry = carry_addition_loop(a, result, &a_idx, &res_idx, carry);
    } else {
	result->decimal_point = b->length - b_dec_len;
	carry = main_addition_loop(a, b, result, &a_idx, &b_idx, &res_idx);

	carry = carry_addition_loop(b, result, &b_idx, &res_idx, carry);
    }

    if (carry) {
	ENSURE_CAP(sizeof(u8), res_idx, result->capacity, result->digits);
	result->digits[res_idx++] = 1;
	result->decimal_point++;
    }

    result->length = res_idx;

    return true;
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

    result->negative = false;

    if (a->negative && b->negative) {
	result->negative = true;
    } else if (a->negative || b->negative) {
	if (a->negative) {
	    return number_subtraction(b, a, result);
	} else {
	    return number_subtraction(a, b, result);
	}
    }

    bool a_no_decimal = a->decimal_point == 0;
    bool b_no_decimal = b->decimal_point == 0;

    if (a_no_decimal && b_no_decimal) {
	return number_addition_no_decimal(a, b, result);
    } else if (!a_no_decimal && !b_no_decimal) {
	if ((a->length - a->decimal_point) > (b->length - b->decimal_point)) {
	    return number_addition_two_decimals(a, b, result);
	} else {
	    return number_addition_two_decimals(b, a, result);
	}
    } else if (b_no_decimal) {
	return number_addition_one_decimal(a, b, result);
    } else {
	return number_addition_one_decimal(b, a, result);
    }

    return false;
}