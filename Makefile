LIBFT_NAME := libft.a
LIBFT_GH := https://github.com/josejpgg/libft_increment.git
LIBFT_PATH := ./lib/libft/
PROGRAM_NAME := minishell
CC = cc
FLAGS = 
# FLAGS = -Wall -Werror -Wextra
SOURCE := main.c env.c param.c \
safe_func.c interactive.c safe_free.c str_util.c \
command.c line.c pipe.c redirection.c token.c \
cmd_impl.c vector.c cmd_util.c builtin_impl.c \
history.c history2.c \
./builtins/controller.c \
./builtins/cd.c ./builtins/echo.c ./builtins/env.c ./builtins/exit.c \
./builtins/export.c ./builtins/pwd.c ./builtins/unset.c \
./util/quote.c ./util/error.c ./util/util.c
# READLINE := -lreadline -lncurses
READLINE := -I/opt/homebrew/opt/readline/include -L/opt/homebrew/opt/readline/lib -lreadline
# READLINE := -I/opt/homebrew/opt/readline/include -L/opt/homebrew/opt/readline/lib -lreadline
COMPILE := ${SOURCE:.c=.o}

all: minishell

%.o: %.c
	$(CC) $(READLINE) $(FLAGS) -c $< -o $@

minishell: $(COMPILE)
	$(CC) $(FLAGS) $(COMPILE) $(LIBFT_NAME) $(READLINE) -o ${PROGRAM_NAME}

clean:
	@rm -rf $(COMPILE)
	@make clean -C $(LIBFT_PATH)

fclean:
	@rm -rf ${PROGRAM_NAME}
	@make fclean -C $(LIBFT_PATH)
	@rm -rf ${LIBFT_NAME}

re: fclean clean libft lib all

lib:
	@if [ ! -f "$(LIBFT_NAME)" ]; then \
		make -C $(LIBFT_PATH); \
		mv $(LIBFT_PATH)/$(LIBFT_NAME) .; \
	fi

libft:
	@if [ ! -d "./lib" ]; then \
		mkdir -p lib; \
	fi
	@if [ -d "./lib/libft" ]; then \
		rm -rf ./lib/libft; \
	fi
	git clone $(LIBFT_GH) $(LIBFT_PATH)

test: all clean
	# echo 'grep hi "<infile"' | ./minishell
	# echo 'grep hi "<infile" <         ./test_files/infile' | ./minishell
	echo '' | ./minishell

leaksMac:
	export MallocStackLogging=1
	leaks --atExit --list -- echo "pwd" | ./minishell

# leaks --atExit --list -- ./minishell
	
# --trace-children=yes

leaksCampus:
	valgrind --leak-check=yes ./[program] [params]

.PHONY: all clean fclean re lib
