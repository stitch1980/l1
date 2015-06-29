/*
** plugin_parse_format_brainfuck.cpp for plugin_parse_format_brainfuck
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
#define PLUGIN_DESCR_PRIORITE    "2000"
#define PLUGIN_DESCR_NAME        "plugin_parse_format_brainfuck"
#define PLUGIN_DESCR_VERSION     "0"
#define PLUGIN_NAME              PLUGIN_DESCR_TYPE "." PLUGIN_DESCR_PRIORITE "." PLUGIN_DESCR_NAME "." PLUGIN_DESCR_VERSION

#define OFFSET_CODE         0x10000   /* Offset a partir duquel placer le code */
#define SIZE_DATA           1000000   /* Taille des donnees */

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
** \brief Fonction permettant d'executer le plugin pour extraire les infos des fichiers Brainfuck
**
** \param info Classe contenant les infos du programme a analyser
** \return Retourne 1 si OK, 0 si le plugin n'a pas ete execute et -1 en cas d'erreur
*/
int    PLUGIN_FUNC_EXECUTE(Info *info)
{
    unsigned char    *buffer;
    unsigned char    size;

    if ((info == NULL) || (info->data == NULL))
        return (-1);

    if (info->format == Info::FORMAT_BRAINFUCK)
    {
        /* Nettoie la structure Info */
        info->clear_segment();
        info->clear_function();
        info->clear_proto();
        info->clear_analyse();

        info->endian = Info::ENDIAN_LITTLE;
        info->archi = Info::ARCHI_BRAINFUCK;

        info->entry.insert(OFFSET_CODE);
        info->sym.add_symbole(OFFSET_CODE, "main", Symbole::FUNC);

        /* Compte le nombre d'octets utiles */
        size = 0;
        for (unsigned long i=0; i<info->size; i++)
        {
            if ((info->data[i] == '+') || (info->data[i] == '-') ||
                (info->data[i] == '<') || (info->data[i] == '>') ||
                (info->data[i] == '[') || (info->data[i] == ']') ||
                (info->data[i] == '.') || (info->data[i] == ','))
                size++;
        }

        /* Preparation du segment de code */
        if ((buffer = new unsigned char[size]) != NULL)
        {
            /* Cree un segment contenant uniquement les octets utiles */
            memset(buffer, 0, size);
            for (unsigned long i=0, j=0; (i<info->size) && (j<size); i++)
            {
                if ((info->data[i] == '+') || (info->data[i] == '-') ||
                    (info->data[i] == '<') || (info->data[i] == '>') ||
                    (info->data[i] == '[') || (info->data[i] == ']') ||
                    (info->data[i] == '.') || (info->data[i] == ','))
                {
                    buffer[j] = info->data[i];
                    j++;
                }
            }
            info->sec.add_section(OFFSET_CODE, size, buffer, SECTION_R | SECTION_X);
        }

        /* Preparation du segment de donnees */
        if ((buffer = new unsigned char[SIZE_DATA]) != NULL)
        {
            memset(buffer, 0, SIZE_DATA);
            info->sec.add_section(OFFSET_CODE + size + 0x1000, SIZE_DATA, buffer, SECTION_R | SECTION_W);
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
    return ("Plugin parsant les fichiers Brainfuck\n"
            "\n"
            "Type : " PLUGIN_DESCR_TYPE "\n"
            "Priorite : " PLUGIN_DESCR_PRIORITE "\n"
            "Nom du plugin : " PLUGIN_DESCR_NAME "\n"
            "Version : " PLUGIN_DESCR_VERSION "\n"
            "\n"
            "Infos necessaires :\n"
            " - Le contenu du fichier a analyser doit etre dans le champ \"data\"\n"
            " - La taille du contenu du fichier a analyser doit etre dans le champ \"size\"\n"
            " - Le champ \"format\" doit valoir \"FORMAT_BRAINFUCK\"\n"
            "\n"
            "Effets :\n"
            " - Les segments, fonctions, prototypes et analyses sont videes\n"
            " - Les caracteres correspondants a des instructions Brainfuck valides sont\n"
            "     places dans un segment accessible en lecture/execution cree a l'adresse\n"
            "     0x10000\n"
            " - Un segment de 1Mo accessible en lecture/ecriture est cree 0x1000 octets apres\n"
            "     la fin de ce segment et est rempli avec des 0 pour les donnees\n"
            " - Un point d'entree est cree a l'adresse 0x10000\n"
            " - Un symbole \"main\" est cree avec 0x10000 pour valeur\n"
            " - Le champ \"format\" vaut \"FORMAT_BRAINFUCK\"\n"
            " - Le champ \"endian\" vaut \"ENDIAN_LITTLE\"\n"
            " - Le champ \"archi\" vaut \"ARCHI_BRAINFUCK\"\n"
            "\n"
            "Valeurs de retour :\n"
            " - Retourne 1 si le champ \"format\" vaut \"FORMAT_BRAINFUCK\" et que\n"
            "   le fichier a ete parse\n"
            " - Retourne 0 si le fichier n'a pas ete parse\n"
            " - Retourne -1 en cas d'erreur\n");
}

