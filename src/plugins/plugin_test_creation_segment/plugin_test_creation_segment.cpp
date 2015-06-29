/*
** plugin_test_creation_segment.cpp for plugin_test_creation_segment
**
** Made by joran herve
**
** Started on  Sun, 28 Jun 2015 13:35:41 joran herve
** Last update Sun, 28 Jun 2015 13:35:41 joran herve
**
** Status : OK (documentation faite)
*/

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <unistd.h>
#include    <time.h>

#include    "Info.hpp"


#define PLUGIN_DESCR_TYPE        PLUGIN_TYPE__TEST
#define PLUGIN_DESCR_PRIORITE    "1"      
#define PLUGIN_DESCR_NAME        "plugin_test_creation_segment"
#define PLUGIN_DESCR_VERSION     "0"  
#define PLUGIN_NAME              PLUGIN_DESCR_TYPE "." PLUGIN_DESCR_PRIORITE "." PLUGIN_DESCR_NAME "." PLUGIN_DESCR_VERSION

#define DEF_NBS_NEW_DELETE    100000    /* Nombre de creation/suppression de segment */
#define DEF_NBS_GET_CHAR      100       /* Nombre de tentative d'acces a une adresse a chaque creation/suppression de segment */
#define DEF_PLAGE_ADRESSE     10000     /* Plage d'adresse possible pour les segments */
#define DEF_PLAGE_SIZE        1000      /* Taille maximum des segments */
#define DEF_MODULO_MESSAGE    10        /* Doit afficher un message tout les combien de creation/suppression de segment */


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
** \brief Fonction permettant d'executer le plugin pour preparer les infos manuellement
**
** \param info Classe contenant les infos du programme a analyser
** \return Retourne 1 si OK, 0 si le plugin n'a pas ete execute et -1 s'il faut arreter
*/
int    PLUGIN_FUNC_EXECUTE(Info *info)
{
    unsigned long    addr;
    unsigned long    size;
    unsigned char    *buffer;
    unsigned long    nbs_create;
    unsigned long    nbs_delete;

    if (info == NULL)
        return (-1);
    
    srand(time(NULL));
    nbs_create = 0;
    nbs_delete = 0;
   
    /* Boucle de creation/suppression de segments */
    for (unsigned long i=0; i<DEF_NBS_NEW_DELETE; i++)
    {
        /* Creation d'un segment */
        if ((rand() % 2) == 0)
        {
            nbs_create++;

            addr = rand() % DEF_PLAGE_ADRESSE;
            size = rand() % DEF_PLAGE_SIZE + 1;
            if ((buffer = new unsigned char[size]) != NULL)
            {
                info->sec.add_section(addr, size, buffer, 0);
            }
        }
        
        /* Suppression d'un segment */
        else
        {
            nbs_delete++;
            
            addr = rand() % DEF_PLAGE_ADRESSE;
            for (unsigned j=0; j<info->sec.get_nbs_section(); j++)
            {
                if (info->sec.get_addr_section(j) == addr)
                {
                    size = info->sec.get_size_section(j);
                    info->sec.del_section(addr, size, 0);
                    break ;
                }
            }
        }
        
        /* Message pour voir comment cela evolue */
        if ((i % DEF_MODULO_MESSAGE) == 0)
            printf("%lu:\tnbs seg: %lu\tnbs create: %lu\tnbs delete: %lu\n",
                   i, info->sec.get_nbs_section(), nbs_create, nbs_delete);
        
        /* On tente d'acceder à quelles adresses */
        for (unsigned long j=0; j<DEF_NBS_GET_CHAR; j++)
            info->sec.get_char(rand() % DEF_PLAGE_ADRESSE);
    }
    
    printf("test_creation_segment: il reste %lu segments\n", info->sec.get_nbs_section());
    info->sec.clear();
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
    return ("Plugin permettant de tester la fiabilite de la creation/suppression de segments\n"
            "\n"
            "Type : " PLUGIN_DESCR_TYPE "\n"
            "Priorite : " PLUGIN_DESCR_PRIORITE "\n"
            "Nom du plugin : " PLUGIN_DESCR_NAME "\n"
            "Version : " PLUGIN_DESCR_VERSION "\n"
            "\n"
            "Infos necessaires :\n"
            "\n"
            "Effets :\n"
            " - Le plugin cree ou supprime des segments de tailles aleatoires a des\n"
            "   adresses aleatoires\n"
            " - A chaque creation/suppression de segments, il tente d'acceder a des octets situes\n"
            "   a des adresses aleatoires dans les segments\n"
            " - Tout les 10 creations/suppressions de segments, un message affiche le numero du cycle,\n"
            "   le nombre de segments actuel, le nombre de creations de segment et le nombre de\n"
            "   tentatives de suppression de segment\n"
            " - Tout les segments sont supprimes a la fin du test\n"
            "\n"
            "Valeurs de retour :\n"
            " - Retourne 1 a la fin du test\n"
            " - Retourne -1 si aucune structure Info n'a ete passee en parametre\n");
}

