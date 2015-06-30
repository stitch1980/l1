/*
** Calcul.hpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 14:34:37 joran herve
** Last update Sat, 27 Jun 2015 14:34:37 joran herve
**
** Status : Pas totalement teste (Et peut largement etre optimise)
*/

#ifndef    CALCUL_HPP_
#define    CALCUL_HPP_

#include    <string>
#include    <cstring>
#include    <cstdio>
#include    <map>
#include    <set>

#include    "Info.hpp"
#include    "Contexte.hpp"
#include    "BNFcalcul.hpp"

#define     MAX_RESULT_SET    CONTENT_CONTEXTE_NBS_VALUES_MAX


/**
** \namespace Calcul
** \brief Namespace gerant les calculs
*/
namespace    Calcul
{
    class    NodeCalcul
    {
    public:
        /**
        ** \enum eType
        ** \brief Enum des differents types de Nodes
        */
        enum eType
        {
            TYPE_PAR, /* Le node est une parenthese ouvrante */
            TYPE_OPER, /* Le node est un operateur */
            TYPE_NBS   /* Le node est un nombre */
        };

    public:
        NodeCalcul(): _type(TYPE_NBS), _priority(0), _func(NULL), _values(), _name() {}

        NodeCalcul(Calcul::NodeCalcul::eType t): _type(t), _priority(0), _func(NULL), _values(), _name() {}

        NodeCalcul(std::set<std::string> &(*func)(std::set<std::string>&,
                                                      const Calcul::NodeCalcul*,
                                                      const Calcul::NodeCalcul*),
                   const char *value): _type(TYPE_NBS), _priority(0), _func(func), _values(), _name()
       {
           if (func != NULL)
               this->_type = Calcul::NodeCalcul::TYPE_OPER;
           if ((value != NULL) && (strlen(value) > 0))
               this->_values.insert(value);
       }

        NodeCalcul(std::set<std::string> &(*func)(std::set<std::string>&,
                                                      const Calcul::NodeCalcul*,
                                                      const Calcul::NodeCalcul*),
                   const std::string &value): _type(TYPE_NBS), _priority(0), _func(func), _values(), _name()
       {
           if (func != NULL)
               this->_type = Calcul::NodeCalcul::TYPE_OPER;
           if (value.size() > 0)
               this->_values.insert(value);
       }

        NodeCalcul(std::set<std::string> &(*func)(std::set<std::string>&,
                                                      const Calcul::NodeCalcul*,
                                                      const Calcul::NodeCalcul*),
                   const std::set<std::string> &value): _type(TYPE_NBS), _priority(0), _func(func), _values(value), _name()
       {
           if (func != NULL)
               this->_type = Calcul::NodeCalcul::TYPE_OPER;
       }

        NodeCalcul(const NodeCalcul &n): _type(n._type), _priority(0), _func(n._func), _values(n._values), _name() {}

    public:
        /** Type de node */
        Calcul::NodeCalcul::eType    _type;

        /** Priorite de l'operateur */
        int                          _priority;
        /** Pointeur sur la fonction de calcul si le node est un operateur */
        std::set<std::string>        &(*_func)(std::set<std::string>&, const Calcul::NodeCalcul*, const Calcul::NodeCalcul*);

        /** Les differentes valeurs possible d'un nombre */
        std::set<std::string>        _values;
        /** Variable d'ou proviennent les valeurs) */
        std::string                  _name;
    };


    /*
    ** Les fonctions de calcul et d'estimations des resultats permettant d'evaluer
    ** les calculs arithmetiques et logiques
    */
    long                     calcul(const std::string &expr, const Info *info, const Contexte *ctx);
    std::string              calcul_str(const std::string &expr, const Info *info, const Contexte *ctx);
    std::string              &calcul_str(std::string &dest, const std::string &expr, const Info *info, const Contexte *ctx);
    std::set<std::string>    evaluate(const std::string &expr, const Info *info, const Contexte *ctx);
    std::set<std::string>    &evaluate(std::set<std::string> &dest, const std::string &expr, const Info *info, const Contexte *ctx);
    std::set<std::string>    &evaluate_add_result(std::set<std::string> &dest, const std::string &expr, const Info *info, const Contexte *ctx);



