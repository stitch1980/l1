/*
** BNFcalcul.cpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 15:28:10 joran herve
** Last update Sat, 27 Jun 2015 15:28:10 joran herve
**
** Status : OK (thread-safe)
*/

#include    "BNFcalcul.hpp"


/*
** Tableau contenant tout les operateurs geres
*/
const char    *operateur[] =
{
    "*",  "/",  "%",
    "+",  "-",
    "&&", "||",
    "&",  "^",  "|",
    ">>", "<<",
    "==", "!=", "<", "<=", ">", ">=",
    NULL
};

/**
** \fn unsigned long    is_association(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
**                                     const IASM *a, unsigned long off, int save)
** \brief Fonction de la BNF verifiant si la chaine contient une association valide
**
** \param s String a analyser (sans espaces ni retour a la ligne)
** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
** \param off Offset a partir duquel chercher le token
** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
** \return Retourne la taille du token si on le trouve, 0 sinon
*/
unsigned long     BNFcalcul::is_association(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                        const IASM *a, unsigned long off, int save)
{
    unsigned long    i;
    unsigned long    tmp;

    /* Recuperation de la destination */
    if ((i = BNFcalcul::is_dest(s, m, a, off, save-1)) == 0)
        return (0);

    /* Recuperation du '=' */
    if (s[off + i] != '=')
        return (0);
    i++;

    /* Recuperation de l'expression */
    if ((tmp = BNFcalcul::is_expression(s, m, a, off + i, save-1)) > 0)
    {
        /* Sauvegarde du token dans la map si besoin est et renvoi de la taille du token */
        if ((save > 0) && (m != NULL))
            (*m)[BNFcalcul::TYPE_ASSOCIATION] = s.substr(off, i + tmp);
        return (i + tmp);
    }
    return (0);
}

/**
** \fn unsigned long    is_expression(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
**                                    const IASM *a, unsigned long off, int save)
** \brief Fonction de la BNF verifiant si la chaine contient une expression valide
**
** \param s String a analyser (sans espaces ni retour a la ligne)
** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
** \param off Offset a partir duquel chercher le token
** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
** \return Retourne la taille du token si on le trouve, 0 sinon
*/
unsigned long    BNFcalcul::is_expression(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                       const IASM *a, unsigned long off, int save)
{
    unsigned long    i;
    unsigned long    tmp;

    if (off >= s.size())
        return (0);

    /* Gestion des expressions de type operande */
    i = BNFcalcul::is_operande(s, m, a, off, 0);

    /* Gestion des expressions de type '(' expression ')' */
    if ((i == 0) && (s[off] == '('))
    {
        i = 1;
        if ((tmp = BNFcalcul::is_expression(s, m, a, off+i, 0)) > 0)
        {
            i += tmp;
            /* Recherche de la parenthese fermante */
            if (s[off + i] != ')')
                return (0);
            i++;
        }
        else
            return (0);
    }
    if (i == 0)
        return (0);

    /* Gestion des expressions de type operande operateur expression */
    if ((tmp = BNFcalcul::is_operateur(s, m, a, off+i, 0)) > 0)
    {
        i += tmp;
        if ((tmp = BNFcalcul::is_expression(s, m, a, off+i, 0)) == 0)
            return (0);
        i += tmp;
    }

    /* Gestion des expressions ternaires */
    if ((save > 0) && (m != NULL))
    {
        (*m)[BNFcalcul::TYPE_TERNAIRE_TEST] = "";
        (*m)[BNFcalcul::TYPE_TERNAIRE_CHOIX_1] = "";
        (*m)[BNFcalcul::TYPE_TERNAIRE_CHOIX_2] = "";
    }
    if (s[off + i] == '?')
    {
        std::string     ternaire_test;
        std::string     ternaire_choix1;
        std::string     ternaire_choix2;

        ternaire_test = s.substr(off, i);
        i++;

        /* Recuperation du choix 1 */
        if ((tmp = BNFcalcul::is_expression(s, m, a, off+i, 0)) == 0)
            return (0);
        if (save > 0)
            ternaire_choix1 = s.substr(off + i, tmp);
        i += tmp;

        /* Recuperation du choix 2 */
        if (s[off + i] == ':')
        {
            i++;
            if ((tmp = BNFcalcul::is_expression(s, m, a, off+i, 0)) == 0)
                return (0);
            if (save > 0)
                ternaire_choix2 = s.substr(off + i, tmp);
            i += tmp;

            if ((save > 0) && (m != NULL))
            {
                (*m)[BNFcalcul::TYPE_TERNAIRE_TEST] = ternaire_test;
                (*m)[BNFcalcul::TYPE_TERNAIRE_CHOIX_1] = ternaire_choix1;
                (*m)[BNFcalcul::TYPE_TERNAIRE_CHOIX_2] = ternaire_choix2;
            }
        }
    }

    if ((save > 0) && (m != NULL))
        (*m)[BNFcalcul::TYPE_EXPRESSION] = s.substr(off, i);
    return (i);
}

