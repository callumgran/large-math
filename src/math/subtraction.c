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

static inline i8 sum_of_three(const Number *a, const Number *b, const Number *c, const u32 a_idx,
							  const u32 b_idx, const u32 c_idx)
{
	return (c->digits[c_idx] + a->digits[a_idx]) - b->digits[b_idx];
}

static inline bool main_subtraction_loop(const Number *a, const Number *b, Number *result,
										 u32 *a_idx, u32 *b_idx, u32 *res_idx, u32 max_len)
{
	u32 local_res_idx = *res_idx;
	u32 local_a_idx = *a_idx;
	u32 local_b_idx = *b_idx;
	bool carry = false;

	while (local_a_idx < max_len) {
		i8 sum = sum_of_three(a, b, result, local_a_idx++, local_b_idx++, local_res_idx);

		if (carry) {
			sum--;
		}

		if (sum < 0) {
			sum += 10;
			result->digits[local_res_idx++] = sum;
			carry = true;
		} else {
			result->digits[local_res_idx++] = sum;
			carry = false;
		}
	}

	*a_idx = local_a_idx;
	*b_idx = local_b_idx;
	*res_idx = local_res_idx;

	return carry;
}

static bool carry_subtraction_loop(const Number *a, Number *result, u32 *a_idx, u32 *res_idx,
								   bool carry)
{
	u32 local_res_idx = *res_idx;
	u32 local_a_idx = *a_idx;
	bool local_carry = carry;

	while (local_a_idx < a->length) {
		if (local_carry) {
			result->digits[local_res_idx++] = a->digits[local_a_idx++] - 1;
			local_carry = false;
		} else {
			result->digits[local_res_idx++] = a->digits[local_a_idx++];
		}
	}

	*a_idx = local_a_idx;
	*res_idx = local_res_idx;

	return local_carry;
}

static bool number_subtraction_no_decimal(const Number *a, const Number *b, Number *result)
{
	reallocate_result_cap(a->capacity, b->capacity, result);

	u32 a_idx = 0;
	u32 b_idx = 0;
	u32 res_idx = 0;
	bool carry = false;

	if (a->length < b->length) {
		result->negative = true;
		carry = main_subtraction_loop(b, a, result, &a_idx, &b_idx, &res_idx, a->length);
		carry = carry_subtraction_loop(b, result, &b_idx, &res_idx, carry);
	} else if (a->length > b->length) {
		carry = main_subtraction_loop(a, b, result, &a_idx, &b_idx, &res_idx, b->length);
		carry = carry_subtraction_loop(a, result, &a_idx, &res_idx, carry);
	} else {
		u32 tmp = a->length - 1;

		while (b->digits[tmp] == a->digits[tmp]) {
			tmp--;
		}

		if (b->digits[tmp] > a->digits[tmp]) {
			result->negative = true;
			carry = main_subtraction_loop(b, a, result, &a_idx, &b_idx, &res_idx, ++tmp);
		} else {
			carry = main_subtraction_loop(a, b, result, &a_idx, &b_idx, &res_idx, ++tmp);
		}
	}

	if (carry) {
		ENSURE_CAP(sizeof(u8), res_idx, result->capacity, result->digits);
		result->digits[res_idx++] -= 1;
	}

	if ((i8)result->digits[res_idx - 1] < 0) {
		result->negative = true;
		result->digits[res_idx - 1] += 10;
	}

	while (result->digits[res_idx - 1] == 0) {
		res_idx--;
	}

	result->length = res_idx;

	return true;
}

bool number_subtraction(const Number *a, const Number *b, Number *result)
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

	if (b->negative) {
		Number tmp;
		number_copy(&tmp, b);
		tmp.negative = false;
		bool ret = number_addition(a, &tmp, result);
		number_free(&tmp);
		return ret;
	}

	result->negative = false;

	bool a_no_decimal = a->decimal_point == 0;
	bool b_no_decimal = b->decimal_point == 0;

	if (a_no_decimal && b_no_decimal) {
		return number_subtraction_no_decimal(a, b, result);
	}

	return false;
}