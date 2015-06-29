/*
** Section.hpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 15:28:10 joran herve
** Last update Sat, 27 Jun 2015 15:28:10 joran herve
**
** Status : OK (thread-safe)
*/

#ifndef    SECTION_HPP_
#define    SECTION_HPP_

#include    <string.h>
#include    <vector>

#include    "Mutex.hpp"


/*
** Defines des droits des sections d'un programme en cours d'execution
*/
#define    SECTION_R    1    /** Section accessible en lecture */
#define    SECTION_W    2    /** Section accessible en ecriture */
#define    SECTION_X    4    /** Section executable */
#define    SECTION_A    8    /** Section allouable dynamiquement */

/**
** \class Section
** \brief Classe servant de getionnaire de sections des executables a analyser
*/
class    Section
{
    class    Sec
    {
    public:
        /**
        ** \fn Sec()
        ** \brief Constructeur par defaut d'une section
        */
        Sec();

        /**
        ** \fn Sec(unsigned long addr, unsigned long size, unsigned char *content, int flag)
        ** \brief Constructeur d'une section precisant ses infos
        **
        ** \param addr Adresse de la section en memoire
        ** \param size Taille de la section en memoire
        ** \param content Buffer contenant les infos (deja allouee dynamiquement aux bonnes dimensions)
        ** \param flag Flag decrivant les droits de la section en memoire
        */
        Sec(unsigned long addr, unsigned long size, unsigned char *content, int flag);

        /**
        ** \fn ~Sec()
        ** \brief Destructeur de la section
        */
        ~Sec();
        
        
        /**
        ** \fn int update_content(unsigned long addr_content, unsigned long size_content,
        **                        const unsigned char *content)
        ** \brief Gere l'actualisation du contenu d'une section (utile en cas se superposition de section)
        **
        ** \param addr_content Adresse de debut des donnees a actualiser
        ** \param size_content Taille des donnees a actualiser
        ** \param content Nouvelles donnees pouvant empieter sur la section
        ** \return Retourne 1 si la section a ete modifiee, 0 sinon
        */
        int    update_content(unsigned long addr_content, unsigned long size_content,
                              const unsigned char *content);

    private:
        /**
        ** \fn Sec(const Section::Sec&)
        ** \brief Constructeur par copie d'une section
        */
        Sec(const Section::Sec&);

        /**
        ** \fn Section::Sec &operator = (const Section::Sec&)
        ** \brief Surcharge de l'operateur = pour les sections (inutilisable)
        **
        ** \return Retourne une reference sur la section
        */
        Section::Sec    &operator = (const Section::Sec&);

    public:
        unsigned long    _addr;
        unsigned long    _size;
        unsigned char    *_content;
        int              _flag;
    };


public:
    /**
    ** \fn Section()
    ** \brief Constructeur par defaut du gestionnaire de sections
    */
    Section();

    /**
    ** \fn ~Section()
    ** \brief Destructeur du gestionnaire de sections
    */
    ~Section();


    /**
    ** \fn void clear()
    ** \brief Gere le vidage du gestionnaire de section
    **
    ** \return Retourne rien
    */
    void    clear();

    /**
    ** \fn void add_section(unsigned long addr, unsigned long size, unsigned char *content, int flag,
    **                      unsigned long prev_addr=0, unsigned long prev_size=0, int prev_flag=0)
    ** \brief Gere l'ajout d'une section dans le gestionnaire (ou son actualisation si les parametres prev_* sont precises)
    **
    ** \param addr Adresse de la section en memoire
    ** \param size Taille de la section en memoire
    ** \param content Buffer contenant les infos (deja allouee dynamiquement aux bonnes dimensions)
    ** \param flag Flag decrivant les droits de la section en memoire
    ** \param prev_addr Ancienne adresse de la section en memoire
    ** \param prev_size Ancienne taille de la section en memoire
    ** \param prev_flag Flags decrivant les anciens droits de la section en memoire
    ** \return Retourne rien
    */
    void    add_section(unsigned long addr, unsigned long size, unsigned char *content, int flag,
                        unsigned long prev_addr=0, unsigned long prev_size=0, int prev_flag=0);

    /**
    ** \fn void del_section(unsigned long addr, unsigned long size, int flags)
    ** \brief Gere la suppression d'une section du le gestionnaire
    **
    ** \param addr Adresse de la section a supprimer
    ** \param size Taille de la section a supprimer
    ** \param flags Flags de la section a supprimer
    ** \return Retourne rien
    */
    void    del_section(unsigned long addr, unsigned long size, int flags);


    /**
    ** \fn int is_in_section(unsigned long addr) const
    ** \brief Assesseur permettant de savoir si une adresse est en memoire
    **
    ** \param addr Adresse a trouver
    ** \return Retourne 1 si l'adresse est dans une des section, 0 sinon
    */
    int               is_in_section(unsigned long addr) const;

