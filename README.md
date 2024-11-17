# Crenshaw Compiler in C

This project follows Jack Crenshaw's "Let's Build a Compiler" tutorial series, with implementations in C instead of Pascal.

## Current Progress
- Implemented the basic "cradle" program
- Added expression parsing for basic arithmetic
- Handles addition and subtraction operations
- Handles multiplication and division operations
- Handles operator precedence rules (BEDMAS, PEDMAS, BODMAS, etc)
- Handles parentheses in expression & unary minus

## Building the Project
gcc cradle.c -o compiler
