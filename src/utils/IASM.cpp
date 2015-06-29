/*
** IASM.cpp for Lecter
**
** Made by joran herve
**
** Started on  Mon, 29 Jun 2015 19:27:13 joran herve
** Last update Mon, 29 Jun 2015 19:27:13 joran herve
**
** Status : OK
*/

#include    "IASM.hpp"
#include    "Info.hpp"
#include    "Contexte.hpp"
#include    "Calcul.hpp"


/**
** \fn InstrASM()
** \brief Constructeur d'une instruction ASM
*/
InstrASM::InstrASM():
    _address(0),
    _size(0),
    _type(0),

    _name(),
    _prefix(),
    _dst(),
    _operande(),

    _description(NULL),

    _effect(),
    _func_exec(NULL),
    _contexte(NULL),

    _addr_next_instr_jump(),
    _next_instr(NULL),
    _prev_instr(NULL)
{
}

/**
** \fn ~InstrASM()
** \brief Destructeur d'une instruction
*/
InstrASM::~InstrASM()
{
    if (this->_contexte != NULL)
        delete this->_contexte;
}

/**
** \fn InstrASM(const InstrASM &)
** \brief Constructeur par copie d'une instruction ASM (inutilisable)
**
** \param i Instruction ASM a copier
*/
InstrASM::InstrASM(const InstrASM &):
    _address(0),
    _size(0),
    _type(0),

    _name(),
    _prefix(),
    _dst(),
    _operande(),

    _description(NULL),

    _effect(),
    _func_exec(NULL),
    _contexte(NULL),

    _addr_next_instr_jump(),
    _next_instr(NULL),
    _prev_instr(NULL)
{
}

/**
** \fn InstrASM& operator = (const InstrASM &)
** \brief Surcharge de l'operateur = pour les instructions ASM (inutilisable)
**
** \param i Instruction ASM a copier
** \return Retourne une reference sur l'instruction ASM
*/
InstrASM&    InstrASM::operator = (const InstrASM &)
{
    return (*this);
}


/**
** \fn std::string to_asm(const Info *info) const
** \brief Gere la preparation de l'instruction ASM
**
** \param info Structure contenant les infos du programme (peut etre NULL)
** \return Retourne une string decrivant l'instruction (peut egalement contenir des commentaires)
*/
std::string    InstrASM::to_asm(const Info *info) const
{
    std::string    s;

    this->to_asm(info, s);
    return (s);
}

/**
** \fn std::string &to_asm(const Info *info, std::string &str) const
** \brief Gere la preparation de l'instruction ASM
**
** \param info Structure contenant les infos du programme (peut etre NULL)
** \param str String ou mettre l'instruction ASM
** \return Retourne une string decrivant l'instruction (peut egalement contenir des commentaires)
*/
std::string    &InstrASM::to_asm(const Info *info, std::string &str) const
{
    unsigned long    addr;
    int              first;

    str.clear();

    /* Prefixe */
    for (std::vector<std::string>::const_iterator it=this->_prefix.begin();
         it!=this->_prefix.end();
         it++)
        str += (*it) + " ";

    /* Nom de l'instruction */
    str += this->_name;

    /* Parametre de l'instruction */
    for (std::vector<InstrASM::Operande>::const_iterator it_operande=this->_operande.begin();
         it_operande!=this->_operande.end();
         it_operande++)
    {
        if (it_operande != this->_operande.begin())
            str += ",\t";
        else
            str += "\t";

        /* Il preciser la taille de l'operande si c'est une adresse memoire */
        if ((it_operande->_name.size() > 0) && (it_operande->_name[0] == '['))
        {
            if (it_operande->_size == 1)
                str += "BYTE PTR ";
            else if (it_operande->_size == 2)
                str += "WORD PTR ";
            else if (it_operande->_size == 4)
                str += "DWORD PTR ";
            else if (it_operande->_size == 8)
                str += "QWORD PTR ";
        }

        str += it_operande->_name;
    }

    /* Ajout de commentaires pour rendre le code plus lisible */
    if (info != NULL)
    {
        /* On regarde si les operandes semblent correspondre a un symbole (0x...) */
        first = 1;
        for (std::vector<InstrASM::Operande>::const_iterator it_operande=this->_operande.begin();
             it_operande!=this->_operande.end();
             it_operande++)
        {
            if ((it_operande->_name.find("0x") == 0) &&
                ((addr = Calcul::Oxtol(it_operande->_name.c_str())) != 0))
            {
                /* Si le nombre correspond a un symbole, on l'ajoute en commentaire */
                if (info->sym.get_name(addr).size() > 0)
                {
                    if (first == 1)
                        str += "\t# ";
                    else
                        str += ", ";
                    str += info->sym.get_name(addr);
                }

                /* Si le nombre correspond a une chaine de caractere, on l'ajoute en commentaire */
                else if (isprint(info->sec.get_char(addr)))
                {
                    std::string      str_tmp = "\"";
                    unsigned long    offset;
                    unsigned char    c;

                    for (offset=0; (offset<10) && (isprint(c=info->sec.get_char(addr+offset))); offset++)
                    {
                        if (isgraph(c) || (c == ' '))
                            str_tmp += c;
                        else if (c == '\f')
                            str_tmp += "\\f";
                        else if (c == '\n')
                            str_tmp += "\\n";
                        else if (c == '\r')
                            str_tmp += "\\r";
                        else if (c == '\t')
                            str_tmp += "\\t";
                        else if (c == '\v')
                            str_tmp += "\\v";
                    }

                    if ((offset == 10) || (c == '\0'))
                    {
                        if (first == 1)
                            str += "\t# ";
                        else
                            str += ", ";

                        if (c == '\0')
                            str += str_tmp + "\"";
                        else
                            str += str_tmp + "...\"";
                    }


                }
            }
        }
    }

    return (str);
}




