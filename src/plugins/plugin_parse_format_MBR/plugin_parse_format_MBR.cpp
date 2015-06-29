/*
** plugin_parse_format_MBR.cpp for plugin_parse_format_MBR
**
** Made by joran herve
**
** Started on  Sun, 28 Jun 2015 11:05:57 joran herve
** Last update Sun, 28 Jun 2015 11:05:57 joran herve
**
** Status : OK (documentation faite)
*/

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>

#include    "Info.hpp"


#define PLUGIN_DESCR_TYPE        PLUGIN_TYPE__PARSING
#define PLUGIN_DESCR_PRIORITE    "2500"      
#define PLUGIN_DESCR_NAME        "plugin_parse_format_MBR"
#define PLUGIN_DESCR_VERSION     "0"  
#define PLUGIN_NAME              PLUGIN_DESCR_TYPE "." PLUGIN_DESCR_PRIORITE "." PLUGIN_DESCR_NAME "." PLUGIN_DESCR_VERSION

#define OFFSET_CODE         0x7c00    /* Offset a partir duquel placer le code */
#define SIZE_DATA           1000000   /* Tailles des donnees apres le code */

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
** \brief Fonction permettant d'executer le plugin pour parser les images de secteurs de MBR
**
** \param info Classe contenant les infos du programme a analyser
** \return Retourne 1 si OK, 0 si le plugin n'a pas ete execute et -1 en cas d'erreur
*/
int    PLUGIN_FUNC_EXECUTE(Info *info)
{
    unsigned char    *buffer;

    if ((info == NULL) || (info->data == NULL))
        return (-1);

    /* Si le format de fichier n'est pas precisee et de que fichier correspond a un MBR */
    if (((info->format == Info::FORMAT_NONE) || (info->format == Info::FORMAT_MBR)) &&
        (info->size >= 512) && (info->data[510] == 0x55) && (info->data[511] == 0xAA))
    {
        /* Nettoie la structure Info */
        info->clear_segment();
        info->clear_function();
        info->clear_proto();
        info->clear_analyse();
        
        /* Par defaut, on considere qu'il s'agit d'une architecture Intel 16 bits */
        info->format = Info::FORMAT_MBR;
        info->endian = Info::ENDIAN_LITTLE;
        info->archi = Info::ARCHI_I16;
    
        /* On ajoute un point d'entree a l'adresse ou doit jumper le BIOS */
        info->entry.insert(OFFSET_CODE);
        info->sym.add_symbole(OFFSET_CODE, "_start", Symbole::FUNC);
    
        /* Preparation d'une zone memoire accessible en lecture et ecriture */
        if ((buffer = new unsigned char[SIZE_DATA + info->size]) != NULL)
        {
            memset(buffer, 0, SIZE_DATA + info->size);
            memcpy(&(buffer[OFFSET_CODE]), info->data, info->size);
            info->sec.add_section(0, SIZE_DATA + info->size, buffer, SECTION_R | SECTION_W);
        }
    
        /* Preparation du segment de MBR */
        if ((buffer = new unsigned char[512]) != NULL)
        {
            /* Cree un segment contenant uniquement les octets utiles */
            memcpy(buffer, info->data, 512);
            info->sec.add_section(OFFSET_CODE, 512, buffer, SECTION_R | SECTION_X | SECTION_W);
        }

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
    return ("Plugin parsant les images de secteurs de MBR\n"
            "\n"
            "Type : " PLUGIN_DESCR_TYPE "\n"
            "Priorite : " PLUGIN_DESCR_PRIORITE "\n"
            "Nom du plugin : " PLUGIN_DESCR_NAME "\n"
            "Version : " PLUGIN_DESCR_VERSION "\n"
            "\n"
            "Infos necessaires :\n"
            " - Le contenu du fichier a analyser doit etre dans le champ \"data\".\n"
            " - La taille du contenu du fichier a analyser doit etre dans le champ \"size\".\n"
            " - Le champ \"format\" doit valoir \"FORMAT_NONE\" ou \"FORMAT_MBR\".\n"
            "\n"
            "Effets :\n"
            " - Si le fichier semble etre un MBR et que le champ \"format\" vaut\n"
            "   \"FORMAT_NONE\" ou \"FORMAT_MBR\" :\n"
            "   - Les segments, fonctions, prototypes et analyses sont videes.\n"
            "   - Un segment de 1Mo accessible en lecture/ecriture est cree a l'adresse 0.\n"
            "     Il contient le contenu du fichier a partir de l'adresse 0x7c00 et est\n"
            "     complete avec des 0.\n"
            "   - Un segment de 512 octets accessible en lecture/execution est cree\n"
            "     a l'adresse 0x7c00. Il contient les 512 premiers octets du fichier.\n"
            "   - Un point d'entree est cree a l'adresse 0x7c00.\n"
            "   - Un symbole \"_start\" est cree avec 0x7c00 pour valeur.\n"
            "   - Le champ \"format\" vaut \"FORMAT_MBR\".\n"
            "   - Le champ \"endian\" vaut \"ENDIAN_LITTLE\".\n"
            "   - Le champ \"archi\" vaut \"ARCHI_I16\".\n"
            "\n"
            "Valeurs de retour :\n"
            " - Retourne 1 si le fichier est un MBR et qu'il a ete parse.\n"
            " - Retourne 0 si le fichier n'est pas un MBR ou qu'il n'a pas ete parse.\n"
            " - Retourne -1 en cas d'erreur.\n");
}

