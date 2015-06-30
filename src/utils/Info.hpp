/*
** Info.hpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 14:34:37 joran herve
** Last update Sat, 27 Jun 2015 14:34:37 joran herve
**
** Status : OK
*/

#ifndef        INFO_HPP_
#define        INFO_HPP_

#include    <cstdlib>
#include    <iostream>
#include    <map>
#include    <set>

#include    "Analyse.hpp"
#include    "Fonction.hpp"
#include    "ContentContexte.hpp"
#include    "IASM.hpp"
#include    "Mutex.hpp"
#include    "Plugin.hpp"
#include    "PrototypeF.hpp"
#include    "PrototypeS.hpp"
#include    "PrototypeT.hpp"
#include    "Section.hpp"
#include    "Symbole.hpp"


/**
** \class Info
** \brief Classe contenant toutes les infos a passer aux plugins lors de leurs executions
*/
class    Info
{
public:
    /**
    ** \class PtrUsefulFunc
    ** \brief Classe contenant les pointeurs de fonctions utile au deassemblage et a l'analyse
    */
    class    PtrUsefulFunc
    {
    public:
        static const char    *f_param_default(unsigned long) { return (NAME_PC); }
        static const char    *f_ret_default() { return (NAME_PC); }
        static const char    *f_num_default() { return (NAME_PC); }

    public:
        PtrUsefulFunc():
            deasm(NULL),
            f_param_func(NULL), f_ret_func(NULL),
            f_num_int(NULL),     f_param_int(NULL),     f_ret_int(NULL),
            f_num_syscall(NULL), f_param_syscall(NULL), f_ret_syscall(NULL)
        {
            this->clear();
        }

        void    clear()
        {
            if (this->deasm != NULL)
                delete this->deasm;
            this->deasm = NULL;

            this->f_param_func = &f_param_default;
            this->f_ret_func = &f_ret_default;
            this->f_num_int = &f_num_default;
            this->f_param_int = &f_param_default;
            this->f_ret_int = &f_ret_default;
            this->f_num_syscall = &f_num_default;
            this->f_param_syscall = &f_param_default;
            this->f_ret_syscall = &f_ret_default;
        }

    public:
        /** Gestionnaire permettant de deassembler les instructions ASM */
        IASM          *deasm;

        /** Fonction permettant de connaitre l'emplacement d'un parametre de fonction */
        const char    *(*f_param_func)(unsigned long);
        /** Fonction permettant de connaitre l'emplacement de la valeur de retour d'une fonction */
        const char    *(*f_ret_func)();

        /** Fonction permettant de connaitre l'emplacement d'un numero d'interruption */
        const char    *(*f_num_int)();
        /** Fonction permettant de connaitre l'emplacement d'un parametre d'interruption */
        const char    *(*f_param_int)(unsigned long);
        /** Fonction permettant de connaitre l'emplacement de la valeur de retour d'une interruption */
        const char    *(*f_ret_int)();

        /** Fonction permettant de connaitre l'emplacement d'un numero d'interruption */
        const char    *(*f_num_syscall)();
        /** Fonction permettant de connaitre l'emplacement d'un parametre de syscall */
        const char    *(*f_param_syscall)(unsigned long);
        /** Fonction permettant de connaitre l'emplacement de la valeur de retour d'un syscall */
        const char    *(*f_ret_syscall)();
    };

    /**
    ** \class VariablesFunc
    ** \brief Classe contenant les definitions de variables et les valeurs predefinie pour les fonctions
    */
    class    VariablesFunc
    {
    public:
        VariablesFunc():
            predefined_values(),
            emplacement_param(),
            emplacement_return() {}

        void    clear()
        {
            this->predefined_values.clear();
            this->emplacement_param.clear();
            this->emplacement_return.clear();
        }

    public:
        /** Liste des valeurs predefinies pour la fonction */
        /** <adresse a laquelle initialiser la valeur, <emplacement de la variable, valeur> > */
        std::map<unsigned long, std::map<std::string, ContentContexte> >    predefined_values;

        /** Liste des emplacement des parametres pour la fonction (eax, [esp], ...) */
        /** Laisser vide pour utiliser l'emplacement par defaut */
        std::list<std::string>                                               emplacement_param;

        /** Emplacement de la valeur de retour pour la fonction (eax, [esp], ...) */
        /** Laisser vide pour utiliser l'emplacement par defaut */
        std::string                                                          emplacement_return;
    };


