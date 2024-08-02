# MSR register usermode controller

This application allows user-mode control of ThrottleStop driver to perform RDMSR (Read Model-Specific Register) and WRMSR (Write Model-Specific Register) operations.

## Features

- **RDMSR (Read Model-Specific Register)**: Read values from CPU-specific registers.
- **WRMSR (Write Model-Specific Register)**: Write values to CPU-specific registers.
- Test signing not needed, because ThrottleStop driver is signed.
