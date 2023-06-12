SRCDIR		= src
OBJDIR		= obj
NAME		= webServ
FILES		= main.cpp utils.cpp
SRCS		= ${addprefix src/, ${FILES}}
OBJS		= ${addprefix obj/, ${FILES:.cpp=.o}}
CC			= c++ -std=c++98
FLAGS		= -Wall -Wextra -Werror
INCLDIR		= src/headers/

all		: ${NAME}

$(NAME)	: ${OBJS}
		${CC} ${FLAGS} -I${INCLDIR} -o ${NAME} ${OBJS}

${OBJS}	: ${SRCS}
		${CC} ${FLAGS} -I${INCLDIR} -c $< -o $@

clean	:
		rm -f ${OBJS}

fclean	:	clean
		rm -f ${NAME}

re:		fclean all

test	:
	echo ${SRCS}

leak	:
	leaks -atExit -- ./${NAME}

.PHONY : all re clean fclean leak