DIR=		../

SRC_LIB_UTILS=	utils/Analyse.cpp \
		utils/BNFcalcul.cpp \
		utils/BNFc.cpp \
		utils/Info.cpp \
		utils/Plugin.cpp \
		utils/Section.cpp \
		utils/Symbole.cpp \
		utils/PrototypeF.cpp \
		utils/PrototypeS.cpp \
		utils/PrototypeT.cpp \
		utils/Fonction.cpp \
		utils/Mutex.cpp \
		utils/Calcul.cpp \
		utils/Contexte.cpp \
		utils/ContentContexte.cpp \
		utils/IASM.cpp \
		utils/Thread.cpp
OBJ_LIB_UTILS=	$(SRC_LIB_UTILS:.cpp=.o)
BIN_LIB=	./libutils.a

SRC=		main.cpp
OBJ=		$(SRC:.cpp=.o)
BIN=		$(DIR)/lecter
BIN_GUI=	$(DIR)/gui

CXX=		g++
CXXFLAGS=	-W -Wall -Wextra -Werror -Weffc++      -I./utils/ -ldl -lpthread -fPIC


all: msg_lib $(BIN_LIB) msg_src $(BIN)
	make all -C ./plugins/
	@echo "\033[01;31m \nCompilation de la GUI du decompilateur \033[00m"
	make -C ./gui_decompilateur
	cp ./gui_decompilateur/gui $(BIN_GUI)

msg_lib:
	@echo "\033[01;31m \nCompilation des sources utiles \033[00m"

msg_src:
	@echo "\033[01;31m \nCompilation du decompilateur \033[00m"

$(BIN_LIB): $(OBJ_LIB_UTILS)
	ar rc $(BIN_LIB) $^
	ranlib $(BIN_LIB)

$(BIN): $(OBJ) $(BIN_LIB)
	@echo "\033[01;31m \nCompilation du coeur \033[00m"
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	@echo "\033[01;31m \nSuppression des .o de la lib \033[00m"
	rm -f $(OBJ_LIB_UTILS)
	@echo "\033[01;31m \nSuppression des .o \033[00m"
	rm -f $(OBJ)
	@echo "\033[01;31m \nSuppression des .o de la GUI du decompilateur \033[00m"
	rm -f ./gui_decompilateur/*.o
	make clean -C ./plugins/

fclean: clean
	@echo "\033[01;31m \nSuppression de la lib \033[00m"
	rm -f $(BIN_LIB)
	@echo "\033[01;31m \nSuppression du coeur \033[00m"
	rm -f $(BIN)
	@echo "\033[01;31m \nSuppression de la GUI du decompilateur \033[00m"
	rm -f $(BIN_GUI)
	rm -f ./gui_decompilateur/gui
	make fclean -C ./plugins/

re: fclean all

