#pragma once

#include "FreeRTOS.h"
#include "semphr.h"

// Function pointer types for initialization and print functions
typedef void (*void_func_t)();
typedef void (*print_func_t)(const char *message);

// API to set the initialization function
bool diagnostic_print_set_init_func(void_func_t func);

bool diagnostic_print_set_deinit_func(void_func_t func);

// API to set the print function
bool diagnostic_print_set_print_func(print_func_t func);

// Function to initialize the diagnostic print module
bool diagnostic_print_init();

// Function to deinitialize the diagnostic print module
void diagnostic_print_deinit();

// Function to send formatted diagnostic messages
bool diagnostic_printf(const char *format, ...);

// Function to send diagnostic messages with a newline character
bool diagnostic_println(const char* message);

