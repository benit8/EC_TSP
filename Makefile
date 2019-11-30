NAME	=	tsp

SRCS	=	sources/main.cpp			\
		sources/Graph.cpp			\
		sources/Graph.Node.cpp			\
		sources/Population.cpp			\
		sources/Population.Individual.cpp	\

OBJS	=	$(SRCS:.cpp=.o)

CXXFLAGS	+=	-std=c++17
CXXFLAGS	+=	-W -Wall -Wextra

################################################################################

all: $(NAME)

$(NAME): $(OBJS)
	g++ -o $(NAME) $(OBJS) $(LDFLAGS)

graph: fclean
graph: CXXFLAGS += -DTSP_GRAPHICS
graph: LDFLAGS += -lsfml-system -lsfml-window -lsfml-graphics
graph: all

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all $(NAME) clean fclean re