    /**
    ** \enum eOS
    ** \brief Enum permettant de decrire le type d'OS prevu pour l'executable
    */
    enum    eOS
    {
        #define    NAME_OS_SYSV          "SYSV"
        OS_SYSV,
        #define    NAME_OS_HPUX          "HPUX"
        OS_HPUX,
        #define    NAME_OS_NETBSD        "NETBSD"
        OS_NETBSD,
        #define    NAME_OS_GNU           "GNU"
        OS_GNU,
        #define    NAME_OS_LINUX         "LINUX"
        OS_LINUX,
        #define    NAME_OS_SOLARIS       "SOLARIS"
        OS_SOLARIS,
        #define    NAME_OS_AIX           "AIX"
        OS_AIX,
        #define    NAME_OS_IRIX          "IRIX"
        OS_IRIX,
        #define    NAME_OS_FREEBSD       "FREEBSD"
        OS_FREEBSD,
        #define    NAME_OS_TRU64         "TRU64"
        OS_TRU64,
        #define    NAME_OS_MODESTO       "MODESTO"
        OS_MODESTO,
        #define    NAME_OS_OPENBSD       "OPENBSD"
        OS_OPENBSD,
        #define    NAME_OS_RM_AEABI      "RM_AEABI"
        OS_ARM_AEABI,
        #define    NAME_OS_ARM           "ARM"
        OS_ARM,
        #define    NAME_OS_STANDALONE    "STANDALONE"
        OS_STANDALONE,
        #define    NAME_OS_NONE          "NONE"
        OS_NONE
    };

    /**
    ** \enum eArchi
    ** \brief Enum permettant de decrire l'architecture prevu pour l'executable
    */
    enum eArchi
    {
        #define    NAME_ARCHI_COREWAR        "COREWAR"
        ARCHI_COREWAR,        /* Assembleur du corewar EPITECH */
        #define    NAME_ARCHI_BRAINFUCK      "BRAINFUCK"
        ARCHI_BRAINFUCK,      /* Assembleur Brainfuck standard */
        #define    NAME_ARCHI_BRAINLOLLER    "BRAINLOLLER"
        ARCHI_BRAINLOLLER,    /* Assembleur Brainloller standard */
        #define    NAME_ARCHI_I16            "I16"
        ARCHI_I16,
        #define    NAME_ARCHI_I32            "I32"
        ARCHI_I32,
        #define    NAME_ARCHI_I64            "I64"
        ARCHI_I64,


