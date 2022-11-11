# SHELL
Goal was to understand the relationship between OS command interpreters (shells), system calls and the kernel
Designed and implemented an extremely simple shell and system call

Designed to be used on a Linux system

Supports built-in commands:
  exit
  log
  print
  theme
  
Non built-in commands:
  commands such as ls, pwd, whoami is executed in the child process, then output is printed. 
  child processes are created using fork() system call 
  shell then waits for child process to termiante via wait() call

Two modes of operation
  interactive mode is activated when there are no command line arguments
  script mode is activated when there are arguments, such as ./cshell <filename>
    this file will contain lines of code in the following format, which will then be parsed
      <command1> <arg0> <arg1> ... <argN>
