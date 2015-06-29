/*
** BIBlocSource.hpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 15:28:10 joran herve
** Last update Sat, 27 Jun 2015 15:28:10 joran herve
**
** Status : OK
*/

#ifndef    I_BLOC_SOURCE_HPP_
#define    I_BLOC_SOURCE_HPP_

#include    <string>

/**
** \class IBlocSource
** \brief Interface des classes permettant de contenir le code source d'une fonction C
*/
class    IBlocSource
{
public:
    /**
    ** \fn IBlocSource()
    ** \brief Constructeur d'un bloc de code source
    */
    IBlocSource() {}

    /**
    ** \fn ~IBlocSource()
    ** \brief Destructeur d'un bloc de code source
    */
    virtual ~IBlocSource() { this->clear(); }

protected:
    IBlocSource(const IBlocSource &) {}
    IBlocSource &operator = (const IBlocSource &) { return (*this); }

public:
    /**
    ** \fn void clear()
    ** \brief Supprime le contenu du bloc de source
    **
    ** \return Retourne rien
    */
    virtual void           clear() {}

    /**
    ** \fn std::string to_string() const
    ** \brief Gere la conversion de tout les blocs de source en un code-source complet
    **
    ** \return Retourne une string contenant le code source complet
    */
    std::string    to_string() const
    {
        std::string    dest;
        return (this->to_string(dest));
    };

    /**
    ** \fn std::string to_string(std::string &dest) const
    ** \brief Gere la conversion de tout les blocs de source en un code-source complet
    **
    ** \param dest String ou mettre le code-source (le code est concatene a la chaine)
    ** \return Retourne une string contenant le code source complet
    */
    virtual std::string    to_string(std::string &dest) const = 0;
};

#endif

