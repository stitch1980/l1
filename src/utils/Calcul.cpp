/*
** Calcul.cpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 14:34:37 joran herve
** Last update Sat, 27 Jun 2015 14:34:37 joran herve
**
** Status : Pas totalement teste (Et peut largement etre optimise)
*/

#include    "Calcul.hpp"


/*
** Les fonctions de calcul et d'estimations des resultats permettant d'evaluer
** les calculs arithmethiques et logiques
*/
long    Calcul::calcul(const std::string &expr, const Info *info, const Contexte *ctx)
{
    std::string   dest;

    Calcul::calcul_str(dest, expr, info, ctx);
    if (dest.size() < 3)
        return (0);
    return (Calcul::Oxtol(dest.c_str()));
}

std::string    Calcul::calcul_str(const std::string &expr, const Info *info, const Contexte *ctx)
{
    std::string   dest;
    return (Calcul::calcul_str(dest, expr, info, ctx));
}

std::string    &Calcul::calcul_str(std::string &dest, const std::string &expr, const Info *info, const Contexte *ctx)
{
    std::set<std::string>    set_dest;

    Calcul::evaluate(set_dest, expr, info, ctx);
    if (set_dest.empty())
        dest = "0x0";
    else
        dest = *(set_dest.begin());

    return (dest);
}

std::set<std::string>    Calcul::evaluate(const std::string &expr, const Info *info, const Contexte *ctx)
{
    std::set<std::string>    set_dest;
    return (Calcul::evaluate(set_dest, expr, info, ctx));
}

std::set<std::string>    &Calcul::evaluate(std::set<std::string> &dest, const std::string &expr, const Info *info, const Contexte *ctx)
{
    dest.clear();
    return (evaluate_add_result(dest, expr, info, ctx));
}

std::set<std::string>    &Calcul::evaluate_add_result(std::set<std::string> &dest, const std::string &expr, const Info *info, const Contexte *ctx)
{
    std::map<BNFcalcul::eTypeToken, std::string>    m;
    std::list<Calcul::NodeCalcul*>                  npi;
    Calcul::NodeCalcul                              *result;

    if (info != NULL)
    {
        /* Une petite verification permet d'eviter les mauvaises surprises et de gerer les ternaires */
        if (BNFcalcul::is_expression(expr, &m, info->ptr_func.deasm, 0, 1) > 0)
        {
            /* Evaluation de la condition si c'est un ternaire */
            if ((m.find(BNFcalcul::TYPE_TERNAIRE_TEST) != m.end()) &&
                (m.find(BNFcalcul::TYPE_TERNAIRE_TEST)->second.size() > 0))
            {
                std::set<std::string>    result_condition;
                int                      choice;

                Calcul::evaluate_add_result(result_condition, m[BNFcalcul::TYPE_TERNAIRE_TEST], info, ctx);

                /* Choix du calcul a effecuter en fonction des resultats */
                choice = 0;
                if (result_condition.size() == 0)
                    choice = 0x3;
                else
                {
                    if (result_condition.size() > 1)
                        choice = choice | 0x1;
                    if (result_condition.find("0x0") != result_condition.end())
                        choice = choice | 0x2;
                    else
                        choice = choice | 0x1;
                }

                /* Ajout des possiblites du choix 1 dans le set si besoin est */
                if ((choice & 0x1) == 0x1)
                    Calcul::evaluate_add_result(dest, m[BNFcalcul::TYPE_TERNAIRE_CHOIX_1], info, ctx);
                /* Ajout des possiblites du choix 2 dans le set si besoin est */
                if ((choice & 0x2) == 0x2)
                    Calcul::evaluate_add_result(dest, m[BNFcalcul::TYPE_TERNAIRE_CHOIX_2], info, ctx);

            }

            /* Eveluation du calcul si c'est un calcul normal */
            else if (Calcul::make_npi(m[BNFcalcul::TYPE_EXPRESSION], info, ctx, npi) > 0)
            {
                if ((result = Calcul::resolve_NPI(npi)) != NULL)
                    dest.insert(result->_values.begin(), result->_values.end());
            }

            Calcul::clear_npi(npi);
        }
    }

    return (dest);
}

