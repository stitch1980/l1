/*
** Plugin.cpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 21:28:33 joran herve
** Last update Sat, 27 Jun 2015 21:28:33 joran herve
**
** Status : OK
*/

#include    "Plugin.hpp"
#include    "Info.hpp"


/**
** \fn void get_lib_name(std::set<std::string> &lib_name, int argc, const char **argv)
** \brief Gere la recuperation des noms des fichiers present dans le dossier des plugins
**
** \param lib_name Liste devant contenir les noms des plugins
** \param argc Nombre de plugins suplementaire
** \param argv Tableau contenant les noms des plugins suplementaires
** \return Retourne rien
*/
void    Plugin::get_lib_name(std::set<std::string> &lib_name, int argc, const char **argv)
{
    std::string      path;
    char             buffer[PATH_MAX + 1];
    DIR              *dir;
    struct dirent    *f;

    /* Identifie le chemin du dossier des plugins */
    if (realpath(argv[0], buffer) == NULL)
        fprintf(stderr, "ERROR:\tCannot find plugins directory\n");
    else
    {
        path = std::string(dirname(buffer)) + NAME_PLUGINS_DIRECTORY;

        /* Recupere les noms des fichiers present dans le dossier des plugins */
        if ((dir = opendir(path.c_str())) != NULL)
        {
            while ((f = readdir(dir)) != NULL)
            {
                if ((f->d_type & DT_REG) == DT_REG)
                {
                    if (realpath(std::string(path + f->d_name).c_str(), buffer) != NULL)
                        lib_name.insert(buffer);
                }
            }
            closedir(dir);
        }
        else
            fprintf(stderr, "ERROR:\tCannot open \"%s\"\n", path.c_str());
    }

    /* Recupere les noms de libs passes en parametres */
    for (int i=2; i<argc; i++)
    {
        if (realpath(argv[i], buffer) != NULL)
            lib_name.insert(buffer);
    }
}

/**
** \fn void get_lib(std::map<std::string, Plugin*> &lib, std::set<std::string> &lib_name)
** \brief Gere l'ouverture de tout les plugins de la liste de noms
**
** \param lib Map contenant les plugins indexes par leur nom
** \param lib_name List contenant les noms des plugins a ouvrir
** \return Retourne rien
*/
void    Plugin::get_lib(std::map<std::string, Plugin*> &lib, std::set<std::string> &lib_name)
{
    Plugin    *plugin;
    char      buffer[30];

    for (std::set<std::string>::iterator it=lib_name.begin(); it!=lib_name.end(); it++)
    {
        if ((plugin = Plugin::open_plugin(*it)) != NULL)
        {
            snprintf(buffer, 29, "%08lu", plugin->get_priorite());
            lib[std::string(buffer) + "-" + plugin->get_plugin_name()] = plugin;
        }
    }
}

/**
** \fn Plugin *open_plugin(const std::string &name)
** \brief Fonction gerant le chargement d'un plugin (un .so) a partir de son nom
**
** \param name Chemin et nom du plugin a charger
** \return Retourne un pointeur sur le plugin cree si OK, NULL sinon
*/
Plugin    *Plugin::open_plugin(const std::string &name)
{
    void          *lib;
    const char    *(*get_plugin_name)();
    int           (*execute)(Info *);
    const char    *(*apropos)();

    if ((lib = dlopen(name.c_str(), RTLD_LAZY)) != NULL)
    {
        if ((get_plugin_name = reinterpret_cast<const char*(*)()>(dlsym(lib, PLUGIN_FUNC_NAME_GET_PLUGIN_NAME))) != NULL)
        {
            if ((execute = reinterpret_cast<int(*)(Info*)>(dlsym(lib, PLUGIN_FUNC_NAME_EXECUTE))) != NULL)
            {
                if ((apropos = reinterpret_cast<const char*(*)()>(dlsym(lib, PLUGIN_FUNC_NAME_APROPOS))) != NULL)
                    return (new Plugin(get_plugin_name, execute, apropos, lib));
                else
                    fprintf(stderr, "ERROR:\tCannot found \"%s\" in \"%s\"\n",
                        PLUGIN_FUNC_NAME_APROPOS, name.c_str());
            }
            else
                fprintf(stderr, "ERROR:\tCannot found \"%s\" in \"%s\"\n",
                    PLUGIN_FUNC_NAME_EXECUTE, name.c_str());
        }
        else
            fprintf(stderr, "ERROR:\tCannot found \"%s\" in \"%s\"\n",
                PLUGIN_FUNC_NAME_GET_PLUGIN_NAME, name.c_str());
        dlclose(lib);
    }
    else
        fprintf(stderr, "ERROR:\tCannot open \"%s\" (%s)\n", name.c_str(), dlerror());

    return (NULL);
}



/**
** \fn Plugin(const char *(*get_plugin_name)(),
**            int (*execute)(Info *),
**            const char *(*apropos)(),
**            void *lib)
** \brief Constructeur du plugin
*/
Plugin::Plugin(const char *(*get_plugin_name)(),
           int (*execute)(Info *),
           const char *(*apropos)(),
           void *lib):
    _get_plugin_name(get_plugin_name),
    _execute(execute),
    _apropos(apropos),
    _lib(lib)
{
}