/**
** \fn unsigned long    is_operateur(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
**                                   const IASM *a, unsigned long off, int save)
** \brief Fonction de la BNF verifiant si la chaine contient un operateur valide
**
** \param s String a analyser (sans espaces ni retour a la ligne)
** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
** \param off Offset a partir duquel chercher le token
** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
** \return Retourne la taille du token si on le trouve, 0 sinon
*/
unsigned long    BNFcalcul::is_operateur(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                      const IASM *, unsigned long off, int save)
{
    if (off >= s.size())
        return (0);

    /* Parcours les registre a la recherche d'une correspondance */
    for (unsigned long i=0; operateur[i]!=NULL; i++)
    {
        /* Si on trouve l'operateur... */
        if (s.find(operateur[i], off) == off)
        {
            /* Sauvegarde du token dans la map si besoin est et renvoi de la taille du token */
            if ((save > 0) && (m != NULL))
                (*m)[BNFcalcul::TYPE_OPERATEUR] = operateur[i];
            return (strlen(operateur[i]));
        }
    }

    return (0);
}

/**
** \fn unsigned long    is_operande(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
**                                  const IASM *a, unsigned long off, int save)
** \brief Fonction de la BNF verifiant si la chaine contient un operande valide
**
** \param s String a analyser (sans espaces ni retour a la ligne)
** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
** \param off Offset a partir duquel chercher le token
** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
** \return Retourne la taille du token si on le trouve, 0 sinon
*/
unsigned long    BNFcalcul::is_operande(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                     const IASM *a, unsigned long off, int save)
{
    unsigned long    i;

    i = 0;
    if ((i = BNFcalcul::is_dest(s, m, a, off, save-1)) == 0)
        i = BNFcalcul::is_nbs(s, m, NULL, off, save-1);

    /* Sauvegarde du token dans la map si besoin est et renvoi de la taille du token */
    if ((i > 0) && (save > 0) && (m != NULL))
        (*m)[BNFcalcul::TYPE_OPERANDE] = s.substr(off, i);
    return (i);
}

/**
** \fn unsigned long    is_dest(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
**                              const IASM *a, unsigned long off, int save)
** \brief Fonction de la BNF verifiant si la chaine contient une dest valide
**
** \param s String a analyser (sans espaces ni retour a la ligne)
** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
** \param off Offset a partir duquel chercher le token
** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
** \return Retourne la taille du token si on le trouve, 0 sinon
*/
unsigned long    BNFcalcul::is_dest(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                 const IASM *a, unsigned long off, int save)
{
    unsigned long    i;

    i = 0;
    if ((i = BNFcalcul::is_registre(s, m, a, off, save-1)) == 0)
        i = BNFcalcul::is_memoire(s, m, a, off, save-1);

    /* Sauvegarde du token dans la map si besoin est et renvoi de la taille du token */
    if ((i > 0) && (save > 0) && (m != NULL))
        (*m)[BNFcalcul::TYPE_DEST] = s.substr(off, i);
    return (i);
}

