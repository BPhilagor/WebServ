#  **************************************************************************  #
#                                                      __ __   __              #
#     WebServ                                         / // / /__ \             #
#                 By: skoulen, bphilago, znichola    / // /_ __/ /             #
#                                                   /__  __// __/              #
#                 Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch  #
#                                                                              #
#  **************************************************************************  #

NAME	= webserv

CC		= c++
CFLAGS	= -Wall -Wextra
CFLAGS	+= -Werror
CFLAGS	+= -std=c++98 -pedantic

ifdef DEBUG
CFLAGS	+= -g3 -fsanitize=address
elses
ifdef DEBUGL
CFLAGS += -g3
endif
endif
ifdef WSL_DISTRO_NAME
LEAKS_CHECK = valgrind
CFLAGS	+= -Wno-unknown-pragmas -DWSL_DISTRO_NAME=Ubuntu
else
LEAKS_CHECK = leaks -atExit --
endif

FILES	= main utils Data launchServers requestWorker handler \
		HTTPRequest HTTPHeaders HTTPResponse \
		HTTPResponse_informationalResponse \
		HTTPResponse_successfulResponse \
		HTTPResponse_redirectionMessage \
		HTTPResponse_clientErrorResponse \
		HTTPResponse_serverErrorResponse \
		HTTPResponse_reason

OBJS_PATH = objs/
SRCS_PATH = src/
INCS_PATH = $(SRCS_PATH)/headers/

SRCS	= $(addprefix $(SRCS_PATH), $(addsuffix .cpp, $(FILES)))
OBJS	= $(addprefix $(OBJS_PATH), $(addsuffix .o, $(FILES)))

all	: $(NAME)

$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -I$(INCS_PATH) -o $@ $<

$(NAME)	: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean	:
	$(RM) $(OBJS)

fclean	: clean
	$(RM) $(NAME)

re	: fclean all

run : all
	./$(NAME) $(ARGS)

leaks : re
	$(LEAKS_CHECK) ./$(NAME) $(ARGS)

.PHONY: all clean fclean re
