NAME = UDP-Flood-Win64.exe

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -Iinclude

LDFLAGS = -lws2_32

SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $(NAME) $(LDFLAGS)
	del /Q *.o 2>nul

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q *.o 2>nul

fclean: clean
	del /Q $(NAME) 2>nul

re: fclean all

.PHONY: all clean fclean re