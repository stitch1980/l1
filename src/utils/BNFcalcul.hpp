/*
** BNFcalcul.hpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 15:28:10 joran herve
** Last update Sat, 27 Jun 2015 15:28:10 joran herve
**
** Status : OK (thread-safe)
*/

#ifndef    BNF_CALCUL_HPP_
#define    BNF_CALCUL_HPP_

#include    <string>
#include    <map>

#include    "Info.hpp"


/**
** \namespace BNFcalcul
** \brief Fonctions de la BNF des calculs
** (les tokens sont indexes dans la map grace a leur type: TYPE_ASSOCIATION, TYPE_EXPRESSION, ...)
**
** association:
**    dest "=" expression
**
** expression:
**    operande
**    ( expression )
**    expression operateur expression
**    expression "?" expression ":" expression
**
** operateur:
**    "*" | "/" | "%" | "+" | "-" | "&&" | "||" | "&" | "^" | "|" | ">>" | "<<" | "==" | "!=" | "<" | "<=" | ">" | ">="
**
** operande:
**    dest
**    nbs
**
** dest:
**    registre
**    memoire
**
** nbs:
**    hexadecimal
**    decimal
**
** registre: (un des registres definis par la classe IASM)
**
** memoire:
**    "[" expression "]"
**
** decimal:
**    udecimal
**    "-" udecimal
**
** udecimal: (tout les nombres utilisant 0123456789)
**
** hexadecimal:
**    "0x" hexa_sans_0x
**
** hexa_sans_0x: (tout les nombres utilisant 0123456789abcdefABCDEF)
*/
namespace    BNFcalcul
{
    /**
    ** \fn eTypeToken
    ** \brief Enum contenant les type de token que peut contenir un calcul
    */
    enum eTypeToken
    {
        TYPE_ASSOCIATION,
        TYPE_EXPRESSION,
        TYPE_TERNAIRE_TEST,
        TYPE_TERNAIRE_CHOIX_1,
        TYPE_TERNAIRE_CHOIX_2,
        TYPE_OPERATEUR,
        TYPE_OPERANDE,
        TYPE_DEST,
        TYPE_NBS,
        TYPE_REGISTRE,
        TYPE_MEMOIRE,
        TYPE_DECIMAL,
        TYPE_UDECIMAL,
        TYPE_0xHEXA,
        TYPE_HEXA,
        TYPE_NONE
    };
    
    /**
    ** \fn unsigned long    is_association(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
    **                                     const IASM *a=NULL, unsigned long off=0, int save=0)
    ** \brief Fonction de la BNF verifiant si la chaine contient une association valide
    **
    ** \param s String a analyser (sans espaces ni retour a la ligne)
    ** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
    ** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
    ** \param off Offset a partir duquel chercher le token
    ** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
    ** \return Retourne la taille du token si on le trouve, 0 sinon
    */
    unsigned long    is_association(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                    const IASM *a=NULL, unsigned long off=0, int save=0);

    /**
    ** \fn unsigned long    is_expression(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
    **                                    const IASM *a=NULL, unsigned long off=0, int save=0)
    ** \brief Fonction de la BNF verifiant si la chaine contient une expression valide
    **
    ** \param s String a analyser (sans espaces ni retour a la ligne)
    ** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
    ** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
    ** \param off Offset a partir duquel chercher le token
    ** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
    ** \return Retourne la taille du token si on le trouve, 0 sinon
    */
    unsigned long    is_expression(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                   const IASM *a=NULL, unsigned long off=0, int save=0);

    /**
    ** \fn unsigned long    is_operateur(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
    **                                   const IASM *a=NULL, unsigned long off=0, int save=0)
    ** \brief Fonction de la BNF verifiant si la chaine contient un operateur valide
    **
    ** \param s String a analyser (sans espaces ni retour a la ligne)
    ** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
    ** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
    ** \param off Offset a partir duquel chercher le token
    ** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
    ** \return Retourne la taille du token si on le trouve, 0 sinon
    */
    unsigned long    is_operateur(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                  const IASM *a=NULL, unsigned long off=0, int save=0);

    /**
    ** \fn unsigned long    is_operande(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
    **                                  const IASM *a=NULL, unsigned long off=0, int save=0)
    ** \brief Fonction de la BNF verifiant si la chaine contient un operande valide
    **
    ** \param s String a analyser (sans espaces ni retour a la ligne)
    ** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
    ** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
    ** \param off Offset a partir duquel chercher le token
    ** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
    ** \return Retourne la taille du token si on le trouve, 0 sinon
    */
    unsigned long    is_operande(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                 const IASM *a=NULL, unsigned long off=0, int save=0);

