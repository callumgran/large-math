#include <stdio.h>

#include "lib/logger.h"
#include "math/addition.h"
#include "math/number.h"
#include "math/subtraction.h"

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

    // printf("   ");
    // number_println(a);
    // printf(" + ");
    // number_println(b);
    // printf(" = ");
    // number_println(&result);

    number_free(&result);
}

static void perform_subtraction(Number *a, Number *b)
{
    Number result;

    result_init(&result);

    bool ret = number_subtraction(a, b, &result);

    if (!ret) {
	LOG_ERR("Failed to add numbers");
	return;
    }

    printf("   ");
    number_println(a);
    printf(" - ");
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

    char *num_str5 = "112345678";
    char *num_str6 = "-11234567";

    Number a;

    // bool ret = number_init(&a, num_str6);

    // if (!ret) {
    // LOG_ERR("Failed to initialize number from string: %s", num_str5);
    // return 1;
    // }

    Number b;

    // ret = number_init(&b, num_str5);

    // if (!ret) {
    // LOG_ERR("Failed to initialize number from string: %s", num_str6);
    // return 1;
    // }

    // perform_addition(&a, &b);

    // number_free(&a);

    // number_free(&b);

    // char *num_str7 =
    // "321898321873216312632184789433890124780132123.45678954127893017864371289391287312838921732176312";
    // char *num_str8 =
    // "37281731289739128546218794891246128389012890312789132890132123456.789345216890381290372183812093821908391073216421856321678";

    // bool ret = number_init(&a, num_str7);

    // if (!ret) {
    // LOG_ERR("Failed to initialize number from string: %s", num_str7);
    // return 1;
    // }

    // ret = number_init(&b, num_str8);

    // if (!ret) {
    // LOG_ERR("Failed to initialize number from string: %s", num_str8);
    // return 1;
    // }

    // for (int i = 0; i < 10000; i++)
    // perform_addition(&a, &b);

    // number_free(&a);

    // number_free(&b);

    bool ret = number_init(&a, num_str5);

    if (!ret) {
	LOG_ERR("Failed to initialize number from string: %s", num_str6);
	return 1;
    }

    ret = number_init(&b, num_str6);

    if (!ret) {
	LOG_ERR("Failed to initialize number from string: %s", num_str5);
	return 1;
    }

    perform_subtraction(&a, &b);

    number_free(&a);

    number_free(&b);

    return 0;
}