PLUGINS_DIR= ../../../plugins/

SRC=    plugin_analyse_invalide_instruction.cpp
OBJ=	$(SRC:.cpp=.o)
BIN=	$(PLUGINS_DIR)/plugin_analyse_invalide_instruction.so

CXX=		g++
CXXFLAGS=	-W -Wall -Wextra -Werror -Weffc++    -I../../utils/ -ldl -lpthread -fPIC -shared

all: msg $(BIN)

$(BIN): $(OBJ) ../../libutils.a
	$(CXX) -o $@ $^ $(CXXFLAGS)

msg:
	@echo "\033[01;34m \nCompilation du plugin " $(BIN) " \033[00m"

clean:
	@echo "\033[01;34m \nSuppression des .o du plugin " $(BIN) " \033[00m"
	rm -f $(OBJ)

fclean: clean
	@echo "\033[01;34m \nSuppression du plugin " $(BIN) " \033[00m"
	rm -f $(BIN)

re: fclean all

