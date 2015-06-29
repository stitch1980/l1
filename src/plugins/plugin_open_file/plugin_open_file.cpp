/*
** plugin_open_file.cpp for plugin_open_file
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 14:34:37 joran herve
** Last update Sat, 27 Jun 2015 14:34:37 joran herve
**
** Status : OK (documentation faite)
*/

#include    <string.h>
#include    <unistd.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <fcntl.h>

#include    "Info.hpp"


#define PLUGIN_DESCR_TYPE        PLUGIN_TYPE__OPEN
#define PLUGIN_DESCR_PRIORITE    "1000"
#define PLUGIN_DESCR_NAME        "plugin_open_file"
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
** \brief Fonction permettant d'executer le plugin pour recuperer le contenu du fichier
**
** \param info Classe contenant les infos du programme a analyser
** \return Retourne 1 si OK, -1 sinon
*/
int        PLUGIN_FUNC_EXECUTE(Info *info)
{
    unsigned long    size;
    unsigned long    tmp;
    int              f;

    if ((info == NULL) || (info->filename.size() <= 0))
        return (0);

    /* Si un buffer existe, on le libere */
    if (info->data != NULL)
        delete[] info->data;
    info->data = NULL;
    info->size = 0;

    /* Ouverture du fichier et recuperation de sa taille */
    if ((f = open(info->filename.c_str(), O_RDONLY)) > 0)
    {
        if ((size = lseek(f, 0, SEEK_END)) != static_cast<unsigned long>(-1))
        {
            if (lseek(f, 0, SEEK_SET) == 0)
            {
                if ((info->data = new unsigned char[size]) != NULL)
                {
                    /* Recuperation du contenu du fichier */
                    memset(info->data, 0, size);
                    while ((tmp = read(f, &(info->data[info->size]), size - info->size)) > 0)
                        info->size += tmp;

                    close(f);
                    return (1);
                }
            }
        }
        close(f);
    }

    return (-1);
}

/**
** \fn const char *get_plugin_name()
** \brief Fonction permettant de d'acceder a la description du plugin
**
** \return Retourne un pointeur vers la description du plugin
*/
const char    *PLUGIN_FUNC_APROPOS()
{
    return ("Plugin permettant de recuperer le contenu d'un fichier a analyser\n"
            "\n"
            "Type : " PLUGIN_DESCR_TYPE "\n"
            "Priorite : " PLUGIN_DESCR_PRIORITE "\n"
            "Nom du plugin : " PLUGIN_DESCR_NAME "\n"
            "Version : " PLUGIN_DESCR_VERSION "\n"
            "\n"
            "Infos necessaires :\n"
            " - Le nom du fichier doit etre precise dans le champ \"filename\"\n"
            "\n"
            "Effets :\n"
            " - Si un fichier etait deja charge, les champs \"data\" et\n"
            "   \"size\" sont remis a 0.\n"
            " - Le contenu du fichier est mis dans le champ \"data\"\n"
            " - La taille du fichier est mis dans le champ \"size\"\n"
            "\n"
            "Valeurs de retour :\n"
            " - Retourne 1 si le contenu du fichier a bien ete recupere\n"
            " - Retourne 0 si le nom du fichier n'est pas precise dans le champ \"filename\"\n"
            " - Retourne -1 si le fichier n'a pas pu etre ouvert\n");
}

