#include <Arduino.h>
#include "PDL_Diagnostic_Print.h"

void setup()
{
    diagnostic_print_init();
}

int i = 0;
void loop()
{
    diagnostic_printf("Hello, world! %d\n", i++);
    delay(500);
    // Nothing to do here
}

