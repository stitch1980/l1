PLUGINS_DIR= ../../../plugins/

SRC_FORMAT=	plugin_parse_format_brainfuck.cpp
SRC_INTERRUPT=  plugin_parse_init_interrupt_brainfuck.cpp
SRC_DEASM=	plugin_deasm_brainfuck.cpp \
		ASM_brainfuck.cpp

OBJ_FORMAT=	$(SRC_FORMAT:.cpp=.o)
OBJ_INTERRUPT=	$(SRC_INTERRUPT:.cpp=.o)
OBJ_DEASM=	$(SRC_DEASM:.cpp=.o)

BIN_FORMAT=	$(PLUGINS_DIR)/plugin_parse_format_brainfuck.so
BIN_INTERRUPT=	$(PLUGINS_DIR)/plugin_parse_init_interrupt_brainfuck.so
BIN_DEASM=	$(PLUGINS_DIR)/plugin_deasm_brainfuck.so

CXX=		g++
CXXFLAGS=	-W -Wall -Wextra -Werror -I../../utils/ -ldl -lpthread -fPIC -shared

all: msg_format $(BIN_FORMAT) msg_interrupt $(BIN_INTERRUPT) msg_deasm $(BIN_DEASM)

$(BIN_FORMAT): $(OBJ_FORMAT) ../../libutils.a
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(BIN_INTERRUPT): $(OBJ_INTERRUPT) ../../libutils.a
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(BIN_DEASM): $(OBJ_DEASM) ../../libutils.a
	$(CXX) -o $@ $^ $(CXXFLAGS)

msg_format:
	@echo "\033[01;34m \nCompilation du plugin " $(BIN_FORMAT) " \033[00m"

msg_interrupt:
	@echo "\033[01;34m \nCompilation du plugin " $(BIN_INTERRUPT) " \033[00m"

msg_deasm:
	@echo "\033[01;34m \nCompilation du plugin " $(BIN_DEASM) " \033[00m"

clean:
	@echo "\033[01;34m \nSuppression des .o du plugin " $(BIN_FORMAT) " \033[00m"
	rm -f $(OBJ_FORMAT)
	@echo "\033[01;34m \nSuppression des .o du plugin " $(BIN_INTERRUPT) " \033[00m"
	rm -f $(OBJ_INTERRUPT)
	@echo "\033[01;34m \nSuppression des .o du plugin " $(BIN_DEASM) " \033[00m"
	rm -f $(OBJ_DEASM)

fclean: clean
	@echo "\033[01;34m \nSuppression du plugin " $(BIN_FORMAT) " \033[00m"
	rm -f $(BIN_FORMAT)
	@echo "\033[01;34m \nSuppression du plugin " $(BIN_INTERRUPT) " \033[00m"
	rm -f $(BIN_INTERRUPT)
	@echo "\033[01;34m \nSuppression du plugin " $(BIN_DEASM) " \033[00m"
	rm -f $(BIN_DEASM)

re: fclean all

