/*
** Info.cpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 14:34:37 joran herve
** Last update Sat, 27 Jun 2015 14:34:37 joran herve
**
** Status : OK
*/

#include    "Info.hpp"
#include    "Plugin.hpp"


/**
** \fn Info()
** \brief Constructeur par defaut de la classe Info
*/
Info::Info():
    plugin(),
    option_plugin(),
    filename(""),
    data(NULL),
    size(0),
    os(Info::OS_NONE),
    archi(Info::ARCHI_NONE),
    format(Info::FORMAT_NONE),
    endian(Info::ENDIAN_NONE),
    entry(),
    sec(),
    sym(),
    function(),
    variablesFunc(),
    ptr_func(),
    syscall(),
    interrupt(),
    proto_func(),
    proto_var(),
    analyses(),
    mutex()
{
}

/**
** \fn ~Info()
** \brief Destructeur par defaut de la classe Info
*/
Info::~Info()
{
    this->clear();
}


/**
** \fn Info(const Info &i)
** \brief Constructeur par copie de la classe Info (Inutilisable)
*/
Info::Info(const Info &):
    plugin(),
    option_plugin(),
    filename(""),
    data(NULL),
    size(0),
    os(Info::OS_NONE),
    archi(Info::ARCHI_NONE),
    format(Info::FORMAT_NONE),
    endian(Info::ENDIAN_NONE),
    entry(),
    sec(),
    sym(),
    function(),
    variablesFunc(),
    ptr_func(),
    syscall(),
    interrupt(),
    proto_func(),
    proto_var(),
    analyses(),
    mutex()
{
}

/**
** \fn Info& operator = (const Info &)
** \brief Surcharge de l'operateur = pour la classe Info (Inutilisable)
*/
Info&    Info::operator = (const Info &)
{
    return (*this);
}

/**
** \fn void clear()
** \brief Gere la suppression des infos du fichier a analyser
**
** \return Retourne rien
*/
void    Info::clear()
{
    this->clear_analyse();
    this->clear_proto();
    this->clear_function();
    this->clear_segment();
    this->clear_file();
    this->clear_plugin();
}


/**
** \fn void clear_plugin()
** \brief Gere la suppression des plugins.
**
** \return Retourne rien
*/
void    Info::clear_plugin()
{
    this->mutex.lock();

    for (std::map<std::string, Plugin*>::iterator it=this->plugin.begin(); it!=this->plugin.end(); it++)
        delete it->second;
    this->plugin.clear();

    this->option_plugin.clear();

    this->mutex.unlock();
}

/**
** \fn void clear_file()
** \brief Gere la suppression du contenu du fichier
**
** \return Retourne rien
*/
void    Info::clear_file()
{
    this->mutex.lock();

    filename = "";

    if (this->data != NULL)
        delete[] data;
    this->data = NULL;
    this->size = 0;

    this->mutex.unlock();
}

/**
** \fn void clear_segment()
** \brief Gere la suppression des segments, symboles et points d'entrees
**
** \return Retourne rien
*/
void    Info::clear_segment()
{
    this->mutex.lock();

    this->entry.clear();
    this->sec.clear();
    this->sym.clear();

    this->ptr_func.clear();

    this->mutex.unlock();
}

/**
** \fn void clear_function()
** \brief Gere la suppression des fonctions decompilees
**
** \return Retourne rien
*/
void    Info::clear_function()
{
    this->mutex.lock();

    for (std::map<unsigned long, Fonction *>::iterator it=this->function.begin();
         it!=this->function.end();
         it++)
        delete it->second;
    this->function.clear();

    for (std::map<unsigned long, VariablesFunc*>::iterator it=this->variablesFunc.begin();
         it!=this->variablesFunc.end();
         it++)
        delete it->second;
    this->variablesFunc.clear();

    this->mutex.unlock();
}

/**
** \fn void clear_proto()
** \brief Gere la suppression des definitions, interruption, ...
**
** \return Retourne rien
*/
void    Info::clear_proto()
{
    this->mutex.lock();

    this->syscall.clear();
    this->interrupt.clear();
    this->proto_func.clear();
    this->proto_var.clear();

    this->mutex.unlock();
}

/**
** \fn void clear_analyse()
** \brief Gere la suppression des analyse ayant ete faites par les plugins
**
** \return Retourne rien
*/
void    Info::clear_analyse()
{
    this->mutex.lock();
    this->analyses.clear();
    this->mutex.unlock();
}