/**
** \fn unsigned long    is_nbs(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
**                             const IASM *, unsigned long off=0, int save=0)
** \brief Fonction de la BNF verifiant si la chaine contient un nbs valide
**
** \param s String a analyser (sans espaces ni retour a la ligne)
** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
** \param off Offset a partir duquel chercher le token
** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
** \return Retourne la taille du token si on le trouve, 0 sinon
*/
unsigned long    BNFcalcul::is_nbs(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                const IASM *, unsigned long off, int save)
{
    unsigned long    i;

    i = 0;
    if ((i = BNFcalcul::is_hexadecimal(s, m, NULL, off, save-1)) == 0)
        i = BNFcalcul::is_decimal(s, m, NULL, off, save-1);

    /* Sauvegarde du token dans la map si besoin est et renvoi de la taille du token */
    if ((i > 0) && (save > 0) && (m != NULL))
        (*m)[BNFcalcul::TYPE_NBS] = s.substr(off, i);
    return (i);
}

/**
** \fn unsigned long    is_registre(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
**                                  const IASM *a, unsigned long off, int save)
** \brief Fonction de la BNF verifiant si la chaine contient un registre valide
**
** \param s String a analyser (sans espaces ni retour a la ligne)
** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
** \param off Offset a partir duquel chercher le token
** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
** \return Retourne la taille du token si on le trouve, 0 sinon
*/
unsigned long    BNFcalcul::is_registre(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                     const IASM *a, unsigned long off, int save)
{
    const char       **reg;
    unsigned long    size;

    if ((off >= s.size()) || (a == NULL) || ((reg = a->get_registre()) == NULL))
        return (0);

    /* Parcours les registre a la recherche d'une correspondance */
    for (unsigned long i=0; reg[i]!=NULL; i++)
    {
        /* Si on trouve le nom d'un registre... */
        if (s.find(reg[i], off) == off)
        {
            /* ... et que c'est bien le nom d'un registre */
            size = strlen(reg[i]);
            if ((isalnum(s[off + size]) == 0) && (s[off + size] != '_'))
            {
                /* Sauvegarde du token dans la map si besoin est et renvoi de la taille du token */
                if ((save > 0) && (m != NULL))
                    (*m)[BNFcalcul::TYPE_REGISTRE] = s.substr(off, size);
                return (size);
            }
        }
    }

    return (0);
}

/**
** \fn unsigned long    is_memoire(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
**                                 const IASM *a, unsigned long off, int save)
** \brief Fonction de la BNF verifiant si la chaine contient une adresse memoire valide
**
** \param s String a analyser (sans espaces ni retour a la ligne)
** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
** \param a Gestionnaire de l'assembleur pour pouvoir gerer les registres
** \param off Offset a partir duquel chercher le token
** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
** \return Retourne la taille du token si on le trouve, 0 sinon
*/
unsigned long    BNFcalcul::is_memoire(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                    const IASM *a, unsigned long off, int save)
{
    unsigned long    i;
    unsigned long    tmp;

    if (off >= s.size())
        return (0);

    i = 0;
    if (s[off + i] != '[')
        return (0);
    i++;

    if ((tmp = BNFcalcul::is_expression(s, m, a, off+i, save-1)) == 0)
        return (0);
    i += tmp;

    if (s[off + i] != ']')
        return (0);
    i++;

    /* Sauvegarde du token dans la map si besoin est et renvoi de la taille du token */
    if ((save > 0) && (m != NULL))
        (*m)[BNFcalcul::TYPE_MEMOIRE] = s.substr(off, i);
    return (i);
}

