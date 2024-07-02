#include "strace.h"

/**
 * traceProcess2 - program that traces system calls of a child process
 * this function creates a child process using fork() and sets up
 * tracing to monitor the system calls made by the child process;
 * it waits for syscall entry and exit, retrieves the syscall
 * number, prints its name, and prints the return value of the syscall;
 * it is impossible to retrieve the last system call return value,
 * simply because it does not return;
 * so, we just have to print a ? like strace does
 * @args: an array of command-line arguments
 * @env: an array of environmental variables
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */

int traceProcess2(char *args[], char *env[])
{
	int status;
	long syscallNumber, syscallReturnValue;
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
		if (waitSyscall2(childPid) != 0)
			break;
		/* Retrieve syscall number and print its name */
		syscallNumber = ptrace(PTRACE_PEEKUSER, childPid,
				       sizeof(long) * ORIG_RAX);
		fprintf(stdout, "%s", syscalls_64_g[syscallNumber].name);

		/* Wait for syscall exit */
		if (waitSyscall2(childPid) != 0)
			break;

		syscallReturnValue = ptrace(PTRACE_PEEKUSER, childPid,
					    sizeof(long) * RAX);
		fprintf(stdout, " = %#lx\n", syscallReturnValue);
	}
	fprintf(stdout, " = ?\n");
	return (EXIT_SUCCESS);
}

/**
 * waitSyscall2 - program that waits for a syscall invocation
 * in the child process
 * @child: PID of the child process
 * Return: 0 if syscall is called, 1 if subprocess is terminated
 */

int waitSyscall2(pid_t child)
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
 * main - the entry point of the program that traces system calls of another program
 * this function acts as the launcher for tracing system calls of a specified program;
 * it requires at least one argument (the program to trace);
 * it will initialize the tracing process by calling 'traceProcess2',
 * passing along any command-line arguments
 * @argc: the argument count
 * @argv: the argument vector
 * @envp: the environmental variables
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
	return (traceProcess2(argv + 1, envp));
}
