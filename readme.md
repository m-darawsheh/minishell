# 42 Minishell

## Overview
**Minishell** is a simple shell implementation built as part of the 42 curriculum. It is designed to replicate the basic functionality of a Unix shell, providing the ability to execute commands, manage processes, and handle basic shell operations. This project covers various aspects of system programming and focuses on understanding the internal mechanics of a shell.

## Features
- Parsing of command-line input.
- Support for basic built-in commands (`cd`, `echo`, `env`, `exit`).
- Execution of external programs.
- Redirection (both input and output) and piping.
- Signal handling (such as SIGINT and SIGQUIT).
- History functionality (optional).

## Installation

To set up the project on your local machine, follow these steps:

1. Clone the repository:
    ```bash
    git clone https://github.com/your-username/minishell.git
    cd minishell
    ```

2. Compile the project using `Makefile`:
    ```bash
    make
    ```

3. Run the minishell:
    ```bash
    ./minishell
    ```

## Usage

Once the shell is running, you can use it just like any other terminal:

- Run simple commands:
    ```bash
    echo "Hello, World!"
    ```

- Execute external commands:
    ```bash
    ls -l
    ```

- Use pipes and redirection:
    ```bash
    echo "Hello, World!" | grep "World"
    ```

- Basic built-in commands:
    - `cd <path>`: Change directory.
    - `exit`: Exit the shell.
    - `env`: Display the environment variables.
    - `echo <text>`: Output the text to the console.

## Project Structure

- `src/`: Contains the main source code files.
    - `main.c`: Entry point of the program.
    - `parser.c`: Command parsing functions.
    - `execute.c`: Functions to execute commands.
    - `builtins.c`: Built-in command implementations.
    - `utils.c`: Helper functions.
- `include/`: Header files for the project.
- `Makefile`: Makefile for compiling the project.

## Testing

You can test your minishell by running various shell commands and checking if the output is as expected. Make sure to test:

- Built-in commands.
- External commands with different arguments.
- Handling of pipes and redirection.
- Signal handling (such as pressing `Ctrl+C`).

## Notes

- The project is designed to handle basic shell operations but doesn't cover all the advanced features of a fully-fledged Unix shell.
- Error handling and memory management are key components of this project.
- It's important to implement proper signal handling to manage interruption signals like `Ctrl+C` and `Ctrl+\`.

## Authors

- Asendar1 (Hamzah Asendar)
- m-darawsheh (Mohammed Darawsheh)

## License

This project is open-source and available under the MIT License.

