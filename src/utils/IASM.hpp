/*
** IASM.hpp for Lecter
**
** Made by joran herve
**
** Started on  Mon, 29 Jun 2015 19:27:13 joran herve
** Last update Mon, 29 Jun 2015 19:27:13 joran herve
**
** Status : OK
*/

#ifndef    IASM_HPP_
#define    IASM_HPP_

#include    <string>
#include    <vector>
#include    <list>
#include    <map>
#include    <set>

#include    "Mutex.hpp"


class    Info;
class    Fonction;
class    Contexte;
class    ContentContexte;

/*
** Nom du compteur programme
** (il reste le meme pour tout les types ASM car il sert au deassembleur
** de fonctions pour determinee s'il y a eu un jump/call)
*/
#define    NAME_PC    "pc"

/*
** Nom de description de l'instruction si elle est invalide
*/
#define    NAME_INVALIDE           "(invalide)"
#define    DESCRIPTION_INVALIDE    "Cette instruction est invalide ou n'a pas ete implementee par le desassembleur"

/*
** Define permettant de preciser le type des instructions (Plusieurs types possible)
*/
#define    INSTR_INVALIDE      0x001     /* Instruction invalide */
#define    INSTR_NOP           0x002     /* Instruction inutile */
#define    INSTR_OPER          0x004     /* Instruction realisant une operation */
#define    INSTR_ASSOC         0x008     /* Instruction realisant une affectation */
#define    INSTR_RETURN        0x010     /* Instruction retournant d'une fonction */
#define    INSTR_JUMP          0x020     /* Instruction de saut */
#define    INSTR_CALL          0x040     /* Instruction appelant une fonction */
#define    INSTR_CONDITION     0x080     /* Instruction conditionelle */
#define    INSTR_INT           0x100     /* Interruption */
#define    INSTR_IO            0x200     /* Instruction ayant un effet externe */
#define    INSTR_FORK          0x400     /* Instruction divisant le processus en 2 */


/**
** \class InstrASM
** \brief Classe Definissant les instructions ASM (tout les attributs sont public !!!)
*/
class    InstrASM
{
public:
    /**
    ** \class Operande
    ** \brief Classe permettant de decrire les operandes des instructions ASM
    */
    class    Operande
    {
    public:
        /**
        ** \fn Operande()
        ** \brief Constructeur par defaut de l'operande
        */
        Operande(): _size(0), _name("") {};

        /**
        ** \fn Operande(unsigned long s, const std::string &n)
        ** \brief Constructeur par copie de l'operande
        **
        ** \param s Taille de l'operande en octet
        ** \param n Nom de l'operande
        */
        Operande(unsigned long s, const std::string &n): _size(s), _name(n) {};

        /**
        ** \fn Operande(const InstrASM::Operande &o)
        ** \brief Constructeur par copie de l'operande
        **
        ** \param o Operande a copier
        */
        Operande(const InstrASM::Operande &o): _size(o._size), _name(o._name) {};

    public:
        /* Taille de l'operande en bits */
        unsigned long    _size;
        /** Nom de l'operande */
        std::string      _name;
    };

public:
    /**
    ** \fn InstrASM()
    ** \brief Constructeur d'une instruction ASM
    */
    InstrASM();

    /**
    ** \fn ~InstrASM()
    ** \brief Destructeur d'une instruction
    */
    virtual ~InstrASM();

protected:
    /**
    ** \fn InstrASM(const InstrASM &)
    ** \brief Constructeur par copie d'une instruction ASM (inutilisable)
    **
    ** \param i Instruction ASM a copier
    */
    InstrASM(const InstrASM &);

    /**
    ** \fn InstrASM& operator = (const InstrASM &)
    ** \brief Surcharge de l'operateur = pour les instructions ASM (inutilisable)
    **
    ** \param i Instruction ASM a copier
    ** \return Retourne une reference sur l'instruction ASM
    */
    InstrASM&    operator = (const InstrASM &);

public:
    /**
    ** \fn std::string to_asm(const Info *info) const
    ** \brief Gere la preparation de l'instruction ASM
    **
    ** \param info Structure contenant les infos du programme (peut etre NULL)
    ** \return Retourne une string decrivant l'instruction (peut egalement contenir des commentaires)
    */
    std::string    to_asm(const Info *info) const;

    /**
    ** \fn std::string &to_asm(const Info *info, std::string &str) const
    ** \brief Gere la preparation de l'instruction ASM
    **
    ** \param info Structure contenant les infos du programme (peut etre NULL)
    ** \param str String ou mettre l'instruction ASM
    ** \return Retourne une string decrivant l'instruction (peut egalement contenir des commentaires)
    */
    virtual std::string    &to_asm(const Info *info, std::string &str) const;

public:
    /** Adresse de l'instruction */
    unsigned long                      _address;
    /** Taille de l'instruction */
    unsigned long                      _size;

