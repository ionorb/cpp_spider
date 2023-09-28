# makefile to build the cpp_spider webscraper project

# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -g3 -std=c++11

# include path
INCLUDES = -I includes/

# includes
CFLAGS += $(INCLUDES)

# Source files
FILES = main.cpp \
		Spider.cpp

# Source path
SRC = $(addprefix sources/, $(FILES))

# Object files
OBJ = $(SRC:.cpp=.o)

# Executable name
NAME = spider

# Build rule
all: $(NAME)

# Linking rule
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

# Compiling rule
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJ)

# Fclean rule
fclean: clean
	rm -f $(NAME)

# Re rule
re: fclean all

# Phony
.PHONY: all clean fclean re
