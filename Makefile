# Colors

C_GREY		=	\033[1;30m
C_RED		=	\033[1;31m
C_GREEN		=	\033[1;32m
C_YELLOW	=	\033[1;33m
C_BLUE		=	\033[1;34m
C_PURPLE	=	\033[1;35m
C_CYAN		=	\033[1;36m
C_WHITE		=	\033[1;37m
C_END		=	\033[0m


NAME	=	ft_containers

CC	=	clang++

RM	=	rm -rf

CP	=	cp -r

CFLAGS	=	-Wall -Wextra -Werror -std=c++98

TFLAGS	=	-Wall -Wextra -Werror -std=c++98

SRC		+=	main.cpp


OBJ_PATH	=	./.obj/

vpath %.cpp .

OBJ		=	$(patsubst %.cpp, $(OBJ_PATH)%.o, $(SRC))

HEADER_NAME	=	stack.hpp ft.hpp vector.hpp map.hpp

HEADER_PATH	=	./includes/

HEADER		=	$(addprefix $(HEADER_PATH)/, $(HEADER_NAME))

all: $(NAME)

$(NAME): $(OBJ_PATH) $(OBJ)
	@$(CC) -I$(HEADER_PATH) -o $(NAME) $(OBJ) $(TFLAGS)
	@echo "$<		[$(C_GREEN)OK$(C_END)]"

$(OBJ): $(OBJ_PATH)%.o: %.cpp $(HEADER)
	$(CC) -I$(HEADER_PATH) $(CFLAGS) -c $< -o $@

$(OBJ_PATH):
	@mkdir $(OBJ_PATH)

clean:
	@$(RM) $(OBJ_PATH)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

debug:
	@echo $(OBJ)
	@echo $(SRC)