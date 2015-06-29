/*
** Symbole.cpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 15:28:10 joran herve
** Last update Sat, 27 Jun 2015 15:28:10 joran herve
**
** Status : OK (thread-safe)
*/

#include    "Symbole.hpp"


/**
** \class SymboleSym
** \brief Classe decrivant un symbole
*/
class    SymboleSym
{
public:
    /**
    ** \fn SymboleSym()
    ** \brief Constructeur par defaut du symbole
    */
    SymboleSym(): _name(""), _type(Symbole::NONE)
    {
    };

    /**
    ** \fn SymboleSym(const SymboleSym &s)
    ** \brief Constructeur par copie du symbole
    **
    ** \param s Symbole a copier
    */
    SymboleSym(const SymboleSym &s): _name(s._name), _type(s._type)
    {
    };

    /**
    ** \fn ~SymboleSym()
    ** \brief Destructeur du symbole
    */
    ~SymboleSym()
    {
    };
    
    /**
    ** \fn SymboleSym &operator = (const SymboleSym &s)
    ** \brief Surcharge de l'operateur = pour les symboles
    **
    ** \param s Symbole a copier
    ** \return Retourne une reference sur le symbole
    */
    SymboleSym    &operator = (const SymboleSym &s)
    {
        this->_name = s._name;
        this->_type = s._type;
        return (*this);
    };

public:
    /** Nom du symbole */
    std::string       _name;
    /** Type de symbole */
    Symbole::eType    _type;
};


/**
** \fn Symbole()
** \brief Constructeur du gestionnaire de symboles
*/
Symbole::Symbole():
    _sym(),
    _mutex()
{
}

/**
** \fn ~Symbole()
** \brief Destructeur du gestionnaire de symboles
*/
Symbole::~Symbole()
{
    this->clear();
}


/**
** \fn void clear()
** \brief Gere le vidage du gestionnaire de symboles
**
** \return Retourne rien
*/
void    Symbole::clear()
{
    std::map<unsigned long, SymboleSym *>::iterator    it;

    this->_mutex.lock();

    for (it=this->_sym.begin(); it!=this->_sym.end(); it++)
        delete it->second;
    this->_sym.clear();

    this->_mutex.unlock();
}

/**
** \fn void add_symbole(unsigned long addr, const std::string &name, Symbole::eType type)
** \brief Gere l'ajout ou le remplacement d'un symbole dans le gestionnaire
**
** \param addr Adresse du symbole
** \param name Nom du symbole
** \param type Type de symbole
** \return Retourne rien
*/
void    Symbole::add_symbole(unsigned long addr, const std::string &name, Symbole::eType type)
{
    SymboleSym    *ptr;
    
    this->del_symbole(addr);
    this->_mutex.lock();

    if (name.size() > 0)
    {
        if ((ptr = new SymboleSym) != NULL)
        {
            ptr->_name = name;
            ptr->_type = type;
            this->_sym[addr] = ptr;
        }
    }

    this->_mutex.unlock();
}

/**
** \fn void del_symbole(unsigned long addr)
** \brief Gere la suppression d'un symbole a partir de son adresse
**
** \param addr Adresse du symbole a supprimer
** \return Retourne rien
*/
void    Symbole::del_symbole(unsigned long addr)
{
    std::map<unsigned long, SymboleSym *>::iterator    it;
    
    this->_mutex.lock();

    if ((it = this->_sym.find(addr)) != this->_sym.end())
    {
        delete it->second;
        this->_sym.erase(it);
    }
    
    this->_mutex.unlock();
}


/**
** \fn unsigned long get_addr(const std::string &name) const
** \brief Gere la recuperation de l'adresse d'un symbole a partir de son nom
**
** \param name Nom du symbole a trouver
** \return Retourne l'adresse d'un symbole si on le trouve, 0 sinon
*/
unsigned long        Symbole::get_addr(const std::string &name) const
{
    std::map<unsigned long, SymboleSym *>::const_iterator    it;
    unsigned long                                            ret;

    const_cast<Mutex*>(&(this->_mutex))->lock();
    
    ret = 0;
    for (it=this->_sym.begin(); it!=this->_sym.end(); )
    {
        if (it->second->_name == name)
        {
            ret = it->first;
            it = this->_sym.end();
        }
        else
            it++;
    }
    
    const_cast<Mutex*>(&(this->_mutex))->unlock();
    return (ret);
}

