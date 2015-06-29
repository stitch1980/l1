/*
** ASM_brainfuck.cpp for plugin_deasm_brainfuck
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 14:34:37 joran herve
** Last update Mon, 29 Jun 2015 19:27:13 joran herve
**
** Status : OK (documentation faite)
*/

#include    "Info.hpp"
#include    "ASM_brainfuck.hpp"
#include    "Contexte.hpp"


#define    ASM_BRAINFUCK_EFFET__NBS_INSTR    8

/*
** Tableau decrivant les differentes instructions du langage Brainfuck
*/
const t_effect_instr    ASM_brainfuck_descr_effect[ASM_BRAINFUCK_EFFET__NBS_INSTR] =
{
    /* '>' */
    {
        '>',
        INSTR_OPER,
        {
            "index = index + 1", NULL
        },
        "inc",
        "index", 4,
        "index", 4,
        ">​ : Incremente le pointeur (ptr).\n"
    },

    /* '<' */
    {
        '<',
        INSTR_OPER,
        {
            "index = index - 1", NULL
        },
        "dec",
        "index", 4,
        "index", 4,
        "< : Decremente le pointeur (ptr).\n"
    },

    /* '+' */
    {
        '+',
        INSTR_OPER,
        {
            "[ptr+index*4] = [ptr+index*4] + 1", NULL
        },
        "inc",
        "[ptr+index*4]", 4,
        "[ptr+index*4]", 4,
        "+ : Incremente l'octet du tableau sur lequel est positionne le pointeur ([ptr+index*4]).\n"
    },

    /* '-' */
    {
        '-',
        INSTR_OPER,
        {
            "[ptr+index*4] = [ptr+index*4] - 1", NULL
        },
        "dec",
        "[ptr+index*4]", 4,
        "[ptr+index*4]", 4,
        "- : Decremente l'octet du tableau sur lequel est positionne le pointeur ([ptr+index*4]).\n"
    },

    /* '.' */
    {
        '.',
        INSTR_INT,
        {
            NULL, NULL
        },
        "write",
        NULL, 0,
        NULL, 0,
        ". : Sortie de l'octet pointe (valeur ASCII).\n"
    },

    /* ',' */
    {
        ',',
        INSTR_INT,
        {
            NULL, NULL
        },
        "read",
        "[ptr+index*4]", 4,
        NULL, 0,
        ", : Entree d'un octet dans le tableau a l'endroit ou est positionne le pointeur (valeur ASCII).\n"
    },

    /* '[' */
    {
        '[',
        INSTR_JUMP | INSTR_CONDITION,
        {
            NAME_PC " = ([ptr+index*4] == 0) ? (next_jump) : ("NAME_PC"+1)",
            NULL
        },
        "je",
        NULL, 0,
        "next_jump", 4,
        "[ : Saute a l'instruction apres le \']\' correspondant si l'octet pointe est a 0.\n"
    },

    /* ']' */
    {
        ']',
        INSTR_JUMP,
        {
            NAME_PC " = (previous_jump)",
            NULL
        },
        "jmp",
        NULL, 0,
        "previous_jump", 4,
        "] : Retourne a l'instruction apres le \'[\' si l'octet pointe est different de 0.\n"
    }
};



/**
** \fn ASM_brainfuck()
** \brief Constructeur par defaut de la classe gerant l'assembleur Brainfuck
*/
ASM_brainfuck::ASM_brainfuck(): IASM()
{
}

/**
** \fn ~ASM_brainfuck()
** \brief Destructeur de la classe gerant l'assembleur Brainfuck
*/
ASM_brainfuck::~ASM_brainfuck()
{
}

/**
** \fn ASM_brainfuck(const ASM_brainfuck&)
** \brief Constructeur par copie de la classe gerant l'assembleur Brainfuck (inutile)
*/
ASM_brainfuck::ASM_brainfuck(const ASM_brainfuck&): IASM()
{
}

/**
** \fn ASM_brainfuck& ASM_brainfuck::operator = (const ASM_brainfuck&)
** \brief Surcharge de l'operateur = pour la classe gerant l'assembleur Brainfuck
**
** \return Retourne une reference sur l'assembleur
*/
ASM_brainfuck&    ASM_brainfuck::operator = (const ASM_brainfuck&)
{
    return (*this);
}



/**
** \fn const char **get_registre() const
** \brief Assesseurs permettant d'acceder a la liste des registres existants
**
** \return Retourne Retourne un tableau contenant les nom des registres
*/
const char   **ASM_brainfuck::get_registre() const
{
    static const char *registre[] =
    {
        "ptr", "index", NAME_PC,
        NULL
    };

    return (registre);
}