/**
** \fn int make_npi(const std::string &expr, const Info *info,
**                  const Contexte *ctx, std::vector<std::string> &npi)
** \brief Gere la creation d'un NPI a partir d'une expression
**
** \param expr Expression a resoudre
** \param info Structure contenant les infos du programme
** \param ctx Contexte d'execution au moment du calcul
** \param npi Vector devant contenir la NPI
** \return Retourne 1 si OK, 0 sinon
*/
int              Calcul::make_npi(const std::string &expr, const Info *info, const Contexte *ctx, std::list<Calcul::NodeCalcul*> &npi)
{
    std::list<Calcul::NodeCalcul*>                  pile;
    Calcul::NodeCalcul                              *node;
    std::set<std::string>                           values_tmp;
    std::string                                     oper_tmp;
    unsigned long                                   off;
    unsigned long                                   tmp;
    unsigned long                                   operator_before;

    npi.clear();

    /* On parse l'expression tant que l'on est pas a sa fin */
    off = 0;
    operator_before = 1;
    while (off < expr.size())
    {
        /* Nombre hexadecimal */
        if ((tmp = BNFcalcul::is_hexadecimal(expr, NULL, NULL, off, 0)) > 0)
        {
            if ((node = new NodeCalcul(NULL, expr.substr(off, tmp))) == NULL)
                return (0);
            node->_name = *(node->_values.begin());

            npi.push_back(node);
            off += tmp;
            operator_before = 0;
        }

        /* Nombre decimal */
        else if ((operator_before != 0) &&
                 (tmp = BNFcalcul::is_decimal(expr, NULL, NULL, off, 0)) > 0)
        {
            if ((node = new NodeCalcul(NULL, Calcul::lto0x(strtol(expr.substr(off, tmp).c_str(), NULL, 10)) )) == NULL)
                return (0);
            node->_name = *(node->_values.begin());

            npi.push_back(node);
            off += tmp;
            operator_before = 0;
        }

        /* Registre ou case memoire */
        else if ((tmp = BNFcalcul::is_dest(expr, NULL, info->ptr_func.deasm, off, 0)) > 0)
        {
            if (ctx == NULL)
            {
                if ((node = new NodeCalcul(NULL, "0x0")) == NULL)
                    return (0);
                node->_name = *(node->_values.begin());

                npi.push_back(node);
            }
            else
            {
                if ((node = new NodeCalcul(NULL, ctx->get_values(values_tmp, expr.substr(off, tmp), info))) == NULL)
                    return (0);
                node->_name = expr.substr(off, tmp);

                npi.push_back(node);
            }
            off += tmp;
            operator_before = 0;
        }

        /* Parenthese ouvrante */
        else if (expr[off] == '(')
        {
            if ((node = new NodeCalcul(Calcul::NodeCalcul::TYPE_PAR)) == NULL)
                return (0);

            pile.push_back(node);
            off += 1;
            operator_before = 1;
        }

        /* Parenthese fermante */
        else if (expr[off] == ')')
        {
            while ((pile.size() > 0) && (pile.back()->_type != Calcul::NodeCalcul::TYPE_PAR))
            {
                npi.push_back(pile.back());
                pile.pop_back();
            }
            if (pile.size() > 0)
            {
                delete pile.back();
                pile.pop_back();
            }
            off += 1;
            operator_before = 0;
        }

        /* Operateur */
        else if ((tmp = BNFcalcul::is_operateur(expr, NULL, info->ptr_func.deasm, off, 0)) > 0)
        {
            oper_tmp = expr.substr(off, tmp);

            /* Tant qu'il y a des operateurs prioritaires sur la pile, on les met sur la NPI */
            while ((pile.size() > 0) &&
                   (pile.back()->_type != Calcul::NodeCalcul::TYPE_PAR) &&
                   (pile.back()->_priority >= Calcul::get_priority_oper(&(expr[off]))) )
            {
                npi.push_back(pile.back());
                pile.pop_back();
            }

            node = NULL;
            if (oper_tmp == "*")
                node = new NodeCalcul(&do_oper_mul, NULL);
            else if (oper_tmp == "/")
                node = new NodeCalcul(&do_oper_div, NULL);
            else if (oper_tmp == "%")
                node = new NodeCalcul(&do_oper_mod, NULL);
            else if (oper_tmp == "+")
                node = new NodeCalcul(&do_oper_add, NULL);
            else if (oper_tmp == "-")
                node = new NodeCalcul(&do_oper_sub, NULL);
            else if (oper_tmp == "<<")
                node = new NodeCalcul(&do_oper_shl, NULL);
            else if (oper_tmp == ">>")
                node = new NodeCalcul(&do_oper_shr, NULL);
            else if (oper_tmp == "&")
                node = new NodeCalcul(&do_oper_and, NULL);
            else if (oper_tmp == "^")
                node = new NodeCalcul(&do_oper_xor, NULL);
            else if (oper_tmp == "|")
                node = new NodeCalcul(&do_oper_or, NULL);
            else if (oper_tmp == "==")
                node = new NodeCalcul(&do_oper_eg, NULL);
            else if (oper_tmp == "!=")
                node = new NodeCalcul(&do_oper_dif, NULL);
            else if (oper_tmp == "<")
                node = new NodeCalcul(&do_oper_lo, NULL);
            else if (oper_tmp == "<=")
                node = new NodeCalcul(&do_oper_le, NULL);
            else if (oper_tmp == ">")
                node = new NodeCalcul(&do_oper_gr, NULL);
            else if (oper_tmp == ">=")
                node = new NodeCalcul(&do_oper_ge, NULL);
            else if (oper_tmp == "&&")
                node = new NodeCalcul(&do_oper_andand, NULL);
            else if (oper_tmp == "||")
                node = new NodeCalcul(&do_oper_oror, NULL);

            if (node == NULL)
               return (0);
            node->_priority = Calcul::get_priority_oper(oper_tmp.c_str());

            /* Mettre l'operateur sur la pile */
            pile.push_back(node);
            off += tmp;
            operator_before = 1;
        }

        /* En cas d'erreur, on arrete */
        else
        {
            fprintf(stderr, "ERROR NPI \"%s\"\n", expr.substr(off, -1).c_str());
            return (0);
        }
    }

    /* Depiler la pile en mettant tout les operateurs restant sur la NPI */
    while (pile.size() > 0)
    {
        if (pile.back()->_type != Calcul::NodeCalcul::TYPE_PAR)
            npi.push_back(pile.back());
        else
            delete pile.back();

        pile.pop_back();
    }

    return (1);
}

