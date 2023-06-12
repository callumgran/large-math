#include <stdio.h>

#include "lib/logger.h"
#include "math/addition.h"
#include "math/number.h"

static void load_num(Number *number, const char *num_str)
{
    bool ret = number_init(number, num_str);

    if (!ret) {
	LOG_ERR("Failed to initialize number from string: %s", num_str);
	return;
    }

    number_println(number);

    number_free(number);
}

static void perform_addition(Number *a, Number *b)
{
    Number result;

    result_init(&result);

    bool ret = number_addition(a, b, &result);

    if (!ret) {
	LOG_ERR("Failed to add numbers");
	return;
    }

    number_println(a);
    printf(" + ");
    number_println(b);
    printf(" = ");
    number_println(&result);

    number_free(&result);
}

int main()
{
    // char *num_str = "123.456";
    // char *num_str2 = "123.456.789";
    // char *num_str3 = "123.456a";
    // char *num_str4 = "-123.456789";

    // Number number;

    // load_num(&number, num_str);

    // load_num(&number, num_str2);

    // load_num(&number, num_str3);

    // load_num(&number, num_str4);

    char *num_str5 = "123456789";
    char *num_str6 = "987654321";

    Number a;

    bool ret = number_init(&a, num_str6);

    if (!ret) {
	LOG_ERR("Failed to initialize number from string: %s", num_str5);
	return 1;
    }

    Number b;

    ret = number_init(&b, num_str5);

    if (!ret) {
	LOG_ERR("Failed to initialize number from string: %s", num_str6);
	return 1;
    }

    perform_addition(&a, &b);

    char *num_str7 = "1234567832913193129.3921896731123456789";
    char *num_str8 = "987654321";

    ret = number_init(&a, num_str7);

    if (!ret) {
	LOG_ERR("Failed to initialize number from string: %s", num_str7);
	return 1;
    }

    ret = number_init(&b, num_str8);

    if (!ret) {
	LOG_ERR("Failed to initialize number from string: %s", num_str8);
	return 1;
    }

    perform_addition(&a, &b);

    number_free(&a);

    number_free(&b);

    return 0;
}