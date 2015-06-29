PLUGINS_DIR= ../../../plugins/

SRC=	plugin_open_file.cpp
OBJ=	$(SRC:.cpp=.o)
BIN=	$(PLUGINS_DIR)/plugin_open_file.so

CXX=		g++
CXXFLAGS=	-W -Wall -Wextra -Werror -Weffc++    -I../../utils/ -ldl -lpthread -fPIC -shared

all: $(BIN)

$(BIN): $(OBJ) ../../libutils.a
	@echo "\033[01;34m \nCompilation du plugin " $(BIN) " \033[00m"
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	@echo "\033[01;34m \nSuppression des .o du plugin " $(BIN) " \033[00m"
	rm -f $(OBJ)

fclean: clean
	@echo "\033[01;34m \nSuppression du plugin " $(BIN) " \033[00m"
	rm -f $(BIN)

re: fclean all