    /**
    ** \fn unsigned long get_char(unsigned long addr) const
    ** \brief Assesseur permettant d'acceder a un char a l'adresse indiquee
    **
    ** \param addr Adresse du char a recuperer
    ** \return Retourne le char si on le trouve, 0 sinon
    */
    unsigned char     get_char(unsigned long addr) const;

    /**
    ** \fn unsigned long get_short(unsigned long addr, int endian) const
    ** \brief Assesseur permettant d'acceder a un short a l'adresse indiquee
    **
    ** \param addr Adresse du short a recuperer
    ** \param endian Endian decrivant comment les nombres sont stockes en memoire (Info::eEndian)
    ** \return Retourne le short si on le trouve, 0 sinon
    */
    unsigned short    get_short(unsigned long addr, int endian) const;

    /**
    ** \fn unsigned long get_int(unsigned long addr, int endian) const
    ** \brief Assesseur permettant d'acceder a un int a l'adresse indiquee
    **
    ** \param addr Adresse de l'int a recuperer
    ** \param endian Endian decrivant comment les nombres sont stockes en memoire (Info::eEndian)
    ** \return Retourne l'int si on le trouve, 0 sinon
    */
    unsigned int      get_int(unsigned long addr, int endian) const;

    /**
    ** \fn unsigned long get_long(unsigned long addr, int endian) const
    ** \brief Assesseur permettant d'acceder a un long a l'adresse indiquee
    **
    ** \param addr Adresse du long a recuperer
    ** \param endian Endian decrivant comment les nombres sont stockes en memoire (Info::eEndian)
    ** \return Retourne le long si on le trouve, 0 sinon
    */
    unsigned long     get_long(unsigned long addr, int endian) const;

    /**
    ** \fn int get_flag(unsigned long addr) const
    ** \brief Assesseur permettant d'acceder aux flags correspondants a l'adresse
    **
    ** \param addr Adresse a trouver
    ** \return Retourne les flags correspondants a l'adresse si OK, 0 sinon
    */
    int               get_flag(unsigned long addr) const;


    /**
    ** \fn unsigned long get_nbs_section() const
    ** \brief Assesseur permettant de connaitre le nombre de sections
    **
    ** \return Retourne le nombre de sections contenues dans le gestionnaire
    */
    unsigned long          get_nbs_section() const;

    /**
    ** \fn const unsigned long get_addr_section(unsigned long n) const
    ** \brief Assesseur permettant de recuperer l'adresse de la N-ieme section
    **
    ** \param n Numero de la section
    ** \return Retourne l'adresse de la section si on la trouve, 0 sinon
    */
    unsigned long          get_addr_section(unsigned long n) const;

    /**
    ** \fn const unsigned long get_size_section(unsigned long n) const
    ** \brief Assesseur permettant de recuperer la taille de la N-ieme section
    **
    ** \param n Numero de la section
    ** \return Retourne la taille de la section si on la trouve, 0 sinon
    */
    unsigned long          get_size_section(unsigned long n) const;

    /**
    ** \fn const unsigned char *get_buffer_section(unsigned long n) const
    ** \brief Assesseur permettant de recuperer le buffer de la N-ieme section
    **
    ** \param n Numero de la section
    ** \return Retourne le buffer de la section si on la trouve, NULL sinon
    */
    const unsigned char    *get_buffer_section(unsigned long n) const;

    /**
    ** \fn int get_flag_section(unsigned long n) const
    ** \brief Assesseur permettant de recuperer les flags de la N-ieme section
    **
    ** \param n Numero de la section
    ** \return Retourne les flags de la section si on la trouve, 0 sinon
    */
    int                    get_flag_section(unsigned long n) const;

protected:
    /**
    ** \fn Section::Sec *find_sec(unsigned long addr) const
    ** \brief Gere la recherche d'une section a partir d'une adresse
    **
    ** \param addr Addresse contenu dans la section a trouver
    ** \return Retourne un pointeur sur la section contenant l'adresse si on la trouve
    */
    Section::Sec    *find_sec(unsigned long addr) const;

    /**
    ** \fn void del_section_no_mutex(unsigned long addr, unsigned long size, int flags)
    ** \brief Gere la suppression d'une section du le gestionnaire
    **
    ** \param addr Adresse de la section a supprimer
    ** \param size Taille de la section a supprimer
    ** \param flags Flags de la section a supprimer
    ** \return Retourne rien
    */
    void    del_section_no_mutex(unsigned long addr, unsigned long size, int flags);

protected:
    /** Liste des sections du programme */
    std::vector<Section::Sec*>    _sec;
    /** Mutex permettant de rendre la classe thread-safe */
    Mutex                         _mutex;
};

#endif