/**
** \fn ~Plugin()
** \brief Destructeur du plugin
*/
Plugin::~Plugin()
{
    if (this->_lib != NULL)
        dlclose(this->_lib);
}


/**
** \fn Plugin()
** \brief Constructeur par defaut d'un plugin (Inutilisable)
*/
Plugin::Plugin():
    _get_plugin_name(NULL),
    _execute(NULL),
    _apropos(NULL),
    _lib(NULL)
{
}

/**
** \fn Plugin(const Plugin&)
** \brief Constructeur par copie d'un plugin (Inutilisable)
*/
Plugin::Plugin(const Plugin&):
    _get_plugin_name(NULL),
    _execute(NULL),
    _apropos(NULL),
    _lib(NULL)
{
}

/**
** \fn Plugin &operator = (const Plugin&)
** \brief Surcharge de l'operateur = pour les plugins (Inutilisable)
**
** \return Retourne une reference sur le plugin
*/
Plugin    &Plugin::operator = (const Plugin&)
{
    return (*this);
}


/**
** \fn const char *get_plugin_name() const
** \brief Surcouche de la fonction permettant de connaitre le nom du plugin
**
** \return Retourne le nom du plugin si OK, NULL sinon
*/
const char    *Plugin::get_plugin_name() const
{
    if (this->_get_plugin_name == NULL)
        return (NULL);
    return ((*this->_get_plugin_name)());
}

/**
** \fn int execute(Info *info) const
** \brief Surcouche de la fonction permettant d'executer le plugin
**
** \param info Srtucture contenant les infos de l'executable a analyser
** \return Retourne 1 si OK, 0 si le plugin n'a pas ete execute et -1 en cas d'erreur
*/
int        Plugin::execute(Info *info) const
{
    if (this->_execute == NULL)
        return (0);
    return ((*this->_execute)(info));
}

/**
** \fn const char *apropos() const
** \brief Surcouche de la fonction permettant de connaitre la description du plugin
**
** \return Retourne la description du plugin si OK, NULL sinon
*/
const char    *Plugin::apropos() const
{
    if (this->_apropos == NULL)
        return (NULL);
    return ((*this->_apropos)());
}


/**
** \fn std::string get_type() const
** \brief Assesseur permettant de connaitre le type du plugin
**
** \return Retourne le type du plugin si OK, "" sinon
*/
std::string    Plugin::get_type() const
{
    const char       *ptr;
    std::string      s;
    unsigned long    pos;

    if ((ptr = this->get_plugin_name()) == NULL)
        return ("");

    s = ptr;
    if ((pos = s.find(".")) != std::string::npos)
        return (s.substr(0, pos));
    return (s);
}

/**
** \fn unsigned long get_priorite() const
** \brief Assesseur permettant de connaitre le niveau de priorite du plugin
**
** \return Retourne le niveau de priorite du plugin si OK, 0 sinon
*/
unsigned long    Plugin::get_priorite() const
{
    const char       *ptr;
    std::string      s;
    unsigned long    pos;
    unsigned long    pos2;

    if ((ptr = this->get_plugin_name()) == NULL)
        return (0);

    s = ptr;
    if ((pos = s.find(".")) != std::string::npos)
    {
        if ((pos2 = s.find(".", pos+1)) != std::string::npos)
            return (atol(s.substr(pos+1, pos2-pos).c_str()));
    }
    return (0);
}

/**
** \fn std::string get_name() const
** \brief Assesseur permettant de connaitre le nom du plugin
**
** \return Retourne le nom du plugin si OK, "" sinon
*/
std::string    Plugin::get_name() const
{
    const char       *ptr;
    std::string      s;
    unsigned long    pos;
    unsigned long    pos2;
    unsigned long    pos3;

    if ((ptr = this->get_plugin_name()) == NULL)
        return (0);

    s = ptr;
    if ((pos = s.find(".")) != std::string::npos)
    {
        if ((pos2 = s.find(".", pos+1)) != std::string::npos)
        {
            if ((pos3 = s.find(".", pos2+1)) != std::string::npos)
                return (s.substr(pos2+1, pos3-pos2-1).c_str());
        }
    }
    return (0);
}

/**
** \fn std::string get_version() const
** \brief Assesseur permettant de connaitre la version du plugin
**
** \return Retourne la version du plugin si OK, "" sinon
*/
std::string    Plugin::get_version() const
{
    const char       *ptr;
    std::string      s;
    unsigned long    pos;
    unsigned long    pos2;
    unsigned long    pos3;

    if ((ptr = this->get_plugin_name()) == NULL)
        return (0);

    s = ptr;
    if ((pos = s.find(".")) != std::string::npos)
    {
        if ((pos2 = s.find(".", pos + 1)) != std::string::npos)
        {
            if ((pos3 = s.find(".", pos2+1)) != std::string::npos)
                return (s.substr(pos3+1, -1).c_str());
        }
    }
    return (0);
}

