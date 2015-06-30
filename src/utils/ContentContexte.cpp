/*
** ContentContexte.cpp for Lecter
**
** Made by joran herve
**
** Started on  Tue, 30 Jun 2015 13:18:15 joran herve
** Last update Tue, 30 Jun 2015 13:18:15 joran herve
**
** Status : OK (thread-safe)
*/

#include    "ContentContexte.hpp"


/**
** \fn ContentContexte()
** \brief Constructeur par defaut du contenu de contexte
*/
ContentContexte::ContentContexte():
    _size(4),
    _type("int"),
    _values(),
    _filters(),
    _must_be_delete(0),
    _mutex()
{
}

/**
** \fn ContentContexte(unsigned long size, const std::string &type,
**                     const std::set<std::string> &value, const std::set<std::string> &filter)
** \brief Constructeur complet du contenu de contexte
**
** \param c Contenu de contexe a copier
*/
ContentContexte::ContentContexte(unsigned long size, const std::string &type,
                                 const std::set<std::string> &value,
                                 const std::set<std::string> &filter):
    _size(size),
    _type(type),
    _values(value),
    _filters(filter),
    _must_be_delete(0),
    _mutex()
{
}

/**
** \fn ContentContexte(const ContentContexte &c)
** \brief Constructeur par copie du contenu de contexte
**
** \param c Contenu de contexe a copier
*/
ContentContexte::ContentContexte(const ContentContexte &c):
    _size(),
    _type(),
    _values(),
    _filters(),
    _must_be_delete(0),
    _mutex()
{
    this->_size = c.get_size();
    this->_type = c.get_type();
    this->_values = c.get_values();
    this->_filters = c.get_filters();
    this->_must_be_delete = c.get_must_be_delete();
}

/**
** \fn ~ContentContexte()
** \brief Constructeur par defaut du contenu de contexte
*/
ContentContexte::~ContentContexte()
{
}


unsigned long                 ContentContexte::get_size() const
{
    return (this->_size);
}

const std::string             &ContentContexte::get_type() const
{
    return (this->_type);
}

const std::set<std::string>    &ContentContexte::get_values() const
{
    return (this->_values);
}

const std::set<std::string>    &ContentContexte::get_filters() const
{
    return (this->_filters);
}


void    ContentContexte::set_size(unsigned long s)
{
    this->_mutex.lock();
    this->_size = s;
    this->_mutex.unlock();
}

void    ContentContexte::set_type(const std::string &t)
{
    this->_mutex.lock();
    this->_type = t;
    this->_mutex.unlock();
}

void    ContentContexte::set_value(const std::string &v)
{
    this->_mutex.lock();

    this->_values.clear();
    if (v.size() > 0)
        this->_values.insert(v);

    this->_mutex.unlock();
}

void    ContentContexte::set_values(const std::set<std::string> &v)
{
    this->_mutex.lock();

    if (v.size() > CONTENT_CONTEXTE_NBS_VALUES_MAX)
        this->_values.clear();
    else
        this->_values = v;

    this->_mutex.unlock();
}

void    ContentContexte::set_filters(const std::set<std::string> &f)
{
    this->_mutex.lock();

    if (f.size() > CONTENT_CONTEXTE_NBS_FILTERS_MAX)
        this->_filters.clear();
    else
        this->_filters = f;

    this->_mutex.unlock();
}


void    ContentContexte::add_values(const std::set<std::string> &v)
{
    this->_mutex.lock();

    if ((this->_values.size() + v.size()) > CONTENT_CONTEXTE_NBS_VALUES_MAX)
        this->_values.clear();
    else
        this->_values.insert(v.begin(), v.end());

    this->_mutex.unlock();
}

void    ContentContexte::add_filters(const std::set<std::string> &f)
{
    this->_mutex.lock();

    if ((this->_filters.size() + f.size()) > CONTENT_CONTEXTE_NBS_FILTERS_MAX)
        this->_filters.clear();
    else
        this->_filters.insert(f.begin(), f.end());

    this->_mutex.unlock();
}


void    ContentContexte::clear_values()
{
    this->_mutex.lock();
    this->_values.clear();
    this->_mutex.unlock();
}

void    ContentContexte::clear_filters()
{
    this->_mutex.lock();
    this->_filters.clear();
    this->_mutex.unlock();
}


void    ContentContexte::set_must_be_delete(int i)
{
    this->_must_be_delete = i;
}

int     ContentContexte::get_must_be_delete() const
{
    return (this->_must_be_delete);
}

