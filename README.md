# Scrabble Game

## Description

This project implements the classic Scrabble board game in C programming language. The game aims to accumulate points by forming words using randomly drawn tiles on a 15x15 grid. Some grid spaces are special, multiplying the letter or word score. The project includes functionality for placing tiles on the board, validating words, and calculating scores.

## Table of Contents

- [Introduction](#introduction)
- [Program Base](#program-base)
- [Game Board](#game-board)
- [Used Tools](#used-tools)
- [Functions](#functions)
- [Main Function](#main-function)
- [References](#references)

## Introduction

### The Scrabble Game
Scrabble is a popular board game where the goal is to score points by forming valid words on a square grid. The letters are randomly distributed, and certain board squares multiply the letter or word score. This project models the Scrabble game using the C programming language.

## Program Base

### Key Features:
- A 15x15 game board.
- Users can place tiles on the board.
- Validates word placement and checks dictionary for validity.
- Supports both horizontal and vertical word placements.
- Assigns tiles randomly to players based on standard distribution.
- Multiplies tile scores according to special squares (double letter, triple letter, etc.).

## Game Board

### Board Representation:
The board is a 15x15 two-dimensional array where columns represent vertical word placement, and rows represent horizontal word placement. Special tiles include:
- **Normal Tile**: No multiplier.
- **Double Letter**: Multiplies letter score by 2.
- **Triple Letter**: Multiplies letter score by 3.
- **Double Word**: Multiplies word score by 2.
- **Triple Word**: Multiplies word score by 3.

## Used Tools

### Preprocessors:
- `#include <stdio.h>`: For basic functions like `printf()`, `scanf()`.
- `#include <stdlib.h>`: For functions like `rand()`, `exit()`.
- `#include <string.h>`: For string manipulation functions like `strlen()`, `strtol()`, `strcspn()`.

### Macros:
- `FLUSH_STDIN`: A macro to discard excessive user input.

### Bitwise Operators:
- Used for efficient processing on integers and characters, including Bitwise AND (`&`), OR (`|`), and XOR (`^`).

### Ternary Conditional Operator:
- Syntax: `variable = condition ? value_if_true : value_if_false;`.

## Functions

### Function Descriptions:
1. **`get_location()`**: 
   - Inputs: User's row and column.
   - Converts the user's input into uppercase and validates it against board limits.

2. **`get_direction()`**:
   - Inputs: User's direction (Horizontal or Vertical).
   - Validates input and sets the direction accordingly.

3. **`get_word()`**:
   - Inputs: User's word, row, column, and direction.
   - Validates the word placement on the board and ensures it fits within the grid limits.

4. **`score_word()`**:
   - Inputs: Word and score pointers.
   - Calculates the score based on tile values and multiplier tiles.

5. **`init_board()`**:
   - Initializes the board with special tiles and empty spaces.

## Main Function

The main function is responsible for initializing the game board, handling user input, validating moves, and calculating scores based on tile placement and word validity.

## References

- Official Scrabble Rules and Guidelines (for board layout and tile distribution).
- C Programming Language documentation for functions used in the project.