/**
** \fn void init_registre(std::map<std::string, ContentContexte> &reg,
**                        unsigned long addr=0, const Info *info=NULL) const = 0;
** \brief Permet d'initialiser les registres du contexte d'execution
**
** \param reg Map contenant les registre a initialiser
** \param addr Adresse de la fonction pour lequel les registre sont prevu
** \param info Contient les infos du prog. (pour trouver d'eventuelles valeurs de reg.)
** \return Retourne rien
*/
void    ASM_brainfuck::init_registre(std::map<std::string, ContentContexte> &reg,
                                   unsigned long addr, const Info *info) const
{
    /* Valeur par defaut du pointeur de data */
    reg["ptr"].set_value("");
    reg["ptr"].set_size(4);
    reg["ptr"].set_type("int*");

    reg["index"].set_value("0x0");
    reg["index"].set_size(4);
    reg["index"].set_type("int");

    /* On cherche une section avec acces en lectuere/ecriture pour le pointeur de data */
    for (unsigned long i=0; i<info->sec.get_nbs_section(); i++)
    {
    	if (((info->sec.get_flag_section(i) & SECTION_R) == SECTION_R) &&
    	    ((info->sec.get_flag_section(i) & SECTION_W) == SECTION_W))
            reg["ptr"].set_value( Calcul::lto0x(info->sec.get_addr_section(i)) );
    }

    /* Precise le PC de la fonction */
    reg[NAME_PC].set_value(Calcul::lto0x(addr));
    reg[NAME_PC].set_size(4);
    reg[NAME_PC].set_type("void*");
}


/**
** \fn std::set<std::string> get_value_registre(std::set<std::string> &dest, const std::string &name,
**                                    const std::map<std::string, ContentContexte> &reg)
** \brief Fonction permettant d'acceder a la valeur d'un registre
**
** \param dest Set ou mettre les valeurs
** \param name Nom du registre
** \param reg Map contenant les registres
** \return Retourne les valeurs du registre si on le trouve, un set vide sinon
*/
std::set<std::string>    &ASM_brainfuck::get_value_registre(std::set<std::string> &dest,
                                                            const std::string &name,
                                                            const std::map<std::string, ContentContexte*> &reg)
{
    this->_mutex.lock();

    dest.clear();
    if (reg.find(name) != reg.end())
        dest = reg.find(name)->second->get_values();

    this->_mutex.unlock();
    return (dest);
}

/**
** \fn std::string get_size_registre(const std::string &name,
**                                   const std::map<std::string, ContentContexte> &reg)
** \brief Fonction permettant d'acceder a la taille d'un registre
**
** \param name Nom du registre
** \param reg Map contenant les registres
** \return Retourne la taille du registre si on le trouve, 0 sinon
*/
unsigned long    ASM_brainfuck::get_size_registre(const std::string &name,
                                                const std::map<std::string, ContentContexte*> &reg)
{
    unsigned long    ret;

    this->_mutex.lock();

    ret = 0;
    if (reg.find(name) != reg.end())
        ret = reg.find(name)->second->get_size();

    this->_mutex.unlock();
    return (ret);
}

/**
** \fn std::string &get_type_registre(std::string &dest, const std::string &name,
**                                   const std::map<std::string, ContentContexte> &reg)
** \brief Fonction permettant d'acceder au type de contenu d'un registre
**
** \param dest String ou mettre les valeurs
** \param name Nom du registre
** \param reg Map contenant les registres
** \return Retourne le type de contenu du registre si on le trouve, "" sinon
*/
std::string      &ASM_brainfuck::get_type_registre(std::string &dest, const std::string &name,
                                                   const std::map<std::string, ContentContexte*> &reg)
{
    this->_mutex.lock();

    dest = "";
    if (reg.find(name) != reg.end())
        dest = reg.find(name)->second->get_type();

    this->_mutex.unlock();
    return (dest);
}

/**
** \fn void set_value_registre(const std::string &name,
**                             std::map<std::string, ContentContexte> &reg,
**                             const std::set<std::string> &values,
**                             const std::string &type)
** \brief Fonction permettant d'acceder a la valeur d'un registre
**
** \param name Nom du registre
** \param reg Map contenant les registres
** \param values Nouvelles valeurs du registre
** \param type Nouveau type du registre
** \return Retourne la valeur du registre si on le trouve, "" sinon
*/
void             ASM_brainfuck::set_value_registre(const std::string &name,
                                                std::map<std::string, ContentContexte*> &reg,
                                                const std::set<std::string> &values,
                                                const std::string &type)
{
    std::map<std::string, ContentContexte*>::iterator    it;

    this->_mutex.lock();

    if ((it = reg.find(name)) != reg.end())
    {
        it->second->set_values(values);
        it->second->set_type(type);
    }

    this->_mutex.unlock();
}


