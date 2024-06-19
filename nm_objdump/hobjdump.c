#include "hobjdump.h"

/**
 * main - the entry point
 * 
 * program that prepares an argument vector to execute the 'objdump' command
 * with specific flags to display the file headers and the full contents of all
 * the sections in the binary file specified by the user;
 * it expects the path to the binary file as its second command-line argument
 * 
 * @argc: the count of command-line arguments
 * @argv: the array of command-line arguments;
 *        argv[1] should contain the file path to be dumped
 * @env: the environment variables array passed to the 'objdump' command
 *       upon execution
 * 
 * Return: EXIT_SUCCESS upon successful execution of 'objdump',
 *         otherwise returns EXIT_FAILURE
 */

int main(int argc, char **argv, char **env)
{
    char *av[] = {"/usr/bin/objdump", "-s", "-f", "", NULL};

    (void)argc;
    av[3] = argv[1];

    if (execve("/usr/bin/objdump", av, env) == -1)
    {
        perror("execv");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}