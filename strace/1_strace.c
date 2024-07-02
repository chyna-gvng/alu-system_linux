#include "strace.h"

/**
 * traceProcess - program that traces system calls of a child process
 * @args: an array of command-line arguments
 * @env: an array of environmental variables
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */

int traceProcess(char *args[], char *env[])
{
	int status;
	long syscallNumber;
	pid_t childPid = fork();

	if (childPid < 0)
	{
		return (EXIT_FAILURE);
	}
	else if (childPid == 0)
	{
		/* Child process: Prepare for tracing and execute the command */
		ptrace(PTRACE_TRACEME);
		kill(getpid(), SIGSTOP);

		return (execve(args[0], args, env));
	}

	/* Parent process: Set up tracing and trace syscall execution */
	waitpid(childPid, &status, 0);
	setbuf(stdout, NULL);
	ptrace(PTRACE_SETOPTIONS, childPid, 0, PTRACE_O_TRACESYSGOOD);

	while (1)
	{
		/* Wait for syscall entry */
		if (waitSyscall(childPid) != 0)
			break;

		/* Retrieve syscall number and print its name */
		syscallNumber = ptrace(PTRACE_PEEKUSER, childPid,
				       sizeof(long) * ORIG_RAX);
		fprintf(stdout, "%s", syscalls_64_g[syscallNumber].name);

		/* Wait for syscall exit */
		if (waitSyscall(childPid) != 0)
			break;

		fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
	return (EXIT_SUCCESS);
}

/**
 * waitSyscall - program that waits for a syscall invocation
 * in the child process
 * @child: PID of the child process
 * Return: 0 if syscall is called, 1 if subprocess is terminated
 */

int waitSyscall(pid_t child)
{
	int status;

	while (1)
	{
		ptrace(PTRACE_SYSCALL, child, 0, 0);
		waitpid(child, &status, 0);

		if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80)
			return (0);

		if (WIFEXITED(status))
			break;
	}
	return (1);
}

/**
 * main - the entry point of the program that initializes the process tracer
 * if the required minimum number of arguments is not provided,
 * the program will prompt the user to supply the needed command
 * arguments and terminate with an error;
 * otherwise, it starts the tracing process by passing control
 * to the 'traceProcess' function
 * @argc: the count of command-line arguments
 * @argv: the array of command-line arguments
 * @envp: the array of environment variables
 * Return: EXIT_FAILURE if failed, EXIT_SUCCESS if successful
 */

int main(int argc, char *argv[], char *envp[])
{
	if (argc < 2)
	{
		fprintf(stderr, "%s command [args...]\n", argv[0]);
		return (EXIT_FAILURE);
	}

	/* Start tracing the process and its system calls */
	return (traceProcess(argv + 1, envp));
}
