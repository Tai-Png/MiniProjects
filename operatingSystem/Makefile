all: my_current_processes my_memory_util my_version

my_current_processes: my_current_processes.c
	gcc -g -Wall my_current_processes.c -o my_current_processes

my_memory_util: my_memory_util.c
	gcc -g -Wall my_memory_util.c -o my_memory_util

my_version: my_version.c
	gcc -g -Wall my_version.c -o my_version

clean:
	rm -f my_current_processes my_memory_util my_version *.o
	