    /**
    ** \fn unsigned long    is_dest(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
    **                              const IASM *a=NULL, unsigned long off=0, int save=0)
    ** \brief Fonction de la BNF verifiant si la chaine contient une dest valide
    **
    ** \param s String a analyser (sans espaces ni retour a la ligne)
    ** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
    ** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
    ** \param off Offset a partir duquel chercher le token
    ** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
    ** \return Retourne la taille du token si on le trouve, 0 sinon
    */
    unsigned long    is_dest(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                             const IASM *a=NULL, unsigned long off=0, int save=0);

    /**
    ** \fn unsigned long    is_nbs(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
    **                             const IASM *a=NULL, unsigned long off=0, int save=0)
    ** \brief Fonction de la BNF verifiant si la chaine contient un nbs valide
    **
    ** \param s String a analyser (sans espaces ni retour a la ligne)
    ** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
    ** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
    ** \param off Offset a partir duquel chercher le token
    ** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
    ** \return Retourne la taille du token si on le trouve, 0 sinon
    */
    unsigned long    is_nbs(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                            const IASM *a=NULL, unsigned long off=0, int save=0);

    /**
    ** \fn unsigned long    is_registre(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
    **                                  const IASM *a=NULL, unsigned long off=0, int save=0)
    ** \brief Fonction de la BNF verifiant si la chaine contient un registre valide
    **
    ** \param s String a analyser (sans espaces ni retour a la ligne)
    ** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
    ** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
    ** \param off Offset a partir duquel chercher le token
    ** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
    ** \return Retourne la taille du token si on le trouve, 0 sinon
    */
    unsigned long    is_registre(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                 const IASM *a=NULL, unsigned long off=0, int save=0);

    /**
    ** \fn unsigned long    is_memoire(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
    **                                 const IASM *a=NULL, unsigned long off=0, int save=0)
    ** \brief Fonction de la BNF verifiant si la chaine contient une adresse memoire valide
    **
    ** \param s String a analyser (sans espaces ni retour a la ligne)
    ** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
    ** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
    ** \param off Offset a partir duquel chercher le token
    ** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
    ** \return Retourne la taille du token si on le trouve, 0 sinon
    */
    unsigned long    is_memoire(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                const IASM *a=NULL, unsigned long off=0, int save=0);

    /**
    ** \fn unsigned long    is_decimal(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
    **                                 const IASM *a=NULL, unsigned long off=0, int save=0)
    ** \brief Fonction de la BNF verifiant si la chaine contient un decimal valide
    **
    ** \param s String a analyser (sans espaces ni retour a la ligne)
    ** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
    ** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
    ** \param off Offset a partir duquel chercher le token
    ** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
    ** \return Retourne la taille du token si on le trouve, 0 sinon
    */
    unsigned long    is_decimal(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                const IASM *a=NULL, unsigned long off=0, int save=0);

    /**
    ** \fn unsigned long    is_udecimal(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
    **                                  const IASM *a=NULL, unsigned long off=0, int save=0)
    ** \brief Fonction de la BNF verifiant si la chaine contient un udecimal valide
    **
    ** \param s String a analyser (sans espaces ni retour a la ligne)
    ** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
    ** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
    ** \param off Offset a partir duquel chercher le token
    ** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
    ** \return Retourne la taille du token si on le trouve, 0 sinon
    */
    unsigned long    is_udecimal(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                 const IASM *a=NULL, unsigned long off=0, int save=0);

    /**
    ** \fn unsigned long    is_hexadecimal(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
    **                                     const IASM *a=NULL, unsigned long off=0, int save=0)
    ** \brief Fonction de la BNF verifiant si la chaine contient un hexadecimal valide
    **
    ** \param s String a analyser (sans espaces ni retour a la ligne)
    ** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
    ** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
    ** \param off Offset a partir duquel chercher le token
    ** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
    ** \return Retourne la taille du token si on le trouve, 0 sinon
    */
    unsigned long    is_hexadecimal(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                    const IASM *a=NULL, unsigned long off=0, int save=0);

    /**
    ** \fn unsigned long    is_hexa_sans_0x(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
    **                                      const IASM *a=NULL, unsigned long off=0, int save=0)
    ** \brief Fonction de la BNF verifiant si la chaine contient un hexa_sans_0x valide
    **
    ** \param s String a analyser (sans espaces ni retour a la ligne)
    ** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
    ** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
    ** \param off Offset a partir duquel chercher le token
    ** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
    ** \return Retourne la taille du token si on le trouve, 0 sinon
    */
    unsigned long    is_hexa_sans_0x(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                     const IASM *a=NULL, unsigned long off=0, int save=0);
}

#endif