        ARCHI_M32,            /* AT&T WE 32100 */
        ARCHI_SPARC,          /* SUN SPARC */
        ARCHI_68K,            /* Motorola m68k family */
        ARCHI_88K,            /* Motorola m88k family */
        ARCHI_860,            /* Intel 80860 */
        ARCHI_MIPS,           /* MIPS R3000 big-endian */
        ARCHI_S370,           /* IBM System/370 */
        ARCHI_MIPS_RS3_LE,    /* MIPS R3000 little-endian */
        ARCHI_PARISC,         /* HPPA */
        ARCHI_VPP500,         /* Fujitsu VPP500 */
        ARCHI_SPARC32PLUS,    /* Sun's "v8plus" */
        ARCHI_960,            /* Intel 80960 */
        ARCHI_PPC,            /* PowerPC */
        ARCHI_PPC64,          /* PowerPC 64-bit */
        ARCHI_S390,           /* IBM S390 */
        ARCHI_V800,           /* NEC V800 series */
        ARCHI_FR20,           /* Fujitsu FR20 */
        ARCHI_RH32,           /* TRW RH-32 */
        ARCHI_RCE,            /* Motorola RCE */
        ARCHI_ARM,            /* ARM */
        ARCHI_FAKE_ALPHA,     /* Digital Alpha */
        ARCHI_SH,             /* Hitachi SH */
        ARCHI_SPARCV9,        /* SPARC v9 64-bit */
        ARCHI_TRICORE,        /* Siemens Tricore */
        ARCHI_ARC,            /* Argonaut RISC Core */
        ARCHI_H8_300,         /* Hitachi H8/300 */
        ARCHI_H8_300H,        /* Hitachi H8/300H */
        ARCHI_H8S,            /* Hitachi H8S */
        ARCHI_H8_500,         /* Hitachi H8/500 */
        ARCHI_IA_64,          /* Intel Merced */
        ARCHI_MIPS_X,         /* Stanford MIPS-X */
        ARCHI_COLDFIRE,       /* Motorola Coldfire */
        ARCHI_68HC12,         /* Motorola M68HC12 */
        ARCHI_MMA,            /* Fujitsu MMA Multimedia Accelerator*/
        ARCHI_PCP,            /* Siemens PCP */
        ARCHI_NCPU,           /* Sony nCPU embeeded RISC */
        ARCHI_NDR1,           /* Denso NDR1 microprocessor */
        ARCHI_STARCORE,       /* Motorola Start*Core processor */
        ARCHI_ME16,           /* Toyota ME16 processor */
        ARCHI_ST100,          /* STMicroelectronic ST100 processor */
        ARCHI_TINYJ,          /* Advanced Logic Corp. Tinyj emb.fam*/
        ARCHI_X86_64,         /* AMD x86-64 architecture */
        ARCHI_PDSP,           /* Sony DSP Processor */
        ARCHI_FX66,           /* Siemens FX66 microcontroller */
        ARCHI_ST9PLUS,        /* STMicroelectronics ST9+ 8/16 mc */
        ARCHI_ST7,            /* STmicroelectronics ST7 8 bit mc */
        ARCHI_68HC16,         /* Motorola MC68HC16 microcontroller */
        ARCHI_68HC11,         /* Motorola MC68HC11 microcontroller */
        ARCHI_68HC08,         /* Motorola MC68HC08 microcontroller */
        ARCHI_68HC05,         /* Motorola MC68HC05 microcontroller */
        ARCHI_SVX,            /* Silicon Graphics SVx */
        ARCHI_ST19,           /* STMicroelectronics ST19 8 bit mc */
        ARCHI_VAX,            /* Digital VAX */
        ARCHI_CRIS,           /* Axis Communications 32-bit embedded processor */
        ARCHI_JAVELIN,        /* Infineon Technologies 32-bit embedded processor */
        ARCHI_FIREPATH,       /* Element 14 64-bit DSP Processor */
        ARCHI_ZSP,            /* LSI Logic 16-bit DSP Processor */
        ARCHI_MMIX,           /* Donald Knuth's educational 64-bit processor */
        ARCHI_HUANY,          /* Harvard University machine-independent object files */
        ARCHI_PRISM,          /* SiTera Prism */
        ARCHI_AVR,            /* Atmel AVR 8-bit microcontroller */
        ARCHI_FR30,           /* Fujitsu FR30 */
        ARCHI_D10V,           /* Mitsubishi D10V */
        ARCHI_D30V,           /* Mitsubishi D30V */
        ARCHI_V850,           /* NEC v850 */
        ARCHI_M32R,           /* Mitsubishi M32R */
        ARCHI_MN10300,        /* Matsushita MN10300 */
        ARCHI_MN10200,        /* Matsushita MN10200 */
        ARCHI_PJ,             /* picoJava */
        ARCHI_OPENRISC,       /* OpenRISC 32-bit embedded processor */
        ARCHI_ARC_A5,         /* ARC Cores Tangent-A5 */
        ARCHI_XTENSA,         /* Tensilica Xtensa Architecture */
        #define    NAME_ARCHI_NONE         "NONE"
        ARCHI_NONE
    };

    /**
    ** \enum eFormat
    ** \brief Enum permettant de decrire le format de l'executable
    */
    enum eFormat
    {
        #define    NAME_FORMAT_COREWAR        "COREWAR"
        FORMAT_COREWAR,        /* Format d'executables du corewar EPITECH */
        #define    NAME_FORMAT_BRAINFUCK      "BRAINFUCK"
        FORMAT_BRAINFUCK,
        #define    NAME_FORMAT_BRAINLOLLER    "BRAINLOLLER"
        FORMAT_BRAINLOLLER,
        #define    NAME_FORMAT_ELF32          "ELF32"
        FORMAT_ELF32,
        #define    NAME_FORMAT_ELF64          "ELF64"
        FORMAT_ELF64,
        #define    NAME_FORMAT_DOS            "DOS"
        FORMAT_DOS,
        #define    NAME_FORMAT_PE32           "PE32"
        FORMAT_PE32,
        #define    NAME_FORMAT_PE64           "PE64"
        FORMAT_PE64,
        #define    NAME_FORMAT_MBR            "MBR"
        FORMAT_MBR,
        #define    NAME_FORMAT_NONE           "NONE"
        FORMAT_NONE
    };

