See [subject](https://github.com/cariestevez/minishell/blob/main/minishell.en.subject.pdf)

# 🐚 minishell

## Learning goals:
- Understand the concept and division of labor of between the Lexer, Parser, Expander & Executor which lie at the core of many applications that involve complex user input processing.
- Concurrently getting comforatble with core features of bash, e.g. environment variables, (multi-) redirection of output, chaining of commands etc.
- Learn to collaborate on a coding project which means developing strategies to split, review and integrate code.
- Learning how to do *Reverse Engineering*
- Maintaining a bigger code base (Version control, file system management, naming conventions, documentation...).

## Core features:
- execute binaries given an absolute, relative path or using the PATH environment variable
- standard i/o redirections (>, <, >>), as well as the heredoc
- implement recursion for subshells "()" with their i/o redirection
- executing multiple commands simulaneously and subsequently with pipe operators
- read, save and manipulate environment variables
- handle signals CTRL+D, CTRL+C, CTRL+/
- implement some of the built-in functions (cd, export, unset, exit...)
- ...

## Usage:
* Compile the code using the `make` command
* Launch `./minishell`
* Play with it!
* Exit the shell with `CTRL+D` or execute `exit`

## Resources:
- ```bash```
- [GNU bash manual](https://www.gnu.org/software/bash/manual/bash.html)