    /**
    ** \fn int make_npi(const std::string &expr, const Info *info,
    **                  const Contexte *ctx, std::vector<std::string> &npi)
    ** \brief Gere la creation d'une NPI a partir d'une expression
    **
    ** \param expr Expression a resoudre
    ** \param info Structure contenant les infos du programme
    ** \param ctx Contexte d'execution au moment du calcul
    ** \param npi Vector devant contenir la NPI
    ** \return Retourne 1 si OK, 0 sinon
    */
    int    make_npi(const std::string &expr, const Info *info, const Contexte *ctx, std::list<Calcul::NodeCalcul*> &npi);

    /**
    ** \fn Calcul::NodeCalcul *resolve_NPI(std::list<Calcul::NodeCalcul*> &npi)
    ** \brief Gere l'estimation des resultats d'une NPI (elle doit etre valide)
    **
    ** \param npi NPI a resoudre
    ** \return Retourne un node contenant les resultats de la NPI
    */
    Calcul::NodeCalcul    *resolve_NPI(std::list<Calcul::NodeCalcul*> &npi);

    /**
    ** \fn void clear_npi(std::list<Calcul::NodeCalcul*> &npi)
    ** \brief Gere la suppression de tout les Nodes d'une NPI
    **
    ** \brief npi NPI a vider
    ** \return Retourne rien
    */
    void    clear_npi(std::list<Calcul::NodeCalcul*> &npi);



    /**
    ** \fn Calcul::NodeCalcul *do_oper(NodeCalcul *dest, const NodeCalcul *n1, const NodeCalcul *oper, const NodeCalcul *n2)
    ** \brief Gere l'execution des calculs simple entre Nodes
    **
    ** \param dest Node destination
    ** \param n1 Node du premier nombre
    ** \param oper Node de l'operateur contenant le pointeur sur la fonction de calcul
    ** \param n2 Node du deuxieme nombre
    ** \return Retourne un pointeur sur la destination
    */
    Calcul::NodeCalcul    *do_oper(NodeCalcul *dest, const NodeCalcul *n1, const NodeCalcul *oper, const NodeCalcul *n2);

