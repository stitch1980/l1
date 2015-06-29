
all:
	@echo "\033[01;31m \nCompilation des plugins \033[00m"
	make all -C ./plugin_open_file
	make all -C ./plugin_parse_brainfuck
	make all -C ./plugin_parse_format_ELF
	make all -C ./plugin_parse_format_MBR
	make all -C ./plugin_parse_format_PE
	make all -C ./plugin_clean_spaghetti
	make all -C ./plugin_clean_junk_code
	make all -C ./plugin_clean_invalid
	make all -C ./plugin_clean_nomme_fonction
	make all -C ./plugin_decompile_effect
	make all -C ./plugin_analyse_invalide_instruction
	make all -C ./plugin_analyse_ROPgadget
	make all -C ./plugin_analyse_search_vulnerable_functions
	make all -C ./plugin_analyse_search_vulnerable_printf
	
	make all -C ./plugin_init_interrupt_dos
	make all -C ./plugin_init_interrupt_intel_linux
	make all -C ./plugin_type_simple
	make all -C ./plugin_deassembleur
	make all -C ./plugin_intel
	make all -C ./plugin_fonction_asm_to_graphe_dot
	
	make all -C ./plugin_test_creation_segment

clean:
	@echo "\033[01;31m \nSuppression des .o des plugins \033[00m"
	make clean -C ./plugin_open_file
	make clean -C ./plugin_parse_brainfuck
	make clean -C ./plugin_parse_format_ELF
	make clean -C ./plugin_parse_format_MBR
	make clean -C ./plugin_parse_format_PE
	make clean -C ./plugin_clean_spaghetti
	make clean -C ./plugin_clean_junk_code
	make clean -C ./plugin_clean_invalid
	make clean -C ./plugin_clean_nomme_fonction
	make clean -C ./plugin_decompile_effect
	make clean -C ./plugin_analyse_invalide_instruction
	make clean -C ./plugin_analyse_ROPgadget
	make clean -C ./plugin_analyse_search_vulnerable_functions
	make clean -C ./plugin_analyse_search_vulnerable_printf
	
	make clean -C ./plugin_init_interrupt_dos
	make clean -C ./plugin_init_interrupt_intel_linux
	make clean -C ./plugin_type_simple
	make clean -C ./plugin_deassembleur
	make clean -C ./plugin_intel
	make clean -C ./plugin_fonction_asm_to_graphe_dot
	
	make clean -C ./plugin_test_creation_segment

fclean:
	@echo "\033[01;31m \nSuppression des plugins \033[00m"
	make fclean -C ./plugin_open_file
	make fclean -C ./plugin_parse_brainfuck
	make fclean -C ./plugin_parse_format_ELF
	make fclean -C ./plugin_parse_format_MBR
	make fclean -C ./plugin_parse_format_PE
	make fclean -C ./plugin_clean_spaghetti
	make fclean -C ./plugin_clean_junk_code
	make fclean -C ./plugin_clean_invalid
	make fclean -C ./plugin_clean_nomme_fonction
	make fclean -C ./plugin_decompile_effect
	make fclean -C ./plugin_analyse_invalide_instruction
	make fclean -C ./plugin_analyse_ROPgadget
	make fclean -C ./plugin_analyse_search_vulnerable_functions
	make fclean -C ./plugin_analyse_search_vulnerable_printf
	
	make fclean -C ./plugin_init_interrupt_dos
	make fclean -C ./plugin_init_interrupt_intel_linux
	make fclean -C ./plugin_type_simple
	make fclean -C ./plugin_deassembleur
	make fclean -C ./plugin_intel
	make fclean -C ./plugin_fonction_asm_to_graphe_dot
	
	make fclean -C ./plugin_test_creation_segment

re: fclean all