    /** Type d'instruction (INSTR_INVALIDE, INSTR_OPER, INSTR_ASSOC, ...) */
    unsigned long                      _type;

    /** Nom de l'instruction ASM (ADD, INC, invalide, ...) */
    std::string                        _name;
    /** Liste des prefix pouvant exister avant l'instruction ASM (REPE, REPNE, ...) */
    std::vector<std::string>           _prefix;
    /** Operande destination (quand il y en a une) (eax, [eax], ...) */
    InstrASM::Operande                 _dst;
    /** Liste des operandes de l'instruction (eax, [eax], 0x42, ...) */
    std::vector<InstrASM::Operande>    _operande;

    /** Description de l'instruction ("NOP: Cette instruction ne fait rien) (Ne pas deallouer!!) */
    const char                         *_description;

    /** Tableau conteant la description des effets de bord de l'instruction */
    std::list<std::string>             _effect;
    /** Fonction a utiliser pour executer l'instruction au lieu d'utiliser les effets de bords */
    int                                (*_func_exec)(Info*, Fonction*, InstrASM*);
    /** Contexte d'execution de l'instruction */
    Contexte                           *_contexte;

    /** Tableau contenant les adresses des instructions suivantes en cas de jump */
    std::list<unsigned long>           _addr_next_instr_jump;
    /** Pointeur sur l'instruction suivante */
    InstrASM                           *_next_instr;
    /** Pointeur sur l'instruction precedente */
    InstrASM                           *_prev_instr;
};



/**
** \class IASM
** \brief Classe virtuelle pure definissant les fonctions proposees par les deassembleurs
*/
class    IASM
{
public:
    /**
    ** \fn IASM()
    ** \brief Constructeur par defaut du deassembleur
    */
    IASM();

    /**
    ** \fn ~IASM()
    ** \brief Constructeur par defaut du deassembleur
    */
    virtual ~IASM();

protected:
    /**
    ** \fn IASM(const IASM&)
    ** \brief Constructeur par copie du deassembleur (inutilisable)
    */
    IASM(const IASM&);

    /**
    ** \fn IASM& operator = (const IASM&)
    ** \brief Surcharge de l'operateur = pour les deassembleur
    **
    ** \return Retourne une reference sur le deassembleur
    */
    IASM&    operator = (const IASM&);

public:
    /**
    ** \fn const char **get_registre() const = 0;
    ** \brief Assesseurs permettant d'acceder a la liste des registres existants
    **
    ** \return Retourne un tableau contenant les nom des registres
    */
    virtual const char   **get_registre() const = 0;

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
    virtual void init_registre(std::map<std::string, ContentContexte> &reg,
                               unsigned long addr=0, const Info *info=NULL) const = 0;


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
    virtual void    get_name_and_mask_getter(const std::string &name, std::string &begin_mask,
                                             std::string &name_parent, std::string &end_mask);

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
    virtual void    get_name_and_mask_setter(const std::string &name, std::string &begin_mask,
                                             std::string &name_parent, std::string &end_mask);

    /**
    ** \fn std::string get_size_registre(const std::string &name,
    **                                   const std::map<std::string, ContentContexte> &reg)
    ** \brief Fonction permettant d'acceder a la taille d'un registre
    **
    ** \param name Nom du registre
    ** \param reg Map contenant les registres
    ** \return Retourne la taille du registre si on le trouve, 0 sinon
    */
    virtual unsigned long    get_size_registre(const std::string &name,
                                               const std::map<std::string, ContentContexte*> &reg) = 0;

    /**
    ** \fn std::string get_type_registre(const std::string &name,
    **                                   const std::map<std::string, ContentContexte> &reg)
    ** \brief Fonction permettant d'acceder au type de contenu d'un registre
    **
    ** \param name Nom du registre
    ** \param reg Map contenant les registres
    ** \return Retourne le type de contenu du registre si on le trouve, "" sinon
    */
    std::string      get_type_registre(const std::string &name,
                                               const std::map<std::string, ContentContexte*> &reg);

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
    virtual std::string      &get_type_registre(std::string &dest, const std::string &name,
                                               const std::map<std::string, ContentContexte*> &reg) = 0;


    /**
    ** \fn deasm_instr(unsigned long addr, Info *info)
    ** \brief Gere le deassemblage de l'instruction ASM a l'adresse indiquee
    **
    ** \param addr Adresse de l'instruction a deassembler
    ** \param info Structure contenant les infos du programme a analyser
    ** \return Retourne un pointeur sur l'instruction cree si OK, NULL sinon
    */
    virtual InstrASM      *deasm_instr(unsigned long addr, Info *info) = 0;

protected:
    /** Mutex permettant de rendre la classe thread-safe */
    Mutex    _mutex;
};

#endif

