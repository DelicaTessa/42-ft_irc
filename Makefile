# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: tessaclement <tessaclement@student.coda      +#+                      #
#                                                    +#+                       #
#    Created: 2022/08/24 08:42:23 by tessaclemen   #+#    #+#                  #
#    Updated: 2022/08/24 15:07:15 by tessaclemen   ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

CXX = c++
VERBOSE = 1
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -D VERBOSE=$(VERBOSE)
NAME = ircserv 
SRC =	srcs/main.cpp \
		srcs/Server.cpp \
		srcs/Client.cpp \
		srcs/Command.cpp \
		srcs/Channel.cpp \
		srcs/commands/pass.cpp \
		srcs/commands/nick.cpp \
		srcs/commands/user.cpp \
		srcs/commands/quit.cpp \
		srcs/commands/oper.cpp \
		srcs/commands/mode.cpp \
		srcs/commands/join.cpp \
		srcs/commands/part.cpp \
		srcs/commands/ping.cpp \
		srcs/commands/kill.cpp \
		srcs/commands/privmsg.cpp \
		srcs/commands/away.cpp \
		srcs/commands/notice.cpp \
		srcs/commands/invite.cpp	\
		srcs/commands/topic.cpp \
		srcs/commands/names.cpp \
		srcs/commands/list.cpp \
		srcs/commands/kick.cpp \
		srcs/commands/whois.cpp \
		srcs/commands/who.cpp \

OBJ = $(SRC:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean :
	@rm -rf $(OBJ)

fclean : clean
	@rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