/**
** \fn InstrASM *deasm_instr(unsigned long addr, Info *info)
** \brief Gere le deassemblage de l'instruction ASM a l'adresse indiquee
**
** \param addr Adresse de l'instruction a deassembler
** \param info Structure contenant les infos du programme a analyser
** \return Retourne un pointeur sur l'instruction cree si OK, NULL sinon
*/
InstrASM      *ASM_brainfuck::deasm_instr(unsigned long addr, Info *info)
{
    InstrASM         *instr;
    std::string      str_tmp;
    unsigned char    octet;

    if (info->sec.is_in_section(addr) == 0)
        return (NULL);
    this->_mutex.lock();

    /* On recupere l'octet a deassembler */
    octet = info->sec.get_char(addr);

    /* Cree et initialise l'instruction ASM */
    if ((instr = new InstrASM) != NULL)
    {
        instr->_address = addr;
        instr->_size = 1;
        instr->_type = INSTR_INVALIDE;

        instr->_name = NAME_INVALIDE;
        instr->_description = DESCRIPTION_INVALIDE;

        /* Recherche des infos personelles de l'instruction */
        for (unsigned long i=0; i<ASM_BRAINFUCK_EFFET__NBS_INSTR; i++)
        {
            if (ASM_brainfuck_descr_effect[i].mnemonic == octet)
            {
                instr->_type = ASM_brainfuck_descr_effect[i].type;
                instr->_name = ASM_brainfuck_descr_effect[i].name;

                /* Recuperation des operandes de l'instruction */
                if (ASM_brainfuck_descr_effect[i].dest_name != NULL)
                    instr->_dst = InstrASM::Operande(ASM_brainfuck_descr_effect[i].dest_size, ASM_brainfuck_descr_effect[i].dest_name);
                if (ASM_brainfuck_descr_effect[i].src_name != NULL)
                    instr->_operande.push_back(InstrASM::Operande(ASM_brainfuck_descr_effect[i].src_size, ASM_brainfuck_descr_effect[i].src_name));

                /* Recuperation des effets de bord de l'instruction */
                for (unsigned long j=0; (j<2) && (ASM_brainfuck_descr_effect[i].effect[j]!=NULL); j++)
                    instr->_effect.push_back(ASM_brainfuck_descr_effect[i].effect[j]);

                instr->_description = ASM_brainfuck_descr_effect[i].description;

                i = ASM_BRAINFUCK_EFFET__NBS_INSTR;
            }
        }

        /* Actualisation des effets de bords et de operandes de l'instrcution en fonction des offsets */
        this->remplace_offset(instr, &(info->sec), addr);
    }

    this->_mutex.unlock();
    return (instr);
}

/**
** \fn void remplace_offset(InstrASM *instr, const Section *sec, unsigned long addr)
** \brief Gere le remplacement des "next_jump" et "previous_jump" par leur adresse
**
** \param instr Instruction a traiter
** \param sec Gestionnaire de segments
** \param addr Adresse de l'instruction
** \return Retourne rien
*/
void    ASM_brainfuck::remplace_offset(InstrASM *instr, const Section *sec, unsigned long addr)
{
    std::vector<InstrASM::Operande>::iterator    it_operande;
    unsigned long                                pos;
    unsigned long                                profondeur;
    unsigned long                                offset;
    std::string                                  str_offset;

    if ((instr->_effect.size() > 0) && (instr->_effect.begin()->find("next_jump") != std::string::npos))
    {
        /* Cherche la fin de la boucle */
        profondeur = 1;
        offset = 1;
        while (((sec->get_flag(addr+offset) & SECTION_X) == SECTION_X) &&
               (profondeur != 0))
        {
            if (sec->get_char(addr+offset) == '[')
                profondeur++;
            else if (sec->get_char(addr+offset) == ']')
                profondeur--;
            offset++;
        }

        /* Prepare l'offset */
        str_offset = Calcul::lto0x(addr+offset);

        /* Remplace la chaine de demande d'offset par sa valeur */
        while ((pos = instr->_effect.begin()->find("next_jump")) != std::string::npos)
            instr->_effect.begin()->replace(pos, 9, str_offset);

        for (it_operande=instr->_operande.begin();
             it_operande!=instr->_operande.end();
             it_operande++)
            while ((pos = it_operande->_name.find("next_jump")) != std::string::npos)
                it_operande->_name.replace(pos, 9, str_offset);
    }

    if ((instr->_effect.size() > 0) && (instr->_effect.begin()->find("previous_jump") != std::string::npos))
    {
        /* Cherche la fin de la boucle */
        profondeur = 1;
        offset = 1;
        while (((sec->get_flag(addr+offset) & SECTION_X) == SECTION_X) &&
               (profondeur != 0))
        {
            if (sec->get_char(addr-offset) == '[')
                profondeur--;
            else if (sec->get_char(addr-offset) == ']')
                profondeur++;
            offset++;
        }
        if ((sec->get_flag(addr+(offset-1)) & SECTION_X) == SECTION_X)
            offset--;

        /* Prepare l'offset */
        str_offset = Calcul::lto0x(addr-offset);

        /* Remplace la chaine de demande d'offset par sa valeur */
        while ((pos = instr->_effect.begin()->find("previous_jump")) != std::string::npos)
            instr->_effect.begin()->replace(pos, 13, str_offset);

        for (it_operande=instr->_operande.begin();
             it_operande!=instr->_operande.end();
             it_operande++)
            while ((pos = it_operande->_name.find("previous_jump")) != std::string::npos)
                it_operande->_name.replace(pos, 13, str_offset);
    }
}

