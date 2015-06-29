/*
** Plugin.hpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 21:28:33 joran herve
** Last update Sat, 27 Jun 2015 21:28:33 joran herve
**
** Status : OK
*/

#ifndef        PLUGIN_HPP_
#define        PLUGIN_HPP_

#include    <map>
#include    <set>

#include    <string>
#include    <cstdio>
#include    <dlfcn.h>
#include    <libgen.h>
#include    <dirent.h>

class    Info;


/** Nom du sous-dossier ou sont stockes les plugins standards */
#define    NAME_PLUGINS_DIRECTORY    "/plugins/"

/*
** Nom des fonctions qu'un plugin doit avoir pour fonctionner
*/
#define        PLUGIN_FUNC_GET_PLUGIN_NAME         get_plugin_name
#define        PLUGIN_FUNC_EXECUTE                 execute
#define        PLUGIN_FUNC_APROPOS                 apropos
#define        PLUGIN_FUNC_NAME_GET_PLUGIN_NAME    "get_plugin_name"
#define        PLUGIN_FUNC_NAME_EXECUTE            "execute"
#define        PLUGIN_FUNC_NAME_APROPOS            "apropos"

/*
** Nom des differents type de plugin
*/
#define        PLUGIN_TYPE__TEST             "TEST"            /* Plugins de test */
#define        PLUGIN_TYPE__OPEN             "OPEN"            /* Chargement du fichier a analyser */
#define        PLUGIN_TYPE__PARSING          "PARSING"         /* Extraction des infos du fichier a analyser */
#define        PLUGIN_TYPE__DEASM            "DEASM"           /* Deassemblage */
#define        PLUGIN_TYPE__CLEAN            "CLEAN"           /* Nettoyage/deobfuscation */
#define        PLUGIN_TYPE__DECOMPILE        "DECOMPILE"       /* Conversion ASM/C */
#define        PLUGIN_TYPE__ANALYSE          "ANALYSE"         /* Analyse des donnes produites par les autres plugins */
#define        PLUGIN_TYPE__OUTPUT           "OUTPUT"          /* Plugins generant une sortie automatiquement (log, pdf, ...) */

/**
** \class Plugin
** \brief Classe parmettant de gerer les plugins de maniere abstraite
*/
class Plugin
{
public:
    /**
    ** \fn void get_lib_name(std::set<std::string> &lib_name, int argc, const char **argv)
    ** \brief Gere la recuperation des noms des fichiers present dans le dossier des plugins
    **
    ** \param lib_name Liste devant contenir les noms des plugins
    ** \param argc Nombre de plugins suplementaire
    ** \param argv Tableau contenant les noms des plugins suplementaires
    ** \return Retourne rien
    */
    static void    get_lib_name(std::set<std::string> &lib_name, int argc, const char **argv);

    /**
    ** \fn void get_lib(std::map<std::string, Plugin*> &lib, std::set<std::string> &lib_name)
    ** \brief Gere l'ouverture de tout les plugins de la liste de noms
    **
    ** \param lib Map contenant les plugins indexes par leur nom
    ** \param lib_name List contenant les noms des plugins a ouvrir
    ** \return Retourne rien
    */
    static void    get_lib(std::map<std::string, Plugin*> &lib, std::set<std::string> &lib_name);

    /**
    ** \fn Plugin *open_plugin(const std::string &name)
    ** \brief Fonction gerant le chargement d'un plugin (un .so) a partir de son nom
    **
    ** \param name Chemin et nom du plugin a charger
    ** \return Retourne un pointeur sur le plugin cree si OK, NULL sinon
    */
    static Plugin    *open_plugin(const std::string &name);

public:
    /**
    ** \fn Plugin(const char *(*get_plugin_name)(),
    **            int (*execute)(Info *),
    **            const char *(*apropos)(),
    **            void *lib)
    ** \brief Constructeur du plugin
    */
    Plugin(const char *(*get_plugin_name)(),
           int (*execute)(Info *),
           const char *(*apropos)(),
           void *lib);

    /**
    ** \fn ~Plugin()
    ** \brief Destructeur du plugin
    */
    ~Plugin();

private:
    /**
    ** \fn Plugin()
    ** \brief Constructeur par defaut d'un plugin (Inutilisable)
    */
    Plugin();

    /**
    ** \fn Plugin(const Plugin&)
    ** \brief Constructeur par copie d'un plugin (Inutilisable)
    */
    Plugin(const Plugin&);


    /**
    ** \fn Plugin &operator = (const Plugin&)
    ** \brief Surcharge de l'operateur = pour les plugins (Inutilisable)
    **
    ** \return Retourne une reference sur le plugin
    */
    Plugin    &operator = (const Plugin&);

public:
    /**
    ** \fn const char *get_plugin_name() const
    ** \brief Surcouche de la fonction permettant de connaitre le nom du plugin
    **
    ** \return Retourne le nom du plugin si OK, NULL sinon
    */
    const char       *get_plugin_name() const;

    /**
    ** \fn int execute(Info *info) const
    ** \brief Surcouche de la fonction permettant d'executer le plugin
    **
    ** \param info Srtucture contenant les infos de l'executable a analyser
    ** \return Retourne 1 si OK, 0 si le plugin n'a pas ete execute et -1 en cas d'erreur
    */
    int              execute(Info *info) const;

    /**
    ** \fn const char *apropos() const
    ** \brief Surcouche de la fonction permettant de connaitre la description du plugin
    **
    ** \return Retourne la description du plugin si OK, NULL sinon
    */
    const char       *apropos() const;


    /**
    ** \fn std::string get_type() const
    ** \brief Assesseur permettant de connaitre le type du plugin
    **
    ** \return Retourne le type du plugin si OK, "" sinon
    */
    std::string      get_type() const;

    /**
    ** \fn unsigned long get_priorite() const
    ** \brief Assesseur permettant de connaitre le niveau de priorite du plugin
    **
    ** \return Retourne le niveau de priorite du plugin si OK, 0 sinon
    */
    unsigned long    get_priorite() const;

    /**
    ** \fn std::string get_name() const
    ** \brief Assesseur permettant de connaitre le nom du plugin
    **
    ** \return Retourne le nom du plugin si OK, "" sinon
    */
    std::string      get_name() const;

    /**
    ** \fn std::string get_version() const
    ** \brief Assesseur permettant de connaitre la version du plugin
    **
    ** \return Retourne la version du plugin si OK, "" sinon
    */
    std::string      get_version() const;

protected:
    /** Pointeur sur la fonction permettant de connaitre le nom du plugin */
    const char    *(*_get_plugin_name)();
    /** Pointeur sur la fonction permettant d'executer le plugin */
    int           (*_execute)(Info *);
    /** Pointeur sur la fonction permettant recuper la description du plugin */
    const char    *(*_apropos)();

    /** Pointeur sur la lib contenant le plugin */
    void          *_lib;
};

#endif

