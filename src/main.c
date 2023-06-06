#include <stdio.h>

#include "math/number.h"
#include "lib/logger.h"

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
        
        return 0;
}