/**
** \fn unsigned long    is_decimal(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
**                                 const IASM *, unsigned long off, int save)
** \brief Fonction de la BNF verifiant si la chaine contient un decimal valide
**
** \param s String a analyser (sans espaces ni retour a la ligne)
** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
** \param off Offset a partir duquel chercher le token
** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
** \return Retourne la taille du token si on le trouve, 0 sinon
*/
unsigned long    BNFcalcul::is_decimal(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                    const IASM *, unsigned long off, int save)
{
    unsigned long    i;
    unsigned long    tmp;

    if (off >= s.size())
        return (0);

    i = 0;
    if (s[off] == '-')
        i = 1;

    /* Sauvegarde du token dans la map si besoin est et renvoi de la taille du token */
    if ((tmp = BNFcalcul::is_udecimal(s, m, NULL, off + i, 0)) > 0)
    {
        if ((save > 0) && (m != NULL))
            (*m)[BNFcalcul::TYPE_DECIMAL] = s.substr(off, i + tmp);
        return (i + tmp);
    }
    return (0);
}

/**
** \fn unsigned long    is_udecimal(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
**                                  const IASM *, unsigned long off, int save)
** \brief Fonction de la BNF verifiant si la chaine contient un udecimal valide
**
** \param s String a analyser (sans espaces ni retour a la ligne)
** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
** \param off Offset a partir duquel chercher le token
** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
** \return Retourne la taille du token si on le trouve, 0 sinon
*/
unsigned long    BNFcalcul::is_udecimal(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                     const IASM *, unsigned long off, int save)
{
    unsigned long    i;

    if (off >= s.size())
        return (0);

    i = 0;
    while ((s[off + i] >= '0') && (s[off + i] <= '9'))
        i++;

    /* Sauvegarde du token dans la map si besoin est et renvoi de la taille du token */
    if ((i > 0) && (save > 0) && (m != NULL))
        (*m)[BNFcalcul::TYPE_UDECIMAL] = s.substr(off, i);
    return (i);
}

/**
** \fn unsigned long    is_hexadecimal(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
**                                     const IASM *, unsigned long off, int save)
** \brief Fonction de la BNF verifiant si la chaine contient un hexadecimal valide
**
** \param s String a analyser (sans espaces ni retour a la ligne)
** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
** \param off Offset a partir duquel chercher le token
** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
** \return Retourne la taille du token si on le trouve, 0 sinon
*/
unsigned long    BNFcalcul::is_hexadecimal(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                        const IASM *, unsigned long off, int save)
{
    unsigned long    i;

    if (off >= s.size())
        return (0);

    if (s[off] != '0')
        return (0);
    if (s[off + 1] != 'x')
        return (0);

    /* Sauvegarde du token dans la map si besoin est et renvoi de la taille du token */
    if ((i = BNFcalcul::is_hexa_sans_0x(s, m, NULL, off+2, 0)) > 0)
    {
        if ((save > 0) && (m != NULL))
            (*m)[BNFcalcul::TYPE_0xHEXA] = s.substr(off, 2+i);
        return (2+i);
    }
    return (0);
}

/**
** \fn unsigned long    is_hexa_sans_0x(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
**                                      const IASM *, unsigned long off=0, int save=0)
** \brief Fonction de la BNF verifiant si la chaine contient un hexa_sans_0x valide
**
** \param s String a analyser (sans espaces ni retour a la ligne)
** \param m Map ou mettre le token si on le trouve et si le parametre "save" > 0
** \param off Offset a partir duquel chercher le token
** \param save Doit etre > a 0 si on veut enregistrer le token dans la map (est decremente lors des recherches de sous-tokens)
** \return Retourne la taille du token si on le trouve, 0 sinon
*/
unsigned long    BNFcalcul::is_hexa_sans_0x(const std::string &s, std::map<BNFcalcul::eTypeToken, std::string> *m,
                                         const IASM *, unsigned long off, int save)
{
    unsigned long    i;

    if (off >= s.size())
        return (0);

    i = 0;
    while (((s[off + i] >= '0') && (s[off + i] <= '9')) ||
           ((s[off + i] >= 'a') && (s[off + i] <= 'f')) ||
           ((s[off + i] >= 'A') && (s[off + i] <= 'F')))
        i++;

    /* Sauvegarde du token dans la map si besoin est et renvoi de la taille du token */
    if ((i > 0) && (save > 0) && (m != NULL))
        (*m)[BNFcalcul::TYPE_HEXA] = s.substr(off, i);
    return (i);
}
                                         
