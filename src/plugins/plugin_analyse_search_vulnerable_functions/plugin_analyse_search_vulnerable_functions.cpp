/*
** plugin_analyse_search_vulnerable_functions.cpp for plugin_analyse_search_vulnerable_functions
**
** Made by joran herve
**
** Started on  Mon, 29 Jun 2015 20:02:43 joran herve
** Last update Mon, 29 Jun 2015 20:02:43 joran herve
**
** Status : OK (documentation faite)
*/

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>

#include    "Info.hpp"
#include    "Calcul.hpp"


#define PLUGIN_DESCR_TYPE        PLUGIN_TYPE__ANALYSE  
#define PLUGIN_DESCR_PRIORITE    "50000"      
#define PLUGIN_DESCR_NAME        "plugin_analyse_search_vulnerable_functions"
#define PLUGIN_DESCR_VERSION     "0"  
#define PLUGIN_NAME              PLUGIN_DESCR_TYPE "." PLUGIN_DESCR_PRIORITE "." PLUGIN_DESCR_NAME "." PLUGIN_DESCR_VERSION

#define TITRE_ANALYSE            "Vulnerable functions"

const char    *tab_name_func[] =
{
    "execle",
    "execvp",
    "execvpe",
    "getenv",
    "system",
    NULL
};

extern "C"
{
    const char    *PLUGIN_FUNC_GET_PLUGIN_NAME();
    int           PLUGIN_FUNC_EXECUTE(Info *info);
    const char    *PLUGIN_FUNC_APROPOS();
}


