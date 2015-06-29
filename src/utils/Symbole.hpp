/*
** Symbole.hpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 15:28:10 joran herve
** Last update Sat, 27 Jun 2015 15:28:10 joran herve
**
** Status : OK (thread-safe)
*/

#ifndef SYMBOLE_HPP_
#define SYMBOLE_HPP_

#include    <string>
#include    <map>

#include    "Mutex.hpp"


/**
** \class SymboleSym
** \brief Classe decrivant un symbole
*/
class    SymboleSym;

/**
** \class Symbole
** \brief Classe permettant de gerer les symboles (thread-safe)
*/
class    Symbole
{
public:
    /**
    ** \enum eType
    ** \brief Enum permettant d'identifier les differents types de symboles
    */
    enum    eType
    {
        FUNC,   /* Symbole indiquant une fonction */
        VAR,    /* Symbole indiquant une variable */
        NONE    /* Non determine */
    };

public:
    /**
    ** \fn Symbole()
    ** \brief Constructeur du gestionnaire de symboles
    */
    Symbole();

    /**
    ** \fn ~Symbole()
    ** \brief Destructeur du gestionnaire de symboles
    */
    ~Symbole();


    /**
    ** \fn void clear()
    ** \brief Gere le vidage du gestionnaire de symboles
    **
    ** \return Retourne rien
    */
    void    clear();

    /**
    ** \fn void add_symbole(unsigned long addr, const std::string &name, Symbole::eType type)
    ** \brief Gere l'ajout ou le remplacement d'un symbole dans le gestionnaire
    **
    ** \param addr Adresse du symbole
    ** \param name Nom du symbole
    ** \param type Type de symbole
    ** \return Retourne rien
    */
    void    add_symbole(unsigned long addr, const std::string &name, Symbole::eType type);

    /**
    ** \fn void del_symbole(unsigned long addr)
    ** \brief Gere la suppression d'un symbole a partir de son adresse
    **
    ** \param addr Adresse du symbole a supprimer
    ** \return Retourne rien
    */
    void    del_symbole(unsigned long addr);


    /**
    ** \fn unsigned long get_addr(const std::string &name) const
    ** \brief Gere la recuperation de l'adresse d'un symbole a partir de son nom
    **
    ** \param name Nom du symbole a trouver
    ** \return Retourne l'adresse d'un symbole si on le trouve, 0 sinon
    */
    unsigned long        get_addr(const std::string &name) const;

    /**
    ** \fn const std::string get_name(unsigned long addr) const
    ** \brief Gere la recuperation du nom d'un symbole a partir de son adresse
    **
    ** \param addr Adresse du symbole a trouver
    ** \return Retourne le nom d'un symbole si on le trouve, "" sinon
    */
    const std::string    get_name(unsigned long addr) const;

    /**
    ** \fn const std::string &get_name(unsigned long addr) const
    ** \brief Gere la recuperation du type d'un symbole a partir de son adresse
    **
    ** \param addr Adresse du symbole a trouver
    ** \return Retourne le type d'un symbole si on le trouve, Symbole::NONE sinon
    */
    Symbole::eType       get_type(unsigned long addr) const;


    /**
    ** \fn unsigned long get_nbs_symbole() const
    ** \brief Permet de savoir combien le gestionnaire contient de symboles
    **
    ** \return Retourne le nombre de symboles contenu par le gestionnaire
    */
    unsigned long        get_nbs_symbole() const;

    /**
    ** \fn unsigned long get_addr_nieme_symbole(unsigned long n) const
    ** \brief Permet de connaitre l'adresse du N-ieme symbole
    **
    ** \param n Numero du symbole
    ** \return Retourne l'adresse du symbole s'il existe, 0 sinon
    */
    unsigned long        get_addr_nieme_symbole(unsigned long n) const;

    /**
    ** \fn const std::string get_name_nieme_symbole(unsigned long n) const
    ** \brief Permet de connaitre le nom du N-ieme symbole
    **
    ** \param n Numero du symbole
    ** \return Retourne le nom du symbole s'il existe, "" sinon
    */
    const std::string    get_name_nieme_symbole(unsigned long n) const;

    /**
    ** \fn Symbole::eType get_type_nieme_symbole(unsigned long n) const
    ** \brief Permet de connaitre le type du N-ieme symbole
    **
    ** \param n Numero du symbole
    ** \return Retourne le type du symbole s'il existe, Symbole::NONE sinon
    */
    Symbole::eType       get_type_nieme_symbole(unsigned long n) const;

protected:
    /** Tableau contenant les symboles indexees par leur adresses */
    std::map<unsigned long, SymboleSym*>    _sym;
    
    /** Mutex permettant de rendre de gestionnaire de symboles thread-safe */
    Mutex                                   _mutex;
};

#endif

