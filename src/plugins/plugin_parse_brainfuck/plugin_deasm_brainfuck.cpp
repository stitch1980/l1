/*
** plugin_deasm_brainfuck.cpp for plugin_deasm_brainfuck
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 14:34:37 joran herve
** Last update Sat, 27 Jun 2015 14:34:37 joran herve
**
** Status : OK (documentation faite)
*/

#include    "Info.hpp"
#include    "ASM_brainfuck.hpp"


#define PLUGIN_DESCR_TYPE        PLUGIN_TYPE__DEASM
#define PLUGIN_DESCR_PRIORITE    "5000"
#define PLUGIN_DESCR_NAME        "plugin_deasm_brainfuck"
#define PLUGIN_DESCR_VERSION     "0"
#define PLUGIN_NAME              PLUGIN_DESCR_TYPE "." PLUGIN_DESCR_PRIORITE "." PLUGIN_DESCR_NAME "." PLUGIN_DESCR_VERSION


extern "C"
{
    const char    *PLUGIN_FUNC_GET_PLUGIN_NAME();
    int           PLUGIN_FUNC_EXECUTE(Info *info);
    const char    *PLUGIN_FUNC_APROPOS();
}


/**
** \fn const char *func_emplacement_param(unsigned long num)
** \brief Fonction permettant de connaitre l'emplacement des parametres d'interruptions
**
** \param num Numero du parametre a localiser
** \return Retourne l'emplacement du parametre si OK, NAME_PC sinon
*/
const char    *func_emplacement_param(unsigned long)
{
    return ("[ptr+index*4]");
}

/**
** \fn const char *func_emplacement_return()
** \brief Fonction permettant de connaitre l'emplacement de la valeur de retour des interruptions
**
** \return Retourne l'emplacement de la valeur de retour
*/
const char    *func_emplacement_return()
{
    return ("[ptr+index*4]");
}

/**
** \fn const char *func_emplacement_numero()
** \brief Fonction permettant de connaitre le numero d'une interruption
**
** \return Retourne l'emplacement du numero d'interruption
*/
const char    *func_emplacement_numero()
{
    return ("[" NAME_PC "]");
}




/**
** \fn const char *get_plugin_name()
** \brief Fonction permettant de connaitre le nom du plugin
**
** \return Retourne un pointeur vers le nom du plugin
*/
const char    *PLUGIN_FUNC_GET_PLUGIN_NAME()
{
    return (PLUGIN_NAME);
}

/**
** \fn int execute(Info *info)
** \brief Fonction permettant d'executer le plugin pour recuperer le contenu du fichier
**
** \param info Classe contenant les infos du programme a analyser
** \return Retourne 1 si OK, -1 sinon
*/
int        PLUGIN_FUNC_EXECUTE(Info *info)
{
    if (info == NULL)
        return (0);

    /* Si l'executable a analyser est un binaire i64, on prepare son deassembleur */
    if (info->archi == Info::ARCHI_BRAINFUCK)
    {
        info->endian = Info::ENDIAN_LITTLE;

        /* Initialisation des pointeurs des fonctions permettant d'acceder aux emplacements des parametres */
        info->ptr_func.f_param_func = &func_emplacement_param;
        info->ptr_func.f_ret_func = &func_emplacement_return;

        info->ptr_func.f_num_int = &func_emplacement_numero;
        info->ptr_func.f_param_int = &func_emplacement_param;
        info->ptr_func.f_ret_int = &func_emplacement_return;

        info->ptr_func.f_num_syscall = &func_emplacement_numero;
        info->ptr_func.f_param_syscall = &func_emplacement_param;
        info->ptr_func.f_ret_syscall = &func_emplacement_return;

        if (info->ptr_func.deasm != NULL)
            delete info->ptr_func.deasm;
        info->ptr_func.deasm = NULL;

        if ((info->ptr_func.deasm = new ASM_brainfuck) == NULL)
            return (-1);

        return (1);
    }

    return (0);
}

/**
** \fn const char *get_plugin_name()
** \brief Fonction permettant de d'acceder a la description du plugin
**
** \return Retourne un pointeur vers la description du plugin
*/
const char    *PLUGIN_FUNC_APROPOS()
{
    return ("Plugin permettant de deassembler les instructions Brainfuck.\n"
            "\n"
            "Type : " PLUGIN_DESCR_TYPE "\n"
            "Priorite : " PLUGIN_DESCR_PRIORITE "\n"
            "Nom du plugin : " PLUGIN_DESCR_NAME "\n"
            "Version : " PLUGIN_DESCR_VERSION "\n"
            "\n"
            "Infos necessaires :\n"
            " - Le champ \"archi\" doit valoir \"ARCHI_BRAINFUCK\".\n"
            "\n"
            "Effets :\n"
            " - Le champ endian vaut \"ENDIAN_LITTLE\".\n"
            " - Les pointeurs des fonctions permettant d'acceder aux emplacements des parametres\n"
            "     (\"ptr_func.f_*\") sont initialises.\n"
            " - Si le champ \"ptr_func.deasm\" pointait sur un deassembleur d'instructions,\n"
            "     ce dernier est supprime.\n"
            " - Le champ \"ptr_func.deasm\" pointe sur un deassembleur d'instructions Brainfuck.\n"
            "\n"
            "Valeurs de retour :\n"
            " - Retourne 1 si le deassembleur d'instructions Brainfuck est initialise.\n"
            " - Retourne 0 si le champ \"archi\" est different de \"ARCHI_BRAINFUCK\".\n"
            " - Retourne -1 en cas d'erreur.\n");
}

