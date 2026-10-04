# mypreprocessor
## Project Title
Design and Implementation of a C Preprocessor

## Objective

The objective of this project is to develop a custom preprocessor tool named `mypreprocessor` that processes a C source file and generates an extended source file with a `.i` extension.

The preprocessor performs:

- Comment removal
- File inclusion
- Macro substitution

## Features

### 1. Comment Removal

The program removes:

- Single-line comments using `//`
- Multi-line comments using `/* ... */`

### 2. File Inclusion

The program processes statements in the following format:

```c
#include <filename>