/**
** \fn const std::string get_name(unsigned long addr) const
** \brief Gere la recuperation du nom d'un symbole a partir de son adresse
**
** \param addr Adresse du symbole a trouver
** \return Retourne le nom d'un symbole si on le trouve, "" sinon
*/
const std::string    Symbole::get_name(unsigned long addr) const
{
    std::map<unsigned long, SymboleSym *>::const_iterator    it;
    std::string                                              ret;
    
    const_cast<Mutex*>(&(this->_mutex))->lock();
    
    if ((it = this->_sym.find(addr)) != this->_sym.end())
        ret = it->second->_name;
    
    const_cast<Mutex*>(&(this->_mutex))->unlock();
    return (ret);
}

/**
** \fn const std::string &get_name(unsigned long addr) const
** \brief Gere la recuperation du type d'un symbole a partir de son adresse
**
** \param addr Adresse du symbole a trouver
** \return Retourne le type d'un symbole si on le trouve, Symbole::NONE sinon
*/
Symbole::eType       Symbole::get_type(unsigned long addr) const
{
    std::map<unsigned long, SymboleSym *>::const_iterator    it;
    Symbole::eType                                           ret;
    
    const_cast<Mutex*>(&(this->_mutex))->lock();
    
    ret = Symbole::NONE;
    if ((it = this->_sym.find(addr)) != this->_sym.end())
        ret = it->second->_type;
    
    const_cast<Mutex*>(&(this->_mutex))->unlock();
    return (ret);
}


/**
** \fn unsigned long get_nbs_symbole() const
** \brief Permet de savoir combien le gestionnaire contient de symboles
**
** \return Retourne le nombre de symboles contenu par le gestionnaire
*/
unsigned long        Symbole::get_nbs_symbole() const
{
    return (this->_sym.size());
}

/**
** \fn unsigned long get_addr_nieme_symbole(unsigned long n) const
** \brief Permet de connaitre l'adresse du N-ieme symbole
**
** \param n Numero du symbole
** \return Retourne l'adresse du symbole s'il existe, 0 sinon
*/
unsigned long        Symbole::get_addr_nieme_symbole(unsigned long n) const
{
    std::map<unsigned long, SymboleSym *>::const_iterator    it;
    unsigned long                                            ret;
    
    const_cast<Mutex*>(&(this->_mutex))->lock();
    
    ret = 0;
    if (n < this->_sym.size())
    {
        it = this->_sym.begin();
        std::advance(it, n);
        
        ret = it->first;
    }
    
    const_cast<Mutex*>(&(this->_mutex))->unlock();
    return (ret);
}

/**
** \fn const std::string get_name_nieme_symbole(unsigned long n) const
** \brief Permet de connaitre le nom du N-ieme symbole
**
** \param n Numero du symbole
** \return Retourne le nom du symbole s'il existe, "" sinon
*/
const std::string    Symbole::get_name_nieme_symbole(unsigned long n) const
{
    std::map<unsigned long, SymboleSym *>::const_iterator    it;
    std::string                                              ret;
    
    const_cast<Mutex*>(&(this->_mutex))->lock();
    
    if (n < this->_sym.size())
    {
        it = this->_sym.begin();
        std::advance(it, n);
        
        ret = it->second->_name;
    }

    const_cast<Mutex*>(&(this->_mutex))->unlock();
    return (ret);
}

/**
** \fn Symbole::eType get_type_nieme_symbole(unsigned long n) const
** \brief Permet de connaitre le type du N-ieme symbole
**
** \param n Numero du symbole
** \return Retourne le type du symbole s'il existe, Symbole::NONE sinon
*/
Symbole::eType       Symbole::get_type_nieme_symbole(unsigned long n) const
{
    std::map<unsigned long, SymboleSym *>::const_iterator    it;
    Symbole::eType                                           ret;
    
    const_cast<Mutex*>(&(this->_mutex))->lock();
    
    ret = Symbole::NONE;
    if (n < this->_sym.size())
    {
        it = this->_sym.begin();
        std::advance(it, n);
        
        ret = it->second->_type;
    }
    
    const_cast<Mutex*>(&(this->_mutex))->unlock();
    return (ret);
}