/**
** \fn IASM()
** \brief Constructeur par defaut du deassembleur
*/
IASM::IASM():
    _mutex()
{
}

/**
** \fn ~IASM()
** \brief Constructeur par defaut du deassembleur
*/
IASM::~IASM()
{
}

/**
** \fn IASM(const IASM&)
** \brief Constructeur par copie du deassembleur (inutilisable)
*/
IASM::IASM(const IASM&):
    _mutex()
{
}

/**
** \fn IASM& operator = (const IASM&)
** \brief Surcharge de l'operateur = pour les deassembleur
**
** \return Retourne une reference sur le deassembleur
*/
IASM&    IASM::operator = (const IASM&)
{
    return (*this);
}


/**
** \fn void get_name_and_mask_getter(const std::string &name, std::string &begin_mask,
**                                   std::string &name_parent, std::string &end_mask)
** \brief Fonction permettant au contexte de savoir comment acceder aux sous-registres
**
** \param name Nom du registre auquel acceder
** \param begin_mask String ou mettre le debut du masque a utiliser pour extraire la valeur du sous-registre de son parent
** \param name_parent String ou mettre le nom du registre parent (vaudra name s'il ny a pas de parent)
** \param end_mask String ou mettre la fin du masque a utiliser pour extraire la valeur du sous-registre de son parent
** \return Retourne rien
*/
void    IASM::get_name_and_mask_getter(const std::string &name, std::string &begin_mask,
                                       std::string &name_parent, std::string &end_mask)
{
    begin_mask.clear();
    name_parent = name;
    end_mask.clear();
}

/**
** \fn void get_name_and_mask_setter(const std::string &name, std::string &begin_mask,
**                                   std::string &name_parent, std::string &end_mask)
** \brief Fonction permettant au contexte de savoir comment modifier les sous-registres
**
** \param name Nom du registre auquel acceder
** \param begin_mask String ou mettre le debut du masque a utiliser pour extraire la valeur du sous-registre de son parent
** \param name_parent String ou mettre le nom du registre parent (vaudra name s'il ny a pas de parent)
** \param end_mask String ou mettre la fin du masque a utiliser pour extraire la valeur du sous-registre de son parent
** \return Retourne rien
*/
void    IASM::get_name_and_mask_setter(const std::string &name, std::string &begin_mask,
                                       std::string &name_parent, std::string &end_mask)
{
    begin_mask.clear();
    name_parent = name;
    end_mask.clear();
}


/**
** \fn std::string get_type_registre(const std::string &name,
**                                   const std::map<std::string, ContentContexte> &reg)
** \brief Fonction permettant d'acceder au type de contenu d'un registre
**
** \param name Nom du registre
** \param reg Map contenant les registres
** \return Retourne le type de contenu du registre si on le trouve, "" sinon
*/
std::string      IASM::get_type_registre(const std::string &name,
                                         const std::map<std::string, ContentContexte*> &reg)
{
    std::string   dest;

    return (this->get_type_registre(dest, name, reg));
}


