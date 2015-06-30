/*
** ContentContexte.hpp for Lecter
**
** Made by joran herve
**
** Started on  Tue, 30 Jun 2015 13:18:15 joran herve
** Last update Tue, 30 Jun 2015 13:18:15 joran herve
**
** Status : OK (thread-safe)
*/

#ifndef    CONTENT_CONTEXTE_HPP_
#define    CONTENT_CONTEXTE_HPP_


#include    <string>
#include    <vector>
#include    <set>

#include    "Mutex.hpp"


#define    CONTENT_CONTEXTE_NBS_VALUES_MAX     256 /* Nombre de valeurs fixe maximal que peut avoir le Content */
#define    CONTENT_CONTEXTE_NBS_FILTERS_MAX    16  /* Nombre de filtres maximal que peut avoir le Content */


/**
** \class ContentContexte
** \brief Classe permettant de gerer les unitees de memoire du contexte d'execution
*/
class ContentContexte
{
public:
    /**
    ** \fn ContentContexte()
    ** \brief Constructeur par defaut du contenu de contexte
    */
    ContentContexte();

    /**
    ** \fn ContentContexte(unsigned long size, const std::string &type,
    **                     const std::set<std::string> &value, const std::set<std::string> &filter)
    ** \brief Constructeur complet du contenu de contexte
    **
    ** \param c Contenu de contexe a copier
    */
    ContentContexte(unsigned long size, const std::string &type,
                    const std::set<std::string> &value, const std::set<std::string> &filter);

    /**
    ** \fn ContentContexte(const ContentContexte &c)
    ** \brief Constructeur par copie du contenu de contexte
    **
    ** \param c Contenu de contexe a copier
    */
    ContentContexte(const ContentContexte &c);

    /**
    ** \fn ~ContentContexte()
    ** \brief Constructeur par defaut du contenu de contexte
    */
    ~ContentContexte();

    unsigned long                  get_size() const;
    const std::string              &get_type() const;
    const std::set<std::string>    &get_values() const;
    const std::set<std::string>    &get_filters() const;

    void    set_size(unsigned long s);
    void    set_type(const std::string &t);
    void    set_value(const std::string &v);
    void    set_values(const std::set<std::string> &v);
    void    set_filters(const std::set<std::string> &f);

    void    add_values(const std::set<std::string> &v);
    void    add_filters(const std::set<std::string> &f);

    void    clear_values();
    void    clear_filters();

    void    set_must_be_delete(int i);
    int     get_must_be_delete() const;

protected:
    /** Taille de la valeur en memoire */
    unsigned long            _size;
    /** Type de valeur ("int", "int*", ...) */
    std::string              _type;
    /** Valeurs (peut etre une expression) */
    std::set<std::string>    _values;
    /** Filtre pour deduire les valeurs possibles du contexte */
    std::set<std::string>    _filters;

    /** Pour savoir s'il s'agit un ContentContexte temporaire qui doit etre detruit apres usage */
    int                      _must_be_delete;

    /** Un mutex pour rendre la clesse thread-safe */
    Mutex                    _mutex;
};

#endif

