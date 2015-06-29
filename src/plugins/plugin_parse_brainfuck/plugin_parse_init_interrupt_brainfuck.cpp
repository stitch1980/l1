/*
** plugin_parse_init_interrupt_brainfuck.cpp for plugin_parse_init_interrupt_brainfuck
**
** Made by joran herve
**
** Started on  Sun, 28 Jun 2015 16:38:34 joran herve
** Last update Sun, 28 Jun 2015 16:38:34 joran herve
**
** Status : OK (documentation faite)
*/

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>

#include    "Info.hpp"


#define PLUGIN_DESCR_TYPE        PLUGIN_TYPE__PARSING
#define PLUGIN_DESCR_PRIORITE    "3000"
#define PLUGIN_DESCR_NAME        "plugin_parse_init_interrupt_brainfuck"
#define PLUGIN_DESCR_VERSION     "0"
#define PLUGIN_NAME              PLUGIN_DESCR_TYPE "." PLUGIN_DESCR_PRIORITE "." PLUGIN_DESCR_NAME "." PLUGIN_DESCR_VERSION


extern "C"
{
    const char    *PLUGIN_FUNC_GET_PLUGIN_NAME();
    int           PLUGIN_FUNC_EXECUTE(Info *info);
    const char    *PLUGIN_FUNC_APROPOS();
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
** \brief Fonction permettant d'initialiser la liste des interruptions pour architecture Brainfuck
**
** \param info Classe contenant les infos du programme a analyser
** \return Retourne 1 si OK, 0 si le plugin n'a pas ete execute et -1 en cas d'erreur
*/
int    PLUGIN_FUNC_EXECUTE(Info *info)
{
    if (info == NULL)
        return (-1);

    if (info->archi == Info::ARCHI_BRAINFUCK)
    {
        info->clear_proto();

        /* Initialisation des interruptions */
        info->interrupt.add_syscall('.', "putchar", "void", "char");
        info->interrupt.add_syscall(',', "getchar", "char");

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
    return ("Plugin permettant d'initialiser la liste des interruptions Brainfuck.\n"
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
            " - Les prototypes et definitions sont supprimees.\n"
            " - Les interruptions utilisables en Brainfuck sont ajoutees.\n"
            "\n"
            "Valeurs de retour :\n"
            " - Retourne 1 si le champ \"archi\" vaut \"ARCHI_BRAINFUCK\".\n"
            " - Retourne 0 si le champ \"archi\" est different de \"ARCHI_BRAINFUCK\".\n"
            " - Retourne -1 en cas d'erreur.\n");
}