/**
** \fn unsigned long traite_fonction(Info *info, const Fonction *f,
**                                   Analyse *analyse_vuln, const char *vuln_name)
** \brief Parcours une fonction a la recherche d'appels a une fonction vulnerable
**
** \param info Structure contenant les infos du programme a analyser
** \param f Fonction a analyser
** \param analyse_vuln Analyse ou mettre le resultat de la recherche d'appels de fonction
** \paran vuln_name Nom de la fonction vulnerable a chercher
** \return Retourne le nombre d'appels a la fonction vulnerable
*/
unsigned long    traite_fonction(Info *info, const Fonction *f, Analyse *analyse_vuln, const char *vuln_name)
{
    InstrASM                 *instr;
    std::set<std::string>    set_addr_call;
    unsigned long            addr_call;
    unsigned long            nbs_call;

    nbs_call = 0;
    
    /* Pour chaque blocs de la fonction */
    for (std::list<Fonction::Bloc*>::const_iterator it_bloc=f->get_list_bloc().begin();
         it_bloc!=f->get_list_bloc().end();
         it_bloc++)
    {
        /* Pour chaque instructions du bloc */
        instr = (*it_bloc)->get_instr_entry();
        while (instr != NULL)
        {
            /* Si l'instruction est un call */
            if (((instr->_type & INSTR_CALL) == INSTR_CALL) &&
                (instr->_operande.size() > 0))
            {
                /* On estime les adresses qu'il est possible d'atteindre avec ce call */
                Calcul::evaluate(set_addr_call, instr->_operande[0]._name, info, instr->_contexte);
                for (std::set<std::string>::const_iterator it_addr=set_addr_call.begin();
                     it_addr!=set_addr_call.end();
                     it_addr++)
                {
                    /* Si l'adresse du call correspond a la fonction vulnerable recherchee */
                    addr_call = Calcul::Oxtol(it_addr->c_str());
                    if (info->sym.get_name(addr_call) == vuln_name)
                    {
                        analyse_vuln->add_analyse_child(Calcul::ltox(instr->_address) + ":", Fonction::get_name_function(f, info));
                        nbs_call++;
                    }
                }
            }

            instr = instr->_next_instr;
        }  
    }

    return (nbs_call);
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
** \brief Fonction permettant d'analyser les fonctions deassemblees pour trouver des appels a des fonctions vulnerables
**
** \param info Classe contenant les infos du programme a analyser
** \return Retourne 1 si OK, 0 si le plugin n'a pas ete execute et -1 en cas d'erreur
*/
int    PLUGIN_FUNC_EXECUTE(Info *info)
{
    Analyse          *analyse;
    Analyse          *analyse_vuln;
    unsigned long    nbs_appels_total;
    unsigned long    nbs_appels;
    
    if (info == NULL)
        return (-1);

    /* Initialisation du rapport d'analyse */
    info->analyses.add_analyse_child(TITRE_ANALYSE, "Liste des appels a des fonctions vulnerables (necessite de deassembler le binaire d'abord)");
    
    /* On a besoin de la fonction de deassemblage d'instructions ASM et des fonctions a analyser */
    if ((info->ptr_func.deasm == NULL) || (info->function.size() <= 0))
        return (0);

    nbs_appels_total = 0;
    if ((analyse = info->analyses.get_analyse_child(TITRE_ANALYSE)) != NULL)
    {
        /* Pour toutes les fonctions vulnerables */
        for (unsigned long i=0; tab_name_func[i]!=NULL; i++)
        {
            analyse->add_analyse_child(tab_name_func[i], "");
            if ((analyse_vuln = analyse->get_analyse_child(tab_name_func[i])) != NULL)
            {
                nbs_appels = 0;

                /* On parcourt les fonctions deassemblees une a une */
                for (std::map<unsigned long, Fonction *>::const_iterator it=info->function.begin();
                     it!=info->function.end();
                     it++)
                    nbs_appels += traite_fonction(info, it->second, analyse_vuln, tab_name_func[i]);
                    
                analyse_vuln->set_txt_analyse(Calcul::ltos(nbs_appels) + " appels");
                nbs_appels_total += nbs_appels;
            }
        }
    
        analyse->set_txt_analyse("Liste des appels a des fonctions vulnerables (" +
                                 Calcul::ltos(nbs_appels_total) + " appels)");
    }


    return (1);
}

/**
** \fn const char *get_plugin_name()
** \brief Fonction permettant de d'acceder a la description du plugin
**
** \return Retourne un pointeur vers la description du plugin
*/
const char    *PLUGIN_FUNC_APROPOS()
{
    return ("Plugin analysant les appels de fonction pour identifier des appels a des fonctions vulnerables.\n"
            "\n"
            "Type : " PLUGIN_DESCR_TYPE "\n"
            "Priorite : " PLUGIN_DESCR_PRIORITE "\n"
            "Nom du plugin : " PLUGIN_DESCR_NAME "\n"
            "Version : " PLUGIN_DESCR_VERSION "\n"
            "\n"
            "Fonctions recherchees :\n"
            "   execle, execvp, execvpe, getenv, system\n"
            "\n"
            "Infos necessaires :\n"
            " - Un gestionnaire de deassemblage doit avoir ete choisi dans le champ \"deasm\".\n"
            " - Le binaire doit avoir ete deassemble.\n"
            " - Les symboles correspondant aux adresses des fonctions vulnerables doivent\n"
            "   etre present.\n"
            "\n"
            "Effets :\n"
            " - Une analyse \"" TITRE_ANALYSE "\" est cree dans le champ \"analyses\".\n"
            " - Pour chaque fonction vulnerable a chercher, une sous-analyse ayant pour titre\n"
            "   le nom de la fonction sera cree dans l'analyse \"" TITRE_ANALYSE "\".\n"
            " - A chaque appel d'une fonction vulnerable, une sous-analyse ayant pour titre\n"
            "   l'adresse de ce CALL sera cree dans la sous-analyse correspondant a la fonction\n"
            "   recherchee.\n"
            "\n"
            "Valeurs de retour :\n"
            " - Retourne 1 si les appels de fonctions ont bien ete analyses.\n"
            " - Retourne 0 si le binaire n'a pas ete prealablement deassemble.\n"
            " - Retourne -1 en cas d'erreur.\n");
}

