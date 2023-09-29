#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "../../include/math/number.h"
#include "../../include/lib/logger.h"

static bool load_num(Number *number, const char *num_str)
{
	bool ret = number_init(number, num_str);

	if (!ret) {
		LOG_ERR("Failed to initialize number from string: %s", num_str);
		return false;
	}

	number_println(number);

	number_free(number);

    return true;
}

int main(void)
{
    char *num_str = "123.456";
	char *num_str2 = "123.456.789";
	char *num_str3 = "123.456a";
	char *num_str4 = "-123.456789";

	Number number;

    bool ret;

	ret = load_num(&number, num_str);

    assert(ret);

	ret = load_num(&number, num_str2);

    assert(!ret);

	ret = load_num(&number, num_str3);

    assert(!ret);

	ret = load_num(&number, num_str4);

    assert(ret);

    return 0;
}