    /**
    ** \enum eEndian
    ** \brief Enum permettant de decrire l'endian de l'executable
    */
    enum eEndian
    {
        #define    NAME_ENDIAN_LITTLE    "LITTLE"
        ENDIAN_LITTLE,
        #define    NAME_ENDIAN_BIG       "BIG"
        ENDIAN_BIG,
        #define    NAME_ENDIAN_NONE      "NONE"
        ENDIAN_NONE
    };

public:
    /**
    ** \fn Info()
    ** \brief Constructeur par defaut de la classe Info
    */
    Info();

    /**
    ** \fn ~Info()
    ** \brief Destructeur par defaut de la classe Info
    */
    ~Info();

private:
    /**
    ** \fn Info(const Info &i)
    ** \brief Constructeur par copie de la classe Info (Inutilisable)
    */
    Info(const Info &);

    /**
    ** \fn Info& operator = (const Info &)
    ** \brief Surcharge de l'operateur = pour la classe Info (Inutilisable)
    */
    Info&    operator = (const Info &);

public:
    /**
    ** \fn void clear()
    ** \brief Gere la suppression des infos du fichier a analyser
    **
    ** \return Retourne rien
    */
    void    clear();

    /**
    ** \fn void clear_plugin()
    ** \brief Gere la suppression des plugins.
    **
    ** \return Retourne rien
    */
    void    clear_plugin();

    /**
    ** \fn void clear_file()
    ** \brief Gere la suppression du contenu du fichier
    **
    ** \return Retourne rien
    */
    void    clear_file();

    /**
    ** \fn void clear_segment()
    ** \brief Gere la suppression des segments, symboles et points d'entrees
    **
    ** \return Retourne rien
    */
    void    clear_segment();

    /**
    ** \fn void clear_function()
    ** \brief Gere la suppression des fonctions decompilees
    **
    ** \return Retourne rien
    */
    void    clear_function();

    /**
    ** \fn void clear_proto()
    ** \brief Gere la suppression des definitions, interruption, ...
    **
    ** \return Retourne rien
    */
    void    clear_proto();

    /**
    ** \fn void clear_analyse()
    ** \brief Gere la suppression des analyse ayant ete faites par les plugins
    **
    ** \return Retourne rien
    */
    void    clear_analyse();

public:
    /** Set des plugins a utiliser */
    std::map<std::string, Plugin*>                                plugin;
    /** Tableau contenant les options a faire passer aux plugins */
    std::map<std::string, std::map<std::string, std::string> >    option_plugin;

    /** Nom de l'executable a analyser */
    std::string                                filename;
    /** Contenu du fichier */
    unsigned char                              *data;
    /** Taille du fichier */
    unsigned long                              size;

    /** Systeme prevu pour l'executable */
    Info::eOS                                  os;
    /** Architecture prevue pour l'executable */
    Info::eArchi                               archi;
    /** Format de l'executable */
    Info::eFormat                              format;
    /** Type d'endian de l'executable */
    Info::eEndian                              endian;
    /** Adresses des points d'entrees a partir dequels analyser le programme */
    std::set<unsigned long>                    entry;
    /** Classe contenant les sections de l'executable une fois mappees en memoire */
    Section                                    sec;
    /** Classe contenant les symboles de l'executable */
    Symbole                                    sym;

    /** Tableau contenant les fonctions deassemblee indexee grace a leur adresse */
    std::map<unsigned long, Fonction *>        function;
    /** Tableau contenant les valeurs predefinie des fonctions */
    std::map<unsigned long, VariablesFunc*>    variablesFunc;

    /** Classe contenant les pointeurs de fonctions utile au deassemblage et a l'analyse */
    PtrUsefulFunc                              ptr_func;

    /** Classe contenant la table des interruptions SYSCALL */
    Syscall                                    syscall;
    /** Classe contenant la table des interruptions INT */
    Syscall                                    interrupt;
    /** Classe contenant la table des prototypes de fonctions */
    PrototypeF                                 proto_func;
    /** Classe contenant la table des definitions de structures */
    PrototypeT                                 proto_var;

    /** Gestionnaire de resultats d'analyse */
    Analyse                                    analyses;

    /** Mutex permettant d'utiliser la structure de maniere plus sure */
    Mutex                                      mutex;
};

#endif

