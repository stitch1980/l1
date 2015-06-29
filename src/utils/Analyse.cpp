/*
** Analyse.cpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 14:34:37 joran herve
** Last update Sat, 27 Jun 2015 14:34:37 joran herve
**
** Status : OK (thread-safe)
*/

#include    "Analyse.hpp"


/**
** \fn Analyse()
** \brief Constructeur par defaut de l'analyse
*/
Analyse::Analyse():
    _analyse(),
    _analyse_child(),
    _mutex()
{
}

/**
** \fn Analyse(const std::string &s)
** \brief Constructeur d'analyse avec son contenu
**
** \param s Contenu de l'analyse
*/
Analyse::Analyse(const std::string &s):
    _analyse(s),
    _analyse_child(),
    _mutex()
{
}

 /**
** \fn ~Analyse()
** \brief Destructeur de l'analyse
*/
Analyse::~Analyse()
{
    this->clear();
}


/**
** \fn Analyse(const Analyse &a)
** \brief Constructeur par copie de l'analyse (inutilisable)
**
** \param a Analyse a copier
*/
Analyse::Analyse(const Analyse &):
    _analyse(),
    _analyse_child(),
    _mutex()
{
}
    
/**
** \fn Analyse &operator = (const Analyse &a)
** \brief Surcharge de l'operateur = pour les analyses (inutilisable)
**
** \param a Analyse a copier
** \return Retourne une reference sur l'analyse
*/
Analyse    &Analyse::operator = (const Analyse &)
{
    return (*this);
}


/**
** \fn void clear()
** \brief Gere la suppression du contenu de l'analyse
**
** \return Retourne rien
*/
void             Analyse::clear()
{
    this->_mutex.lock();

    this->_analyse.clear();
    
    for (std::map<std::string, Analyse*>::iterator it=this->_analyse_child.begin();
         it!=this->_analyse_child.end();
         it++)
        delete it->second;
    this->_analyse_child.clear();
    
    this->_mutex.unlock();
}
    
    
/**
** \fn void set_txt_analyse(const std::string &s)
** \brief Assesseur permettant de modifier le texte de l'analyse
**
** \param t Nouveau texte de l'analyse
** \return Retourne rien
*/
void             Analyse::set_txt_analyse(const std::string &s)
{
    this->_mutex.lock();
    this->_analyse = s;
    this->_mutex.unlock();
}
    
/**
** \fn std::string get_txt_analyse() const
** \brief Assesseur permettant de recuperer le texte de l'analyse
**
** \return Retourne le texte de l'analyse (pas celui des analyses filles)
*/
std::string      Analyse::get_txt_analyse() const
{
    std::string    s;
    
    const_cast<Mutex*>(&(this->_mutex))->lock();
    s = this->_analyse;
    const_cast<Mutex*>(&(this->_mutex))->unlock();

    return (s);
}
    
    
/**
** \fn void add_analyse_child(const std::string &title, const std::string &content)
** \brief Ajoute/remplace une analyse fille
**
** \param title Titre de l'analyse fille
** \param content Contenu de l'analyse fille
** \return Retourne rien
*/
void             Analyse::add_analyse_child(const std::string &title, const std::string &content)
{
    this->add_analyse_child(title, new Analyse(content));
}
    
/**
** \fn void add_analyse_child(const std::string &title, Analyse *a)
** \brief Ajoute/remplace une analyse fille (elle sera delete avec clear() et del_analyse_child(...))
**
** \param title Titre de l'analyse fille
** \param a Nouvelle analyse fille
** \return Retourne rien
*/
void             Analyse::add_analyse_child(const std::string &title, Analyse *a)
{
    this->del_analyse_child(title);

    const_cast<Mutex*>(&(this->_mutex))->lock();
    if ((a != NULL) && (a != this))
        this->_analyse_child[title] = a;
    const_cast<Mutex*>(&(this->_mutex))->unlock();
}
    
