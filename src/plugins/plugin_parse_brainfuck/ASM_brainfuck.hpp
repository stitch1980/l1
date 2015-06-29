/*
** ASM_brainfuck.hpp for plugin_deasm_brainfuck
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 14:34:37 joran herve
** Last update Mon, 29 Jun 2015 19:27:13 joran herve
**
** Status : OK (documentation faite)
*/

#ifndef    ASM_BRAINFUCK_HPP_
#define    ASM_BRAINFUCK_HPP_

#include    <stdio.h>
#include    <string.h>
#include    "IASM.hpp"
#include    "Calcul.hpp"


class    Info;
class    InstrASM;
class    ASM_brainfuck;


/**
** \struct s_effect_instr
** \brief Structure contenant la description des effets d'une instruction ASM
*/
typedef struct s_effect_instr
{
    /** Mnemonic permettant d'identifier l'instruction */
    unsigned char    mnemonic;
    /** Type d'instruction (INSTR_INVALIDE, INSTR_OPER, INSTR_ASSOC, ...) */
    unsigned long    type;
    /** Description des effets de bord sous forme de calcul */
    const char       *effect[2];

    /** Nom de l'instruction ASM */
    const char       *name;
    /** Nom et taille de l'operande destination */
    const char       *dest_name;
    unsigned long    dest_size;
    /** Nom et taille de l'operande source */
    const char       *src_name;
    unsigned long    src_size;

    /** Description de l'instruction ("NOP: Cette instruction ne fait rien) */
    const char       *description;
} t_effect_instr;


/**
** \class ASM_brainfuck
** \brief Classe permettant de gerer le deassemblage des instructions Brainfuck
*/
class    ASM_brainfuck: public IASM
{
public:
    /**
    ** \fn ASM_brainfuck()
    ** \brief Constructeur par defaut de la classe gerant l'assembleur Brainfuck
    */
    ASM_brainfuck();

    /**
    ** \fn ~ASM_brainfuck()
    ** \brief Destructeur de la classe gerant l'assembleur Brainfuck
    */
    ~ASM_brainfuck();

protected:
    /**
    ** \fn ASM_brainfuck(const ASM_brainfuck&)
    ** \brief Constructeur par copie de la classe gerant l'assembleur Brainfuck (inutile)
    */
    ASM_brainfuck(const ASM_brainfuck&);

    /**
    ** \fn ASM_brainfuck& operator = (const ASM_brainfuck&)
    ** \brief Surcharge de l'operateur = pour la classe gerant l'assembleur Brainfuck
    **
    ** \return Retourne une reference sur l'assembleur
    */
    ASM_brainfuck&    operator = (const ASM_brainfuck&);

public:
    /**
    ** \fn const char **get_registre() const;
    ** \brief Assesseurs permettant d'acceder a la liste des registres existants
    **
    ** \return Retourne Retourne un tableau contenant les nom des registres
    */
    virtual const char   **get_registre() const;

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
                               unsigned long addr=0, const Info *info=NULL) const;

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
    virtual std::set<std::string>    &get_value_registre(std::set<std::string> &dest,
                                                         const std::string &name,
                                                         const std::map<std::string, ContentContexte*> &reg);

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
                                                const std::map<std::string, ContentContexte*> &reg);

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
    virtual void             set_value_registre(const std::string &name,
                                                std::map<std::string, ContentContexte*> &reg,
                                                const std::set<std::string> &values,
                                                const std::string &type);


    /**
    ** \fn InstrASM *deasm_instr(unsigned long addr, Info *info)
    ** \brief Gere le deassemblage de l'instruction ASM a l'adresse indiquee
    **
    ** \param addr Adresse de l'instruction a deassembler
    ** \param info Structure contenant les infos du programme a analyser
    ** \return Retourne un pointeur sur l'instruction cree si OK, NULL sinon
    */
    virtual InstrASM      *deasm_instr(unsigned long addr, Info *info);

protected:
    /**
    ** \fn void remplace_offset(InstrASM *instr, const Section *sec, unsigned long addr)
    ** \brief Gere le remplacement des "next_jump" et "previous_jump" par leur adresse
    **
    ** \param instr Instruction a traiter
    ** \param sec Gestionnaire de segments
    ** \param addr Adresse de l'instruction
    ** \return Retourne rien
    */
    void    remplace_offset(InstrASM *instr, const Section *sec, unsigned long addr);
};

#endif