/**
** \fn Calcul::NodeCalcul *resolve_NPI(std::list<Calcul::NodeCalcul*> &npi)
** \brief Gere l'estimation des resultats d'une NPI (elle doit etre valide)
**
** \param npi NPI a resoudre
** \return Retourne un node contenant les resultats de la NPI
*/
Calcul::NodeCalcul    *Calcul::resolve_NPI(std::list<Calcul::NodeCalcul*> &npi)
{
    std::list<Calcul::NodeCalcul*>::iterator    it_oper;
    std::list<Calcul::NodeCalcul*>::iterator    it_n1;
    std::list<Calcul::NodeCalcul*>::iterator    it_n2;
/*printf("NPI= ");
for (std::list<Calcul::NodeCalcul*>::iterator it=npi.begin(); it!=npi.end(); it++)
{
 if ((*it)->_type == Calcul::NodeCalcul::TYPE_OPER)
    printf(".oper. ");
 else if (((*it)->_type == Calcul::NodeCalcul::TYPE_NBS) && ((*it)->_values.size() > 0))
    printf(".%s. ", (*((*it)->_values.begin())).c_str());
    }
printf("\n");*/
    it_oper = npi.begin();
    it_n1 = npi.begin();
    it_n2 = npi.begin();
    while ((npi.size() >= 3) && (it_oper != npi.end()))
    {
        /* Si on arrive a un calcul */
        if (((*it_oper)->_type == Calcul::NodeCalcul::TYPE_OPER) &&
            ((*it_n1)->_type == Calcul::NodeCalcul::TYPE_NBS) &&
            ((*it_n2)->_type == Calcul::NodeCalcul::TYPE_NBS) &&
            (it_n1 != it_n2))
        {
            /* Execute le calcul */
            Calcul::do_oper((*it_oper), (*it_n1), (*it_oper), (*it_n2));

            /* Supprime les operandes (l'operateur est devenu le resultat */
            delete (*it_n1);
            delete (*it_n2);
            npi.erase(it_n1);
            npi.erase(it_n2);

            /* Retourne au debut de la NPI */
            it_oper = npi.begin();
            it_n1 = npi.begin();
            it_n2 = npi.begin();
        }

        /* Sinon, on avance dans la npi */
        else
        {
            it_n1 = it_n2;
            it_n2 = it_oper;
            it_oper++;
        }
    }

    /* Retourne le resultat de la NPI */
    if (npi.begin() == npi.end())
        return (NULL);
    return (*(npi.begin()));
}

/**
** \fn void clear_npi(std::list<Calcul::NodeCalcul*> &npi)
** \brief Gere la suppression de tout les Nodes d'une NPI
**
** \brief npi NPI a vider
** \return Retourne rien
*/
void    Calcul::clear_npi(std::list<Calcul::NodeCalcul*> &npi)
{
    for (std::list<Calcul::NodeCalcul*>::iterator it=npi.begin();
         it!=npi.end();
         it++)
        delete (*it);
    npi.clear();
}




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
Calcul::NodeCalcul    *Calcul::do_oper(NodeCalcul *dest, const NodeCalcul *n1, const NodeCalcul *oper, const NodeCalcul *n2)
{
    if (dest != NULL)
    {
        if ((n1 != NULL) && (oper != NULL) && (n2 != NULL) &&
            (oper->_func != NULL))
            oper->_func(dest->_values, n1, n2);

        dest->_type = Calcul::NodeCalcul::TYPE_NBS;
        dest->_func = NULL;
    }
    return (dest);
}



