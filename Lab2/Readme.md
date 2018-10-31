# IUPUI CSCI 503 Fall 2017 Lab 2

## A Simple Shell

### Background

This project is a simple shell that invokes system calls. It takes inputs from the user, parses the command, validates
the input, and executes the system call if valid.

The project uses `open`, `dup2`, `pipe`, `execvp`, `wait` and `fork` system calls.

It follows the following command line grammar:

```
command lineàcmd [< fn] [| cmd]* [> fn] [&] EOL cmdàcn [ar]*
cnà<a string> //command name
fnà<a string> //file name
arà<a string> //argument
```

It supports file redirection (`< file` and `> file`) and chaining commands (`cmd1 | cmd2 ...`);

### File Description

* **shell.c** : Source code for the shell
* **shell** : Executable file generated after compilation

### Usage

* Run `make` command to create the *shell* executable file
* Run `make clean` command to clear all object and executable files
