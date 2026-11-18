NAME    = Codexion
HEADER  = codex.h
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -I./
RM      = rm -f

SRC     = main.c \
		parse.c \
		ultis.c \
		routine.c \
		burn.c
OBJ     = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@echo "⋆ ˚｡⋆୨$(NAME)୧⋆ ˚｡ compiled ⋆ ˚｡"

%.o: %.c $(HEADER)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJ)
	@echo "Objects removed /ᐠ. .ᐟ\ฅ"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(NAME) removed /ᐠ. .ᐟ\ฅ"

re: fclean all

.PHONY: all clean fclean re
