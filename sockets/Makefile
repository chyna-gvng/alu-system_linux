CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -I. -include socket2.h

# Sources
COMMON_SRCS = server.c trim.c
COMMON_OBJS = $(COMMON_SRCS:.c=.o)

APIS = todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 todo_api_5 todo_api_6 todo_api_7

.PHONY: all clean oclean fclean re $(APIS)

all: $(APIS)

todo_api_0: 4-request.o $(COMMON_OBJS)
	$(CC) $^ -o $@

todo_api_1: 5-queries.o $(COMMON_OBJS)
	$(CC) $^ -o $@

todo_api_2: 6-headers.o $(COMMON_OBJS)
	$(CC) $^ -o $@

todo_api_3: 7-body_parameters.o $(COMMON_OBJS)
	$(CC) $^ -o $@

todo_api_4: 8-create_todo.o $(COMMON_OBJS)
	$(CC) $^ -o $@

todo_api_5: 9-retrieve_todos.o $(COMMON_OBJS)
	$(CC) $^ -o $@

todo_api_6: 10-retrieve_single_todo.o $(COMMON_OBJS)
	$(CC) $^ -o $@

todo_api_7: 11-delete_todo.o $(COMMON_OBJS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o $(APIS)

oclean:
	$(RM) *.o

fclean: clean
	$(RM) $(APIS)

re: fclean all