/*
** Ces fonctions permettent de d'estimer les resultats possibles d'un calcul simple.
** Les parametres "n1" et "n2" sont les operandes et "dest" est le resultat.
**
** Certaines d'entre elles sont optimisees afin de diminuer les faux positifs
** Si un des operandes ou le resultat ont plus de MAX_RESULT_SET possibilites,
** ils sont vide afin de signifier qu'il sont totalement innconu. Cela permet
** de limiter le temps de calcul et la memoire utilisee
*/
std::set<std::string>    &Calcul::do_oper_mul(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    std::string    str;

    dest.clear();

    /* Si une des deux valeurs est inconnu, le resultat sera inconnu */
    if (!n1->_values.empty() && !n2->_values.empty())
    {
        /* Sinon, on fait tout les calculs possibles */
        for (std::set<std::string>::const_iterator it1=n1->_values.begin();
             it1!=n1->_values.end();
             it1++)
        {
            for (std::set<std::string>::const_iterator it2=n2->_values.begin();
                 it2!=n2->_values.end();
                 it2++)
            {
                /* Fais le calcul */
                Calcul::lto0x(str, Calcul::Oxtol((*it1).c_str()) * Calcul::Oxtol((*it2).c_str()));
                dest.insert(str);

                /* Si le nombre de resultats est trop eleve, on arrete */
                if (dest.size() > MAX_RESULT_SET)
                {
                    dest.clear();
                    return (dest);
                }
            }
        }
    }

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_div(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    std::string    str;

    dest.clear();

    /* Si une des deux valeurs est inconnu, le resultat sera inconnu */
    if (!n1->_values.empty() && !n2->_values.empty())
    {
        /* Sinon, on fait tout les calculs possibles */
        for (std::set<std::string>::const_iterator it1=n1->_values.begin();
             it1!=n1->_values.end();
             it1++)
        {
            for (std::set<std::string>::const_iterator it2=n2->_values.begin();
                 it2!=n2->_values.end();
                 it2++)
            {
                /* Evite les divisions par 0 */
                if ((*it2) != "0x0")
                {
                    /* Fais le calcul */
                    Calcul::lto0x(str, Calcul::Oxtol((*it1).c_str()) / Calcul::Oxtol((*it2).c_str()));
                    dest.insert(str);

                    /* Si le nombre de resultats est trop eleve, on arrete */
                    if (dest.size() > MAX_RESULT_SET)
                    {
                        dest.clear();
                        return (dest);
                    }
                }
            }
        }
    }

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_mod(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    std::set<unsigned long>    list_n2;
    unsigned long              mod_max;
    std::string                str;

    dest.clear();

    /* Si la premiere valeur est inconnu mais la deuxieme est connu, il y a moyen de gerer */
    if (n1->_values.empty() && !n2->_values.empty())
    {
        /* On cree la liste de valeurs possibles pour le deuxieme operandes */
        for (std::set<std::string>::const_iterator it=n2->_values.begin();
             it!=n2->_values.end();
             it++)
            list_n2.insert( labs(Calcul::Oxtol((*it).c_str())) );

        /* Si le deuxieme operande n'est pas trop grand */
        mod_max = *(list_n2.rbegin());
        if (mod_max < MAX_RESULT_SET)
        {
            for (unsigned long i=0; i<mod_max; i++)
            {
                Calcul::lto0x(str, i);
                dest.insert(str);
            }
        }
    }

    /* Si les deux valeurs sont inconnus, le resultat sera inconnu */
    else if (!n1->_values.empty() && !n2->_values.empty())
    {
        /* Sinon, on fait tout les calculs possibles */
        for (std::set<std::string>::const_iterator it1=n1->_values.begin();
             it1!=n1->_values.end();
             it1++)
        {
            for (std::set<std::string>::const_iterator it2=n2->_values.begin();
                 it2!=n2->_values.end();
                 it2++)
            {
                /* Evite les divisions par 0 */
                if ((*it2) != "0x0")
                {
                    /* Fais le calcul */
                    Calcul::lto0x(str, Calcul::Oxtol((*it1).c_str()) % Calcul::Oxtol((*it2).c_str()));
                    dest.insert(str);

                    /* Si le nombre de resultats est trop eleve, on arrete */
                    if (dest.size() > MAX_RESULT_SET)
                    {
                        dest.clear();
                        return (dest);
                    }
                }
            }
        }
    }

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_add(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    std::string    str;

    dest.clear();

    /* Si une des deux valeurs est inconnu, le resultat sera inconnu */
    if (!n1->_values.empty() && !n2->_values.empty())
    {
        /* Sinon, on fait tout les calculs possibles */
        for (std::set<std::string>::const_iterator it1=n1->_values.begin();
             it1!=n1->_values.end();
             it1++)
        {
            for (std::set<std::string>::const_iterator it2=n2->_values.begin();
                 it2!=n2->_values.end();
                 it2++)
            {
                /* Fais le calcul */
                Calcul::lto0x(str, Calcul::Oxtol((*it1).c_str()) + Calcul::Oxtol((*it2).c_str()));
                dest.insert(str);

                /* Si le nombre de resultats est trop eleve, on arrete */
                if (dest.size() > MAX_RESULT_SET)
                {
                    dest.clear();
                    return (dest);
                }
            }
        }
    }

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_sub(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    std::string    str;

    dest.clear();

    /* Si une des deux valeurs est inconnu, le resultat sera inconnu */
    if (!n1->_values.empty() && !n2->_values.empty())
    {
        /* Sinon, on fait tout les calculs possibles */
        for (std::set<std::string>::const_iterator it1=n1->_values.begin();
             it1!=n1->_values.end();
             it1++)
        {
            for (std::set<std::string>::const_iterator it2=n2->_values.begin();
                 it2!=n2->_values.end();
                 it2++)
            {
                /* Fais le calcul */
                Calcul::lto0x(str, Calcul::Oxtol((*it1).c_str()) - Calcul::Oxtol((*it2).c_str()));
                dest.insert(str);

                /* Si le nombre de resultats est trop eleve, on arrete */
                if (dest.size() > MAX_RESULT_SET)
                {
                    dest.clear();
                    return (dest);
                }
            }
        }
    }

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_shr(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    std::string    str;

    dest.clear();

    /* La deuxieme operande est inconnue, il est possible d'estimer les resultats possibles */
    if (!n1->_values.empty() && n2->_values.empty())
    {
        for (std::set<std::string>::const_iterator it1=n1->_values.begin();
             it1!=n1->_values.end();
             it1++)
        {
            for (unsigned long i=0;
                 i<65;
                 i++)
            {
                /* Fais le calcul */
                Calcul::lto0x(str, Calcul::Oxtol((*it1).c_str()) >> i);
                dest.insert(str);

                /* Si le nombre de resultats est trop eleve, on arrete */
                if (dest.size() > MAX_RESULT_SET)
                {
                    dest.clear();
                    return (dest);
                }
            }
        }
    }

    /* Si les deux operandes sont connues, on peur calculer les resultats possible */
    else if (!n1->_values.empty() && !n2->_values.empty())
    {
        /* On fait tout les calculs possibles */
        for (std::set<std::string>::const_iterator it1=n1->_values.begin();
             it1!=n1->_values.end();
             it1++)
        {
            for (std::set<std::string>::const_iterator it2=n2->_values.begin();
                 it2!=n2->_values.end();
                 it2++)
            {
                /* Fais le calcul */
                Calcul::lto0x(str, Calcul::Oxtol((*it1).c_str()) >> Calcul::Oxtol((*it2).c_str()));
                dest.insert(str);

                /* Si le nombre de resultats est trop eleve, on arrete */
                if (dest.size() > MAX_RESULT_SET)
                {
                    dest.clear();
                    return (dest);
                }
            }
        }
    }

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_shl(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    std::string    str;

    dest.clear();

    /* La deuxieme operande est inconnue, il est possible d'estimer les resultats possibles */
    if (!n1->_values.empty() && n2->_values.empty())
    {
        for (std::set<std::string>::const_iterator it1=n1->_values.begin();
             it1!=n1->_values.end();
             it1++)
        {
            for (unsigned long i=0;
                 i<65;
                 i++)
            {
                /* Fais le calcul */
                Calcul::lto0x(str, Calcul::Oxtol((*it1).c_str()) << i);
                dest.insert(str);

                /* Si le nombre de resultats est trop eleve, on arrete */
                if (dest.size() > MAX_RESULT_SET)
                {
                    dest.clear();
                    return (dest);
                }
            }
        }
    }

    /* Si les deux operandes sont connues, on peur calculer les resultats possible */
    else if (!n1->_values.empty() && !n2->_values.empty())
    {
        /* On fait tout les calculs possibles */
        for (std::set<std::string>::const_iterator it1=n1->_values.begin();
             it1!=n1->_values.end();
             it1++)
        {
            for (std::set<std::string>::const_iterator it2=n2->_values.begin();
                 it2!=n2->_values.end();
                 it2++)
            {
                /* Fais le calcul */
                Calcul::lto0x(str, Calcul::Oxtol((*it1).c_str()) << Calcul::Oxtol((*it2).c_str()));
                dest.insert(str);

                /* Si le nombre de resultats est trop eleve, on arrete */
                if (dest.size() > MAX_RESULT_SET)
                {
                    dest.clear();
                    return (dest);
                }
            }
        }
    }

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_and(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    std::string                    str;

    dest.clear();

    /* Si les deux operandes sont identiques le resultat est toujours egal aux valeurs de l'operande */
    if ((n1->_name.size() > 0) && (n1->_name == n2->_name))
        dest = n1->_values;

    /* Si les valeurs d'un seul operandes sont connus, on peut gerer */
    else if ((n1->_values.empty() && !n2->_values.empty()) ||
             (!n1->_values.empty() && n2->_values.empty()))
        return (Calcul::gestion_masque_oper_and(dest, n1, n2));

    /* Si une des deux valeurs est inconnu, le resultat sera inconnu */
    else if (!n1->_values.empty() && !n2->_values.empty())
    {
        /* Sinon, on fait tout les calculs possibles */
        for (std::set<std::string>::const_iterator it1=n1->_values.begin();
             it1!=n1->_values.end();
             it1++)
        {
            for (std::set<std::string>::const_iterator it2=n2->_values.begin();
                 it2!=n2->_values.end();
                 it2++)
            {
                /* Fais le calcul */
                Calcul::lto0x(str, Calcul::Oxtol((*it1).c_str()) & Calcul::Oxtol((*it2).c_str()));
                dest.insert(str);

                /* Si le nombre de resultats est trop eleve, on arrete */
                if (dest.size() > MAX_RESULT_SET)
                {
                    dest.clear();
                    return (dest);
                }
            }
        }
    }

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_xor(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    std::string    str;

    dest.clear();

    /* Si les deux operandes sont identiques le resultat est toujours 0 */
    if ((n1->_name.size() > 0) && (n1->_name == n2->_name))
        dest.insert("0x0");

    /* Si une des deux valeurs est inconnu, le resultat sera inconnu */
    else if (!n1->_values.empty() && !n2->_values.empty())
    {
        /* Sinon, on fait tout les calculs possibles */
        for (std::set<std::string>::const_iterator it1=n1->_values.begin();
             it1!=n1->_values.end();
             it1++)
        {
            for (std::set<std::string>::const_iterator it2=n2->_values.begin();
                 it2!=n2->_values.end();
                 it2++)
            {
                /* Fais le calcul */
                Calcul::lto0x(str, Calcul::Oxtol((*it1).c_str()) ^ Calcul::Oxtol((*it2).c_str()));
                dest.insert(str);

                /* Si le nombre de resultats est trop eleve, on arrete */
                if (dest.size() > MAX_RESULT_SET)
                {
                    dest.clear();
                    return (dest);
                }
            }
        }
    }

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_or(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    std::string    str;

    dest.clear();

    /* Si les valeurs d'un seul operandes sont connus, on peut gerer */
    if ((n1->_values.empty() && !n2->_values.empty()) ||
        (!n1->_values.empty() && n2->_values.empty()))
        return (Calcul::gestion_masque_oper_or(dest, n1, n2));

    /* Si une des deux valeurs est inconnu, le resultat sera inconnu */
    if (!n1->_values.empty() && !n2->_values.empty())
    {
        /* Sinon, on fait tout les calculs possibles */
        for (std::set<std::string>::const_iterator it1=n1->_values.begin();
             it1!=n1->_values.end();
             it1++)
        {
            for (std::set<std::string>::const_iterator it2=n2->_values.begin();
                 it2!=n2->_values.end();
                 it2++)
            {
                /* Fais le calcul */
                Calcul::lto0x(str, Calcul::Oxtol((*it1).c_str()) | Calcul::Oxtol((*it2).c_str()));
                dest.insert(str);

                /* Si le nombre de resultats est trop eleve, on arrete */
                if (dest.size() > MAX_RESULT_SET)
                {
                    dest.clear();
                    return (dest);
                }
            }
        }
    }

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_eg(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    dest.clear();

    /* Si une des deux valeurs est inconnu, le resultat sera inconnu */
    if ((n1->_values.empty()) || (n2->_values.empty()) ||
        (n1->_values.size() > 1) || (n2->_values.size() > 1))
    {
        dest.insert("0x0");
        dest.insert("0x1");
    }

    /* Sinon, on peut comparer les valeurs */
    else if ( Calcul::Oxtol((*(n1->_values.begin())).c_str()) == Calcul::Oxtol((*(n2->_values.begin())).c_str()) )
        dest.insert("0x1");
    else
        dest.insert("0x0");

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_dif(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    dest.clear();

    /* Si une des deux valeurs est inconnu, le resultat sera inconnu */
    if ((n1->_values.empty()) || (n2->_values.empty()) ||
        (n1->_values.size() > 1) || (n2->_values.size() > 1))
    {
        dest.insert("0x0");
        dest.insert("0x1");
    }

    /* Sinon, on peut comparer les valeurs */
    else if ( Calcul::Oxtol((*(n1->_values.begin())).c_str()) != Calcul::Oxtol((*(n2->_values.begin())).c_str()) )
        dest.insert("0x1");
    else
        dest.insert("0x0");

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_lo(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    std::set<long>              list_n1;
    std::set<long>              list_n2;

    dest.clear();

    /* Si une des deux valeurs est inconnu, le resultat sera inconnu */
    if (n1->_values.empty() || n2->_values.empty())
    {
        dest.insert("0x0");
        dest.insert("0x1");
    }

    /* Sinon on utilise une methode optimisee pour deduire les possibilites de resultat */
    else
    {
        /* On cree des listes de valeurs possibles pour les deux operandes */
        for (std::set<std::string>::const_iterator it=n1->_values.begin();
             it!=n1->_values.end();
             it++)
            list_n1.insert( Calcul::Oxtol((*it).c_str()) );

        for (std::set<std::string>::const_iterator it=n2->_values.begin();
             it!=n2->_values.end();
             it++)
            list_n2.insert( Calcul::Oxtol((*it).c_str()) );

        /* On deduit les possibilites de resultat en comparant les extremes des possibilites de valeurs */
        if (*(list_n1.begin()) >= *(list_n2.rbegin()))
            dest.insert("0x0");
        if (*(list_n1.rbegin()) < *(list_n2.begin()))
            dest.insert("0x1");
    }

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_le(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    std::set<long>              list_n1;
    std::set<long>              list_n2;

    dest.clear();

    /* Si une des deux valeurs est inconnu, le resultat sera inconnu */
    if (n1->_values.empty() || n2->_values.empty())
    {
        dest.insert("0x0");
        dest.insert("0x1");
    }

    /* Sinon on utilise une methode optimisee pour deduire les possibilites de resultat */
    else
    {
        /* On cree des listes de valeurs possibles pour les deux operandes */
        for (std::set<std::string>::const_iterator it=n1->_values.begin();
             it!=n1->_values.end();
             it++)
            list_n1.insert( Calcul::Oxtol((*it).c_str()) );

        for (std::set<std::string>::const_iterator it=n2->_values.begin();
             it!=n2->_values.end();
             it++)
            list_n2.insert( Calcul::Oxtol((*it).c_str()) );

        /* On deduit les possibilites de resultat en comparant les extremes des possibilites de valeurs */
        if (*(list_n1.begin()) > *(list_n2.rbegin()))
            dest.insert("0x0");
        if (*(list_n1.rbegin()) <= *(list_n2.begin()))
            dest.insert("0x1");
    }

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_gr(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    std::set<long>              list_n1;
    std::set<long>              list_n2;

    dest.clear();

    /* Si une des deux valeurs est inconnu, le resultat sera inconnu */
    if (n1->_values.empty() || n2->_values.empty())
    {
        dest.insert("0x0");
        dest.insert("0x1");
    }

    /* Sinon on utilise une methode optimisee pour deduire les possibilites de resultat */
    else
    {
        /* On cree des listes de valeurs possibles pour les deux operandes */
        for (std::set<std::string>::const_iterator it=n1->_values.begin();
             it!=n1->_values.end();
             it++)
            list_n1.insert( Calcul::Oxtol((*it).c_str()) );

        for (std::set<std::string>::const_iterator it=n2->_values.begin();
             it!=n2->_values.end();
             it++)
            list_n2.insert( Calcul::Oxtol((*it).c_str()) );

        /* On deduit les possibilites de resultat en comparant les extremes des possibilites de valeurs */
        if (*(list_n1.begin()) <= *(list_n2.rbegin()))
            dest.insert("0x0");
        if (*(list_n1.rbegin()) > *(list_n2.begin()))
            dest.insert("0x1");
    }

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_ge(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    std::set<long>              list_n1;
    std::set<long>              list_n2;

    dest.clear();

    /* Si une des deux valeurs est inconnu, le resultat sera inconnu */
    if (n1->_values.empty() || n2->_values.empty())
    {
        dest.insert("0x0");
        dest.insert("0x1");
    }

    /* Sinon on utilise une methode optimisee pour deduire les possibilites de resultat */
    else
    {
        /* On cree des listes de valeurs possibles pour les deux operandes */
        for (std::set<std::string>::const_iterator it=n1->_values.begin();
             it!=n1->_values.end();
             it++)
            list_n1.insert( Calcul::Oxtol((*it).c_str()) );

        for (std::set<std::string>::const_iterator it=n2->_values.begin();
             it!=n2->_values.end();
             it++)
            list_n2.insert( Calcul::Oxtol((*it).c_str()) );

        /* On deduit les possibilites de resultat en comparant les extremes des possibilites de valeurs */
        if (*(list_n1.begin()) < *(list_n2.rbegin()))
            dest.insert("0x0");
        if (*(list_n1.rbegin()) >= *(list_n2.begin()))
            dest.insert("0x1");
    }

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_andand(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    std::string    str;

    dest.clear();

    /* Si on connait toutes les valeurs possibles */
    if (!n1->_values.empty() && !n2->_values.empty())
    {
        /* Si les deux nombres ne peuvent pas valoir 0, le resultat vaudra toujours 1 */
        if ((n1->_values.find("0x0") == n1->_values.end()) && (n2->_values.find("0x0") == n2->_values.end()))
            dest.insert("0x1");

        /* S'il n'y a qu'une possibilite et que c'est 0, le resultat vaudra toujours 0 */
        else if ( ((n1->_values.size() == 1) && (n1->_values.find("0x0") == n1->_values.begin())) ||
                  ((n2->_values.size() == 1) && (n2->_values.find("0x0") == n2->_values.begin())) )
            dest.insert("0x0");

        else
        {
            dest.insert("0x0");
            dest.insert("0x1");
        }
    }
    /* Si on ne connais pas les valeurs, tout est possible */
    else
    {
        dest.insert("0x0");
        dest.insert("0x1");
    }

    return (dest);
}

std::set<std::string>    &Calcul::do_oper_oror(std::set<std::string> &dest, const Calcul::NodeCalcul *n1, const Calcul::NodeCalcul *n2)
{
    std::string    str;

    dest.clear();

    /* Si on connait toutes les valeurs possibles */
    if (!n1->_values.empty() && !n2->_values.empty())
    {
        /* Si un des nombres ne peut pas valoir 0, le resultat vaudra toujours 1 */
        if ((n1->_values.find("0x0") == n1->_values.end()) || (n2->_values.find("0x0") == n2->_values.end()))
            dest.insert("0x1");

        /* S'il n'y a qu'une possibilite et que c'est 0, le resultat vaudra toujours 0 */
        else if ( (n1->_values.size() == 1) && (n1->_values.find("0x0") == n1->_values.begin()) &&
                  (n2->_values.size() == 1) && (n2->_values.find("0x0") == n2->_values.begin()) )
            dest.insert("0x0");

        else
        {
            dest.insert("0x0");
            dest.insert("0x1");
        }
    }
    /* Si on ne connais pas les valeurs, tout est possible */
    else
    {
        dest.insert("0x0");
        dest.insert("0x1");
    }

    return (dest);
}




/*
** Ces fonctions permettent d'optimiser l'estimation des resultats d'un AND-logique
** si un des operandes est inconnu.
**
** Elles utilisent des masques pour determiner les bits qui seront toujours a 0
*/
std::set<std::string>    &Calcul::gestion_masque_oper_and(std::set<std::string> &dest,
                                                          const Calcul::NodeCalcul *n1,
                                                          const Calcul::NodeCalcul *n2)
{
    const std::set<std::string>    *ptr_list;
    std::string                    str;
    unsigned long                  value_operande;
    unsigned long                  nbs_bit_a_1;
    unsigned long                  masque[64];
    int                            ret;

    /* Choisi l'operande servant pour le masque */
    if (!n1->_values.empty())
        ptr_list = &(n1->_values);
    else
        ptr_list = &(n2->_values);

    /* Pour toutes les valeurs possibles de cet operande... */
    for (std::set<std::string>::const_iterator it=ptr_list->begin();
         it!=ptr_list->end();
         it++)
    {
        value_operande = Calcul::Oxtol((*it).c_str());

        /* Prepare le masque et compte les bits a 1 */
        nbs_bit_a_1 = 0;
        for (unsigned long i=0; i<64; i++)
        {
            if (((value_operande >> i) & 1) == 1)
                nbs_bit_a_1++;
            masque[i] = (value_operande >> i) & 1;
        }

        /* S'il y a plus de 8 bits a 1, ca fait trop de possibilites */
        if (nbs_bit_a_1 > 8)
        {
            dest.clear();
            return (dest);
        }

        /* Genere des valeurs a partir du masque */
        do
        {
            value_operande = 0;
            for (unsigned long i=0; i<64; i++)
            {
                if (masque[i] == 2)
                    value_operande = value_operande | (1 << i);
            }

            dest.insert(Calcul::lto0x(str, value_operande));
        }
        while ((ret = Calcul::gestion_masque_oper_and_rec(masque, dest, 0)) == 0);

        /* Si la gestion de masque retourne -1, cela signifie qu'on a trouve trop de possibilites */
        if (ret < 0)
        {
            dest.clear();
            return (dest);
        }
    }

    return (dest);
}

int    Calcul::gestion_masque_oper_and_rec(unsigned long masque[64],
                                           std::set<std::string> &dest,
                                           unsigned long bit)
{
    /* S'il y a trop de possibilites, on arrete */
    if (dest.size() > MAX_RESULT_SET)
        return (-1);

    if (bit >= 64)
        return (1);

    /* Si le bit du masque est a 0-fixe, on s'en fout */
    if (masque[bit] == 0)
        return(Calcul::gestion_masque_oper_and_rec(masque, dest, bit+1));

    if (masque[bit] == 1)
    {
        for (unsigned long i=0; i<bit; i++)
        {
            if (masque[i] >= 2)
                masque[i] = 1;
        }
        masque[bit]++;
        return (0);
    }
    return (Calcul::gestion_masque_oper_and_rec(masque, dest, bit+1));
}



/*
** Ces fonctions permettent d'optimiser l'estimation des resultats d'un OR-logique
** si un des operandes est inconnu.
**
** Elles utilisent des masques pour determiner les bits qui seront toujours a 1
*/
std::set<std::string>    &Calcul::gestion_masque_oper_or(std::set<std::string> &dest,
                                                          const Calcul::NodeCalcul *n1,
                                                          const Calcul::NodeCalcul *n2)
{
    const std::set<std::string>    *ptr_list;
    std::string                    str;
    unsigned long                  value_operande;
    unsigned long                  nbs_bit_a_0;
    unsigned long                  masque[64];
    int                            ret;

    /* Choisi l'operande servant pour le masque */
    if (!n1->_values.empty())
        ptr_list = &(n1->_values);
    else
        ptr_list = &(n2->_values);

    /* Pour toutes les valeurs possibles de cet operande... */
    for (std::set<std::string>::const_iterator it=ptr_list->begin();
         it!=ptr_list->end();
         it++)
    {
        value_operande = Calcul::Oxtol((*it).c_str());

        /* Prepare le masque et compte les bits a 1 */
        nbs_bit_a_0 = 0;
        for (unsigned long i=0; i<64; i++)
        {
            if (((value_operande >> i) & 1) == 0)
                nbs_bit_a_0++;
            masque[i] = (value_operande >> i) & 1;
        }

        /* S'il y a plus de 8 bits a 0, ca fait trop de possibilites */
        if (nbs_bit_a_0 > 8)
        {
            dest.clear();
            return (dest);
        }

        /* Genere des valeurs a partir du masque */
        do
        {
            value_operande = 0;
            for (unsigned long i=0; i<64; i++)
            {
                if (masque[i] > 0)
                    value_operande = value_operande | (1 << i);
            }

            dest.insert(Calcul::lto0x(str, value_operande));
        }
        while ((ret = Calcul::gestion_masque_oper_and_rec(masque, dest, 0)) == 0);

        /* Si la gestion de masque retourne -1, cela signifie qu'on a trouve trop de possibilites */
        if (ret < 0)
        {
            dest.clear();
            return (dest);
        }
    }

    return (dest);
}

int    Calcul::gestion_masque_oper_or_rec(unsigned long masque[64],
                                           std::set<std::string> &dest,
                                           unsigned long bit)
{
    /* S'il y a trop de possibilites, on arrete */
    if (dest.size() > MAX_RESULT_SET)
        return (-1);

    if (bit >= 64)
        return (1);

    /* Si le bit du masque est a 1-fixe, on s'en fout */
    if (masque[bit] == 1)
        return(Calcul::gestion_masque_oper_or_rec(masque, dest, bit+1));

    if (masque[bit] == 0)
    {
        for (unsigned long i=0; i<bit; i++)
        {
            if (masque[i] >= 2)
                masque[i] = 0;
        }
        masque[bit]++;
        return (0);
    }
    return (Calcul::gestion_masque_oper_or_rec(masque, dest, bit+1));
}







/**
** \fn std::string &suppr_space_expr(std::string &s)
** \brief Gere la suppression des espaces et des tabulations dans une string
**
** \param s String a epurer
** \return Retourne rien
*/
std::string    &Calcul::suppr_space_expr(std::string &s)
{
    size_t    pos;

    while ((pos = s.find(" ")) != std::string::npos)
        s.erase(pos, 1);
    while ((pos = s.find("\t")) != std::string::npos)
        s.erase(pos, 1);
    return (s);
}

/**
** \fn std::string &suppr_space_expr(std::string &s)
** \brief Gere la suppression des retours a la ligne dans une string
**
** \param s String a epurer
** \return Retourne rien
*/
std::string    &Calcul::suppr_endl_expr(std::string &s)
{
    size_t    pos;

    while ((pos = s.find("\n")) != std::string::npos)
        s.erase(pos, 1);
    while ((pos = s.find("\r")) != std::string::npos)
        s.erase(pos, 1);
    return (s);
}

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
int    Calcul::get_priority_oper(const char *oper)
{
    const char *signe[] =
    {
        "*", "/", "%",
        "+", "-",
        ">>", "<<",
        "&&", "||",
        "&",
        "^",
        "|",
        "==", "!=", "<=", ">=", "<", ">",
        NULL
    };
    int priorite[] =
    {
        8, 8, 8,
        7, 7,
        6, 6,
        1, 1,
        5,
        4,
        3,
        2, 2, 2, 2, 2, 2,
        0
    };

    for (unsigned long i=0; signe[i]!=NULL; i++)
    {
        if (strncmp(oper, signe[i], strlen(signe[i])) == 0)
            return (priorite[i]);
    }
    return (0);
}

/**
** \fn std::string ltos(long n)
** \brief Gere la converion d'un long en string decimal
**
** \param n Nombre a convertir
** \return Retourne une string contenant le nombre
*/
std::string    Calcul::ltos(long n)
{
    char    buffer[50];

    snprintf(buffer, 49, "%ld", n);
    return (buffer);
}

/**
** \fn std::string ltox(long n)
** \brief Gere la converion d'un long en string hexadecimal
**
** \param n Nombre a convertir
** \return Retourne une string contenant le nombre
*/
std::string    Calcul::ltox(long n)
{
    char    buffer[50];

    snprintf(buffer, 49, "%lx", n);
    return (buffer);
}

/**
** \fn std::string ltos(long n)
** \brief Gere la converion d'un long en string 0xhexadecimal
**
** \param n Nombre a convertir
** \return Retourne une string contenant le nombre
*/
std::string    Calcul::lto0x(long n)
{
    char    buffer[50];

    snprintf(buffer, 49, "0x%lx", n);
    return (buffer);
}


/**
** \fn std::string &ltos(std::string &dest, long n)
** \brief Gere la converion d'un long en string decimal
**
** \param dest String ou mettre le nombre converti
** \param n Nombre a convertir
** \return Retourne une string contenant le nombre
*/
std::string    &Calcul::ltos(std::string &dest, long n)
{
    char    buffer[50];

    snprintf(buffer, 49, "%ld", n);
    dest = buffer;
    return (dest);
}

/**
** \fn std::string &ltox(std::string &dest, long n)
** \brief Gere la converion d'un long en string hexadecimal
**
** \param dest String ou mettre le nombre converti
** \param n Nombre a convertir
** \return Retourne une string contenant le nombre
*/
std::string    &Calcul::ltox(std::string &dest, long n)
{
    char    buffer[50];

    snprintf(buffer, 49, "%lx", n);
    dest = buffer;
    return (dest);
}

/**
** \fn std::string &ltos(std::string &dest, long n)
** \brief Gere la converion d'un long en string 0xhexadecimal
**
** \param dest String ou mettre le nombre converti
** \param n Nombre a convertir
** \return Retourne une string contenant le nombre
*/
std::string    &Calcul::lto0x(std::string &dest, long n)
{
    char    buffer[50];

    snprintf(buffer, 49, "0x%lx", n);
    dest = buffer;
    return (dest);
}


/**
** \fn long xtol(const char *s)
** \brief Gere la conversion d'une chaine de caractere hexadecimal en long
**
** \param s Chaine de caracteres hexadecimal a convertir en long
** \return Retourne le nombre contenu dans la chaine si OK, 0 sinon
*/
long    Calcul::xtol(const char *s)
{
    long    n;

    n = 0;
    while (isxdigit(*s))
    {
        if (isdigit(*s))
            n = (n * 16) + (*s - '0');
        else if (islower(*s))
            n = (n * 16) + (*s - 'a' + 10);
        else if (isupper(*s))
            n = (n * 16) + (*s - 'A' + 10);
        s++;
    }
    return (n);
}

/**
** \fn long Oxtol(const char *s)
** \brief Gere la conversion d'une chaine de caractere 0xhexa en long (elle a interet a etre valide)
**
** \param s Chaine de caracteres 0xhexadecimal a convertir en long
** \return Retourne le nombre contenu dans la chaine si OK, 0 sinon
*/
long    Calcul::Oxtol(const char *s)
{
    long    n;

    n = 0;
    s++;
    s++;
    while (isxdigit(*s))
    {
        if (isdigit(*s))
            n = (n * 16) + (*s - '0');
        else if (islower(*s))
            n = (n * 16) + (*s - 'a' + 10);
        else if (isupper(*s))
            n = (n * 16) + (*s - 'A' + 10);
        s++;
    }
    return (n);
}

