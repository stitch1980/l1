/*
** plugin_analyse_invalide_instruction.cpp for plugin_analyse_invalide_instruction
**
** Made by joran herve
**
** Started on  Mon, 29 Jun 2015 15:44:41 joran herve
** Last update Mon, 29 Jun 2015 15:44:41 joran herve
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
#define PLUGIN_DESCR_NAME        "plugin_analyse_invalide_instruction"
#define PLUGIN_DESCR_VERSION     "0"
#define PLUGIN_NAME              PLUGIN_DESCR_TYPE "." PLUGIN_DESCR_PRIORITE "." PLUGIN_DESCR_NAME "." PLUGIN_DESCR_VERSION

#define TITRE_ANALYSE            "Unknown ASM instructions"


extern "C"
{
    const char    *PLUGIN_FUNC_GET_PLUGIN_NAME();
    int           PLUGIN_FUNC_EXECUTE(Info *info);
    const char    *PLUGIN_FUNC_APROPOS();
}


/**
** \fn unsigned long traite_fonction(const Info *info, const Fonction *f, Analyse *analyse)
** \brief Parcours les instructions ASM d'une fonction a la recherche des instructions invalides
**
** \param info Structure contenant les infos du programme a analyser
** \param f Fonction a analyser
** \param analyse Analyse ou mettre la description des instructions invalide
** \return Retourne le nombre d'instructions invalides trouvees dans la fonction
*/
unsigned long    traite_fonction(const Info *info, const Fonction *f, Analyse *analyse)
{
    const InstrASM    *instr;
    std::string       descr_instr;
    char              buffer[10];
    unsigned long     nbs;

    nbs = 0;

    /* On parcours tout les blocs de la fonction */
    for (std::list<Fonction::Bloc*>::const_iterator it=f->get_list_bloc().begin();
         it!=f->get_list_bloc().end();
         it++)
    {
        /* On parcours toutes les instructions du bloc */
        instr = (*it)->get_instr_entry();
        while (instr != NULL)
        {
            /* Si l'instruction est invalide, on l'ajoute a l'analyse */
            if ((instr->_type & INSTR_INVALIDE) == INSTR_INVALIDE)
            {
                nbs++;
                instr->to_asm(info, descr_instr);
                descr_instr += "\t\t(";

                /* Ajout des octets des l'instruction a la description */
                for (unsigned long i=0; i<instr->_size; i++)
                {
                    if (i < (instr->_size - 1))
                        sprintf(buffer, "%02x ", info->sec.get_char(instr->_address + i));
                   else
                        sprintf(buffer, "%02x", info->sec.get_char(instr->_address + i));

                   descr_instr += buffer;
                }
                descr_instr += ")";

                analyse->add_analyse_child(Calcul::lto0x(instr->_address), descr_instr);
            }

            instr = instr->_next_instr;
        }
    }

    return (nbs);
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
** \brief Fonction permettant d'analyser les fonctions deassemblees pour trouver les instruction ASM invalides
**
** \param info Classe contenant les infos du programme a analyser
** \return Retourne 1 si OK, 0 si le plugin n'a pas ete execute et -1 en cas d'erreur
*/
int    PLUGIN_FUNC_EXECUTE(Info *info)
{
    Analyse          *analyse;
    unsigned long    nbs_unknown_instr;

    if (info == NULL)
        return (-1);

    /* Initialisation du rapport d'analyse */
    info->analyses.add_analyse_child(TITRE_ANALYSE, "Liste des instructions ASM inconnues presentes dans le binaire (necessite de deassembler le binaire)");

    /* On a besoin des fonctions a analyser */
    if (info->function.size() <= 0)
        return (0);

    if ((analyse = info->analyses.get_analyse_child(TITRE_ANALYSE)) != NULL)
    {
        nbs_unknown_instr = 0;

        /* On parcourt les fonctions deassemblees une a une */
        for (std::map<unsigned long, Fonction *>::const_iterator it=info->function.begin();
             it!=info->function.end();
             it++)
            nbs_unknown_instr += traite_fonction(info, it->second, analyse);

        analyse->set_txt_analyse("Liste des instructions ASM inconnues presentes dans le binaire (" + Calcul::ltos(nbs_unknown_instr) + " instructions inconnues)");
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
    return ("Plugin analysant les fonctions deassemblees pour trouver les instructions ASM inconnues.\n"
            "\n"
            "Type : " PLUGIN_DESCR_TYPE "\n"
            "Priorite : " PLUGIN_DESCR_PRIORITE "\n"
            "Nom du plugin : " PLUGIN_DESCR_NAME "\n"
            "Version : " PLUGIN_DESCR_VERSION "\n"
            "\n"
            "Infos necessaires :\n"
            " - Le binaire doit avoir ete deassemble.\n"
            "\n"
            "Effets :\n"
            " - Une analyse \"" TITRE_ANALYSE "\" est cree dans le champ \"analyses\".\n"
            " - Pour chaque instruction ayant un type TYPE_INVALIDE, une sous-analyse ayant\n"
            "   pour titre l'adresse de l'unstruction est cree. Cette sous-analyse contient\n"
            "   l'instruction ASM si elle existe et les octets encodant l'instruction sous\n"
            "   forme hexadecimal.\n"
            "\n"
            "Valeurs de retour :\n"
            " - Retourne 1 si les fonctions ont bien ete analysees.\n"
            " - Retourne 0 si le binaire n'a pas ete prealablement deassemble.\n"
            " - Retourne -1 en cas d'erreur.\n");
}

