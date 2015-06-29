PLUGINS_DIR= ../../../plugins/

SRC=	plugin_parse_format_MBR.cpp
OBJ=	$(SRC:.cpp=.o)
BIN=	$(PLUGINS_DIR)/plugin_parse_format_MBR.so

CXX=		g++
CXXFLAGS=	-W -Wall -Wextra -Werror -Weffc++    -I../../utils/ -ldl -lpthread -fPIC -shared

all: $(BIN)

$(BIN): $(OBJ) ../../libutils.a
	@echo "\033[01;34m \nCompilation du plugin " $(BIN) " \033[00m"
	$(CXX) -o $@ $^ $(CXXFLAGS)


clean:
	@echo "\033[01;34m \nSuppression des .o du plugin " $(BIN) " \033[00m"
	rm -f $(OBJ)$

fclean: clean
	@echo "\033[01;34m \nSuppression du plugin " $(BIN) " \033[00m"
	rm -f $(BIN)

re: fclean all