/**
** \fn void del_analyse_child(const std::string &title)
** \brief Gere la suppression d'une analyse fille si elle existe
**
** \param title Titre de l'analyse fille a supprimer
** \return Retourne rien
*/
void             Analyse::del_analyse_child(const std::string &title)
{
    std::map<std::string, Analyse*>::iterator    it;

    this->_mutex.lock();

    if ((it = this->_analyse_child.find(title)) != this->_analyse_child.end())
    {
        delete it->second;
        this->_analyse_child.erase(it);
    }

    this->_mutex.unlock();
}
    
    
/**
** \fn std::string get_txt_analyse_child(const std::string &title) const
** \brief Assesseur parmettent d'acceder au contenu d'une analyse fille grace a son titre
**
** \param title Titre de l'analyse fille
** \return Retourne le contenu de l'analyse fille si OK, "" sinon
*/
std::string      Analyse::get_txt_analyse_child(const std::string &title) const
{
    std::map<std::string, Analyse*>::const_iterator    it;
    std::string                                        s;

    const_cast<Mutex*>(&(this->_mutex))->lock();
    
    if ((it = this->_analyse_child.find(title)) != this->_analyse_child.end())
        s = it->second->get_txt_analyse();
        
    const_cast<Mutex*>(&(this->_mutex))->unlock();
    return (s);
}
    
/**
** \fn Analyse *get_analyse_child(const std::string &title) const
** \brief Assesseur parmettent d'acceder a une analyse fille grace a son titre
**
** \param title Titre de l'analyse fille
** \return Retourne un pointeur sur l'analyse fille si OK, NULL sinon
*/
Analyse          *Analyse::get_analyse_child(const std::string &title) const
{
    std::map<std::string, Analyse*>::const_iterator    it;
    Analyse                                            *ptr;

    const_cast<Mutex*>(&(this->_mutex))->lock();
    
    ptr = NULL;
    if ((it = this->_analyse_child.find(title)) != this->_analyse_child.end())
        ptr = it->second;
        
    const_cast<Mutex*>(&(this->_mutex))->unlock();
    return (ptr);
}
    
    
/**
** \fn unsigned long get_nbs_analyse_child() const
** \brief Assesseur permettant de connaitre le nombre d'analyses filles
**
** \return Retourne le nombre d'analyse fille
*/
unsigned long    Analyse::get_nbs_analyse_child() const
{
    unsigned long    nbs;
    
    const_cast<Mutex*>(&(this->_mutex))->lock();
    nbs = this->_analyse_child.size();
    const_cast<Mutex*>(&(this->_mutex))->unlock();

    return (nbs);
}

/**
** \fn std::string get_title_analyse_child_num(unsigned long n) const
** \brief Assesseur parmettant d'acceder au titre de la n-ieme analyse fille
**
** \param n Numero de l'analyse fille
** \return Retourne le titre de l'analyse fille si OK, "" sinon
*/
std::string      Analyse::get_title_analyse_child_num(unsigned long n) const
{
    std::map<std::string, Analyse*>::const_iterator    it;
    std::string                                        ret;
    
    const_cast<Mutex*>(&(this->_mutex))->lock();
    
    if (n < this->_analyse_child.size())
    {
        it = this->_analyse_child.begin();
        std::advance(it, n);
        ret = it->first;
    }
    
    const_cast<Mutex*>(&(this->_mutex))->unlock();

    return (ret);
}
    
/**
** \fn std::string get_txt_analyse_child_num(unsigned long n) const
** \brief Assesseur parmettant d'acceder au contenu de la n-ieme analyse fille
**
** \param n Numero de l'analyse fille
** \return Retourne le contenu de l'analyse fille si OK, "" sinon
*/
std::string      Analyse::get_txt_analyse_child_num(unsigned long n) const
{
    std::map<std::string, Analyse*>::const_iterator    it;
    std::string                                        ret;
    
    const_cast<Mutex*>(&(this->_mutex))->lock();
    
    if (n < this->_analyse_child.size())
    {
        it = this->_analyse_child.begin();
        std::advance(it, n);
        ret = it->second->get_txt_analyse();
    }
    
    const_cast<Mutex*>(&(this->_mutex))->unlock();

    return (ret);
}
    
/**
** \fn Analyse *get_analyse_child_num(unsigned long n) const
** \brief Assesseur parmettant d'acceder a la n-ieme analyse fille
**
** \param n Numero de l'analyse fille
** \return Retourne un pointeur sur l'analyse fille si OK, NULL sinon
*/
Analyse          *Analyse::get_analyse_child_num(unsigned long n) const
{
    std::map<std::string, Analyse*>::const_iterator    it;
    Analyse                                           *ret;
    
    const_cast<Mutex*>(&(this->_mutex))->lock();
    
    ret = NULL;
    if (n < this->_analyse_child.size())
    {
        it = this->_analyse_child.begin();
        std::advance(it, n);
        ret = it->second;
    }
    
    const_cast<Mutex*>(&(this->_mutex))->unlock();

    return (ret);
}

