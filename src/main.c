#include <stdio.h>

#include "math/number.h"
#include "lib/logger.h"
#include "math/addition.h"

static void load_num(Number *number, const char *num_str)
{
        bool ret = number_init(&number, num_str);

        if (!ret) {
                LOG_ERR("Failed to initialize number from string: %s", num_str);
                return;
        }

        number_print(&number);

        number_free(&number);
}

int main()
{
        char *num_str = "123.456";
        char *num_str2 = "123.456.789";
        char *num_str3 = "123.456a";
        char *num_str4 = "-123.456789";

        Number number;

        load_num(&number, num_str);

        load_num(&number, num_str2);

        load_num(&number, num_str3);

        load_num(&number, num_str4);

        char *num_str5 = "1234";
        char *num_str6 = "5678";

        Number a;
        
        bool ret = number_init(&a, num_str5);

        if (!ret) {
                LOG_ERR("Failed to initialize number from string: %s", num_str5);
                return 1;
        }

        Number b;

        ret = number_init(&b, num_str);

        if (!ret) {
                LOG_ERR("Failed to initialize number from string: %s", num_str6);
                return 1;
        }

        Number result;
        result.digits = malloc(NUMBER_DEFAULT_CAPACITY * sizeof(u8));

        ret = number_addition(&a, &b, &result);

        if (!ret) {
                LOG_ERR("Failed to add numbers");
                return 1;
        }

        number_print(&result);

        number_free(&a);
        
        number_free(&b);

        number_free(&result);
        
        return 0;
}