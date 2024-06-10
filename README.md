# PDL Diagnostic Print

This project provides a diagnostic printing module using FreeRTOS for task and queue management, designed for serial communication. It includes functionalities for initialization, deinitialization, and printing of diagnostic messages.

## Features

- **Initialization and Deinitialization**:
  - Default and custom initialization (`init`) and deinitialization (`deinit`) functions.
- **Printing**:
  - Default and custom print functions.
- **Thread Safety**:
  - Uses mutex and queue for thread-safe printing.
- **Task Management**:
  - A FreeRTOS task for handling queued messages.

## Usage

### Initialization

To initialize the diagnostic print module:

```cpp
if (!diagnostic_print_init()) {
    // Handle initialization error
}
```

### Setting Custom Functions

You can set custom initialization, deinitialization, and print functions:

```cpp
diagnostic_print_set_init_func(custom_init_func);
diagnostic_print_set_deinit_func(custom_deinit_func);
diagnostic_print_set_print_func(custom_print_func);
```

### Printing Messages

To print a formatted message:

```cpp
diagnostic_printf("Formatted message: %d", value);
```

To print a message with a newline:

```cpp
diagnostic_println("Message with newline");
```

### Deinitialization

To deinitialize the diagnostic print module:

```cpp
diagnostic_print_deinit();
```

## Unit Testing

Unit tests are provided in the `unit_test.ino` file. These tests demonstrate the usage of the diagnostic print module and verify its functionality.

### Running the Unit Tests

1. Open the `unit_test.ino` file in the Arduino IDE.
2. Upload the code to your Arduino board.
3. Open the Serial Monitor to view the test results.

## File Descriptions

### `PDL_Diagnostic_Print.cpp`
This file contains the implementation of the diagnostic print module, including default functions, mutex and queue management, and the diagnostic print task.

### `PDL_Diagnostic_Print.h`
This file provides the function prototypes and necessary includes for the diagnostic print module.

### `unit_test.ino`
This file contains unit tests for the diagnostic print module, demonstrating its initialization, printing, and deinitialization functionalities.

## Dependencies

- FreeRTOS
- Adafruit TinyUSB Library

## License

This project is licensed under the MIT License.

## Contributing

Contributions are welcome! Please submit pull requests or open issues for any bugs or feature requests.

## Acknowledgements

Special thanks to the contributors and the open-source community.

## Contact

For any questions or inquiries, please contact [your email].
