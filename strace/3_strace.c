#include "strace.h"

/**
 * print_params - program that prints syscall parameters as hex values
 * in a comma-delimited series
 * @regs: pointer to user_regs_struct containing latest registers
 *        queried from tracee
 * Return: nothing (void)
 */

void print_params(struct user_regs_struct *regs)
{
	size_t i;
	unsigned long param;
	syscall_t syscall = syscalls_64_g[regs->orig_rax];

	if (!regs)
		return;

	for (i = 0; i < syscall.nb_params; i++)
	{
		if (syscall.params[i] == VOID)
			continue;

		switch (i)
		{
			case 0:
				param = (unsigned long)regs->rdi;
				break;
			case 1:
				param = (unsigned long)regs->rsi;
				break;
			case 2:
				param = (unsigned long)regs->rdx;
				break;
			case 3:
				param = (unsigned long)regs->r10;
				break;
			case 4:
				param = (unsigned long)regs->r8;
				break;
			case 5:
				param = (unsigned long)regs->r9;
				break;
			default:
				return;
		}

		if (syscall.params[i] == VARARGS)
			printf("...");
		else
			printf("%#lx%s", param,
			       (i < syscall.nb_params - 1) ? ", " : "");
	}
}

/**
 * trace_loop - program that traces syscalls and prints their names
 * and parameters
 * @child_pid: process ID of tracee/child
 * Return: 0 on successful completion, 1 on failure
 */

int trace_loop(pid_t child_pid)
{
	int status, syscall_return, first_syscall;
	struct user_regs_struct regs;

	first_syscall = 1;
	syscall_return = 1;

	while (1)
	{
		if (wait(&status) == -1)
			return (1);

		if (WIFEXITED(status))
		{
			printf(") = ?\n");
			break;
		}

		if (ptrace(PTRACE_GETREGS, child_pid, NULL, &regs) == -1)
			return (1);

		if (!syscall_return || first_syscall)
		{
			printf("%s(", syscalls_64_g[regs.orig_rax].name);
			print_params(&regs);
			fflush(stdout);
			first_syscall = 0;
		}

		if (syscall_return)
			printf(") = %#lx\n", (unsigned long)regs.rax);

		if (ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1)
			return (1);

		syscall_return = syscall_return ? 0 : 1;
	}

	return (0);
}

/**
 * main - the entry point for the syscall tracer program, which monitors
 * and displays system call usage of another running process
 * the program requires at least one argument to run correctly, the name
 * and optionally, arguments of the program to be traced;
 * it executes the specified program and begins tracing its system calls,
 * printing out each call as it occurs along with its parameters
 * and return values
 * @argc: count of command line parameters
 * @argv: array of command line parameters
 * @envp: array of environmental variables
 * Return: 0 on success, 1 on failure
 */

int main(int argc, char *argv[], char *envp[])
{
	pid_t pid;

	if (argc < 2 || !argv)
	{
		fprintf(stderr, "usage: %s <prog> <prog args>...\n", argv[0]);
		return (1);
	}

	switch (pid = fork())
	{
		case -1:
			return (1);
		case 0:
			if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
				return (1);
			if (execve(argv[1], argv + 1, envp) == -1)
				return (1);
		default:
			break;
	}

	return (trace_loop(pid));
}