    /*
    ** Ces fonctions permettent de d'estimer les resultats possibles d'un calcul simple.
    ** Les parametres "n1" et "n2" sont les operandes et "dest" est le resultat.
    **
    ** Certaines d'entre elles sont optimisees afin de diminuer les faux positifs
    ** Si un des operandes ou le resultat ont plus de MAX_RESULT_SET possibilites,
    ** ils sont vide afin de signifier qu'il sont totalement innconu. Cela permet
    ** de limiter le temps de calcul et la memoire utilisee
    */
    std::set<std::string>    &do_oper_mul(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_div(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_mod(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_add(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_sub(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_shr(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_shl(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_and(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_xor(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_or(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_eg(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_dif(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_lo(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_le(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_gr(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_ge(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_andand(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);
    std::set<std::string>    &do_oper_oror(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2);

    /*
    ** Ces fonctions permettent d'optimiser l'estimation des resultats d'un AND-logique
    ** si un des operandes est inconnu.
    **
    ** Elles utilisent des masques pour determiner les bits qui seront toujours a 0
    */
    std::set<std::string>    &gestion_masque_oper_and(std::set<std::string> &dest,
                                                      const Calcul::NodeCalcul *n1,
                                                      const Calcul::NodeCalcul *n2);
    int    gestion_masque_oper_and_rec(unsigned long masque[64], std::set<std::string> &dest,
                                       unsigned long bit);
          
    /*
    ** Ces fonctions permettent d'optimiser l'estimation des resultats d'un OR-logique
    ** si un des operandes est inconnu.
    **
    ** Elles utilisent des masques pour determiner les bits qui seront toujours a 1
    */
    std::set<std::string>    &gestion_masque_oper_or(std::set<std::string> &dest,
                                                     const Calcul::NodeCalcul *n1,
                                                     const Calcul::NodeCalcul *n2);
    int    gestion_masque_oper_or_rec(unsigned long masque[64],
                                      std::set<std::string> &dest,
                                      unsigned long bit);


/*
** Fonctions pouvant etre utiles aux calculs
*/
    /**
    ** \fn std::string &suppr_space_expr(std::string &s)
    ** \brief Gere la suppression des espaces et des tabulations dans une string
    **
    ** \param s String a epurer
    ** \return Retourne rien
    */
    std::string    &suppr_space_expr(std::string &s);

    /**
    ** \fn std::string &suppr_space_expr(std::string &s)
    ** \brief Gere la suppression des retours a la ligne dans une string
    **
    ** \param s String a epurer
    ** \return Retourne rien
    */
    std::string    &suppr_endl_expr(std::string &s);


    /**
    ** \fn int get_priority_oper(const char *oper)
    ** \brief Gere l'identification du niveau de prioritee d'un operateur
    **
    ** \param oper Pointeur sur la chaine contenant l'operateur
    ** \return Retourne le niveau de priorite de l'operateur si OK, 0 sinon
    **
    ** * / %            : 8
    ** + -              : 7
    ** << >>            : 6
    ** &                : 5
    ** ^                : 4
    ** |                : 3
    ** == != < <= > >=  : 2
    ** && ||            : 1
    ** ERREUR           : 0
    */
    int            get_priority_oper(const char *oper);


    /**
    ** \fn std::string ltos(long n)
    ** \brief Gere la converion d'un long en string decimal
    **
    ** \param n Nombre a convertir
    ** \return Retourne une string contenant le nombre
    */
    std::string    ltos(long n);

    /**
    ** \fn std::string ltox(long n)
    ** \brief Gere la converion d'un long en string hexadecimal
    **
    ** \param n Nombre a convertir
    ** \return Retourne une string contenant le nombre
    */
    std::string    ltox(long n);

    /**
    ** \fn std::string ltos(long n)
    ** \brief Gere la converion d'un long en string 0xhexadecimal
    **
    ** \param n Nombre a convertir
    ** \return Retourne une string contenant le nombre
    */
    std::string    lto0x(long n);


    /**
    ** \fn std::string &ltos(std::string &dest, long n)
    ** \brief Gere la converion d'un long en string decimal
    **
    ** \param dest String ou mettre le nombre converti
    ** \param n Nombre a convertir
    ** \return Retourne une string contenant le nombre
    */
    std::string    &ltos(std::string &dest, long n);

    /**
    ** \fn std::string &ltox(std::string &dest, long n)
    ** \brief Gere la converion d'un long en string hexadecimal
    **
    ** \param dest String ou mettre le nombre converti
    ** \param n Nombre a convertir
    ** \return Retourne une string contenant le nombre
    */
    std::string    &ltox(std::string &dest, long n);

    /**
    ** \fn std::string &ltos(std::string &dest, long n)
    ** \brief Gere la converion d'un long en string 0xhexadecimal
    **
    ** \param dest String ou mettre le nombre converti
    ** \param n Nombre a convertir
    ** \return Retourne une string contenant le nombre
    */
    std::string    &lto0x(std::string &dest, long n);


    /**
    ** \fn long xtol(const char *s)
    ** \brief Gere la conversion d'une chaine de caractere hexadecimal en long
    **
    ** \param s Chaine de caracteres hexadecimal a convertir en long
    ** \return Retourne le nombre contenu dans la chaine si OK, 0 sinon
    */
    long           xtol(const char *s);

    /**
    ** \fn long Oxtol(const char *s)
    ** \brief Gere la conversion d'une chaine de caractere 0xhexa en long (elle a interet a etre valide)
    **
    ** \param s Chaine de caracteres 0xhexadecimal a convertir en long
    ** \return Retourne le nombre contenu dans la chaine si OK, 0 sinon
    */
    long           Oxtol(const char *s);
}

#endif

