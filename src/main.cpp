/*
** main.cpp for Lecter
**
** Made by joran herve
**
** Started on  Mon, 29 Jun 2015 14:35:22 joran herve
** Last update Mon, 29 Jun 2015 14:35:22 joran herve
**
** Status : OK (thread-safe)
*/

#include    <sys/types.h>
#include    <limits.h>
#include    <stdlib.h>
#include    <stdio.h>

#include    <iostream>
#include    <set>

#include    "Info.hpp"
#include    "Plugin.hpp"


/**
** \fn int execute_type_plugin(Info *info const std::string &type)
** \brief Gere l'execution de tout les plugins d'un meme type par ordre de priorite
**
** \param info Structure contenant les infos du programme a analyser
** \param type Type des plugins a executer
** \return Retourne -1 en cas d'erreur, 1 si au moins un des plugins a ete execute, 0 sinon
*/
int    execute_type_plugin(Info *info, const std::string &type)
{
    std::map<std::string, Plugin*>::iterator    it;
    int                                         ret;
    int                                         ok;

    ok = 0;

    /* Execution des plugins les uns apres les autres */
    for (it=info->plugin.begin(); it!=info->plugin.end(); it++)
    {
        if (it->second->get_type() == type)
        {
            if ((ret = it->second->execute(info)) > 0)
            {
                fprintf(stderr, "OK:\t%s\n", it->second->get_plugin_name());
                ok = 1;
            }
            else if (ret == 0)
                fprintf(stderr, "IGN:\t%s\n", it->second->get_plugin_name());
            else
            {
                fprintf(stderr, "KO:\t%s\n", it->second->get_plugin_name());
                return (-1);
            }
        }
    }

    return (ok);
}

/**
** \fn int main(int argc, const char **argv)
** \brief Main du decompilateur
**
** \param argc Nombre de parametres utilisateur
** \param argv Tableau contenant les parametres utilisateur
** \return Retourne 1 si OK, 0 sinon
*/
int    main(int argc, const char **argv)
{
    std::set<std::string>                                         lib_name;
    Info                                                          info;

    if (argc < 2)
    {
        fprintf(stderr, "Usage:\t%s <executable> [lib] [lib] ...\n", argv[0]);
        return (0);
    }
    info.filename = argv[1];

    /* Chargement des plugins */
    Plugin::get_lib_name(lib_name, argc, argv);
    Plugin::get_lib(info.plugin, lib_name);

    /* Execution des plugins les uns apres les autres */
/*    execute_type_plugin(&info, PLUGIN_TYPE__TEST); */
    execute_type_plugin(&info, PLUGIN_TYPE__OPEN);
    execute_type_plugin(&info, PLUGIN_TYPE__PARSING);
    execute_type_plugin(&info, PLUGIN_TYPE__DEASM);
    execute_type_plugin(&info, PLUGIN_TYPE__CLEAN);
    execute_type_plugin(&info, PLUGIN_TYPE__DECOMPILE);

    return (1);
}

