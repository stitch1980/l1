/*
** Section.cpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 15:28:10 joran herve
** Last update Sat, 27 Jun 2015 15:28:10 joran herve
**
** Status : OK (thread-safe)
*/

#include    "Section.hpp"
#include    "Info.hpp"


/**
** \fn Sec()
** \brief Constructeur par defaut d'une section
*/
Section::Sec::Sec():
    _addr(0),
    _size(0),
    _content(NULL),
    _flag(0)
{
}

/**
** \fn Sec(unsigned long addr, unsigned long size, unsigned char *content, int flag)
** \brief Constructeur d'une section precisant ses infos
**
** \param addr Adresse de la section en memoire
** \param size Taille de la section en memoire
** \param content Buffer contenant les infos (deja allouee dynamiquement aux bonnes dimensions)
** \param flag Flag decrivant les droit de la section en memoire
*/
Section::Sec::Sec(unsigned long addr, unsigned long size, unsigned char *content, int flag):
    _addr(addr),
    _size(size),
    _content(content),
    _flag(flag)
{
}

/**
** \fn ~Sec()
** \brief Destructeur de la section
*/
Section::Sec::~Sec()
{
    if (this->_content != NULL)
        delete[] this->_content;
}

/**
** \fn Sec(const Section::Sec&)
** \brief Constructeur par copie d'une section
*/
Section::Sec::Sec(const Section::Sec&):
    _addr(0),
    _size(0),
    _content(NULL),
    _flag(0)
{
}

/**
** \fn Section::Sec &operator = (const Section::Sec&)
** \brief Surcharge de l'operateur = pour les sections (inutilisable)
**
** \return Retourne une reference sur la section
*/
Section::Sec    &Section::Sec::operator = (const Section::Sec&)
{
    return (*this);
}
        
        
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
int    Section::Sec::update_content(unsigned long addr_content, unsigned long size_content,
                                    const unsigned char *content)
{
    /* Les nouvelles donnes empiettent-elles sur la section ? */
    if (((this->_addr > addr_content) && (this->_addr > (addr_content + size_content))) ||
         ((this->_addr + this->_size) < addr_content))
        return (0);
    
    for (unsigned long i=0; i<size_content; i++)
    {
        /* Si l'octet est comprit dans la section, on actualise cette derniere */
        if ((this->_addr <= (addr_content + i)) &&
            ((this->_addr + this->_size) > (addr_content + i)))
        {
            this->_content[addr_content + i - this->_addr] = content[i];
        }
    }
    return (1);
}




/**
** \fn Section()
** \brief Constructeur par defaut du gestionnaire de sections
*/
Section::Section():
    _sec(),
    _mutex()
{
}

/**
** \fn ~Section()
** \brief Destructeur du gestionnaire de sections
*/
Section::~Section()
{
    this->clear();
}


/**
** \fn void clear()
** \brief Gere le vidage du gestionnaire de section
**
** \return Retourne rien
*/
void    Section::clear()
{
    std::vector<Section::Sec*>::iterator    it;

    this->_mutex.lock();

    for (it=this->_sec.begin(); it!=this->_sec.end(); it++)
        delete (*it);
    this->_sec.clear();
    
    this->_mutex.unlock();
}

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
void    Section::add_section(unsigned long addr, unsigned long size, unsigned char *content, int flag,
                             unsigned long prev_addr, unsigned long prev_size, int prev_flag)
{
    Section::Sec    *s;

    if ((content != NULL) && (size > 0))
    {
        /* Suppression des sections identiques et de la section precedente si besoin est */
        this->del_section_no_mutex(addr, size, flag);
        if (prev_size > 0)
            this->del_section_no_mutex(prev_addr, prev_size, prev_flag);
        
        /* Creation de la nouvelle section */
        if ((s = new Section::Sec(addr, size, content, flag)) != NULL)
        {
            this->_mutex.lock();

            for (unsigned long i=0; i<this->_sec.size(); i++)
                this->_sec[i]->update_content(addr, size, content);
            this->_sec.push_back(s);
            
            this->_mutex.unlock();
        }
    }
}

/**
** \fn void del_section(unsigned long addr, unsigned long size, int flags)
** \brief Gere la suppression d'une section du le gestionnaire
**
** \param addr Adresse de la section a supprimer
** \param size Taille de la section a supprimer
** \param flags Flags de la section a supprimer
** \return Retourne rien
*/
void    Section::del_section(unsigned long addr, unsigned long size, int flags)
{
    this->_mutex.lock();
    this->del_section_no_mutex(addr, size, flags);
    this->_mutex.unlock();
}


/**
** \fn int is_in_section(unsigned long addr) const
** \brief Assesseur permettant de savoir si une adresse est en memoire
**
** \param addr Adresse a trouver
** \return Retourne 1 si l'adresse est dans une des section, 0 sinon
*/
int               Section::is_in_section(unsigned long addr) const
{
    int    ret;
    
    const_cast<Mutex*>(&(this->_mutex))->lock();

    ret = 0;
    if (this->find_sec(addr) != NULL)
        ret = 1;

    const_cast<Mutex*>(&(this->_mutex))->unlock();
    return (ret);
}

/**
** \fn unsigned long get_char(unsigned long addr) const
** \brief Assesseur permettant d'acceder a un char a l'adresse indiquee
**
** \param addr Adresse du char a recuperer
** \return Retourne le char si on le trouve, 0 sinon
*/
unsigned char     Section::get_char(unsigned long addr) const
{
    Section::Sec     *s;
    unsigned char    ret;

    const_cast<Mutex*>(&(this->_mutex))->lock();

    if ((s = this->find_sec(addr)) == NULL)
        ret = 0;
    else
        ret = s->_content[addr - s->_addr];

    const_cast<Mutex*>(&(this->_mutex))->unlock();
    return (ret);
}

/**
** \fn unsigned long get_short(unsigned long addr, int endian) const
** \brief Assesseur permettant d'acceder a un short a l'adresse indiquee
**
** \param addr Adresse du short a recuperer
** \param endian Endian decrivant comment les nombres sont stockes en memoire (Info::eEndian)
** \return Retourne le short si on le trouve, 0 sinon
*/
unsigned short    Section::get_short(unsigned long addr, int endian) const
{
    Section::Sec      *s;
    unsigned short    res;

    const_cast<Mutex*>(&(this->_mutex))->lock();
    if ((s = this->find_sec(addr)) == NULL)
    {
        const_cast<Mutex*>(&(this->_mutex))->unlock();
        return (0);
    }
    const_cast<Mutex*>(&(this->_mutex))->unlock();

    /* On recupere le nombre octet par octets en fonction de son endian */
    res = 0;
    if (endian == Info::ENDIAN_BIG)
    {
        res = this->get_char(addr);
        res = (res << 8) + this->get_char(addr + 1);
    }
    else
    {
        res = this->get_char(addr + 1);
        res = (res << 8) + this->get_char(addr);
    }
    
    return (res);
}

/**
** \fn unsigned long get_int(unsigned long addr, int endian) const
** \brief Assesseur permettant d'acceder a un int a l'adresse indiquee
**
** \param addr Adresse de l'int a recuperer
** \param endian Endian decrivant comment les nombres sont stockes en memoire (Info::eEndian)
** \return Retourne l'int si on le trouve, 0 sinon
*/
unsigned int      Section::get_int(unsigned long addr, int endian) const
{
    Section::Sec    *s;
    unsigned int    res;

    const_cast<Mutex*>(&(this->_mutex))->lock();
    if ((s = this->find_sec(addr)) == NULL)
    {
        const_cast<Mutex*>(&(this->_mutex))->unlock();
        return (0);
    }
    const_cast<Mutex*>(&(this->_mutex))->unlock();

    /* On recupere le nombre octet par octets en fonction de son endian */
    res = 0;
    if (endian == Info::ENDIAN_BIG)
    {
        res = this->get_char(addr);
        res = (res << 8) + this->get_char(addr + 1);
        res = (res << 8) + this->get_char(addr + 2);
        res = (res << 8) + this->get_char(addr + 3);
    }
    else
    {
        res = this->get_char(addr + 3);
        res = (res << 8) + this->get_char(addr + 2);
        res = (res << 8) + this->get_char(addr + 1);
        res = (res << 8) + this->get_char(addr);
    }

    return (res);
}

/**
** \fn unsigned long get_long(unsigned long addr, int endian) const
** \brief Assesseur permettant d'acceder a un long a l'adresse indiquee
**
** \param addr Adresse du long a recuperer
** \param endian Endian decrivant comment les nombres sont stockes en memoire (Info::eEndian)
** \return Retourne le long si on le trouve, 0 sinon
*/
unsigned long     Section::get_long(unsigned long addr, int endian) const
{
    Section::Sec     *s;
    unsigned long    res;

    const_cast<Mutex*>(&(this->_mutex))->lock();
    if ((s = this->find_sec(addr)) == NULL)
    {
        const_cast<Mutex*>(&(this->_mutex))->unlock();
        return (0);
    }
    const_cast<Mutex*>(&(this->_mutex))->unlock();

    /* On recupere le nombre octet par octets en fonction de son endian */
    res = 0;
    if (endian == Info::ENDIAN_BIG)
    {
        res = this->get_char(addr);
        res = (res << 8) + this->get_char(addr + 1);
        res = (res << 8) + this->get_char(addr + 2);
        res = (res << 8) + this->get_char(addr + 3);
        res = (res << 8) + this->get_char(addr + 4);
        res = (res << 8) + this->get_char(addr + 5);
        res = (res << 8) + this->get_char(addr + 6);
        res = (res << 8) + this->get_char(addr + 7);
    }
    else
    {
        res = this->get_char(addr + 7);
        res = (res << 8) + this->get_char(addr + 6);
        res = (res << 8) + this->get_char(addr + 5);
        res = (res << 8) + this->get_char(addr + 4);
        res = (res << 8) + this->get_char(addr + 3);
        res = (res << 8) + this->get_char(addr + 2);
        res = (res << 8) + this->get_char(addr + 1);
        res = (res << 8) + this->get_char(addr);
    }

    return (res);
}

/**
** \fn int get_flag(unsigned long addr) const
** \brief Assesseur permettant d'acceder aux flags correspondants a l'adresse
**
** \param addr Adresse a trouver
** \return Retourne les flags correspondants a l'adresse si OK, 0 sinon
*/
int               Section::get_flag(unsigned long addr) const
{
    std::vector<Section::Sec*>::const_iterator    it;
    int                                           flag;

    const_cast<Mutex*>(&(this->_mutex))->lock();

    flag = 0;
    for (it=this->_sec.begin(); it!=this->_sec.end(); it++)
    {
        if (((*it)->_addr <= addr) && (addr < ((*it)->_addr + (*it)->_size)))
            flag = flag | (*it)->_flag;
    }

    const_cast<Mutex*>(&(this->_mutex))->unlock();
    return (flag);
}


/**
** \fn unsigned long get_nbs_section() const
** \brief Assesseur permettant de connaitre le nombre de sections
**
** \return Retourne le nombre de sections contenues dans le gestionnaire
*/
unsigned long          Section::get_nbs_section() const
{
    return (this->_sec.size());
}

/**
** \fn const unsigned long get_addr_section(unsigned long n) const
** \brief Assesseur permettant de recuperer l'adresse de la N-ieme section
**
** \param n Numero de la section
** \return Retourne l'adresse de la section si on la trouve, 0 sinon
*/
unsigned long          Section::get_addr_section(unsigned long n) const
{
    unsigned long    ret;

    const_cast<Mutex*>(&(this->_mutex))->lock();
    if (n >= this->_sec.size())
        ret = 0;
    else
        ret = this->_sec[n]->_addr;
    const_cast<Mutex*>(&(this->_mutex))->unlock();
    
    return (ret);
}

/**
** \fn const unsigned long get_size_section(unsigned long n) const
** \brief Assesseur permettant de recuperer la taille de la N-ieme section
**
** \param n Numero de la section
** \return Retourne la taille de la section si on la trouve, 0 sinon
*/
unsigned long          Section::get_size_section(unsigned long n) const
{
    unsigned long    ret;

    const_cast<Mutex*>(&(this->_mutex))->lock();
    if (n >= this->_sec.size())
        ret = 0;
    else
        ret = this->_sec[n]->_size;
    const_cast<Mutex*>(&(this->_mutex))->unlock();
    
    return (ret);
}

/**
** \fn const unsigned char *get_buffer_section(unsigned long n) const
** \brief Assesseur permettant de recuperer le buffer de la N-ieme section
**
** \param n Numero de la section
** \return Retourne le buffer de la section si on la trouve, NULL sinon
*/
const unsigned char    *Section::get_buffer_section(unsigned long n) const
{
    unsigned char    *ret;

    const_cast<Mutex*>(&(this->_mutex))->lock();
    if (n >= this->_sec.size())
        ret = NULL;
    else
        ret = this->_sec[n]->_content;
    const_cast<Mutex*>(&(this->_mutex))->unlock();
    
    return (ret);
}

/**
** \fn int get_flag_section(unsigned long n) const
** \brief Assesseur permettant de recuperer les flags de la N-ieme section
**
** \param n Numero de la section
** \return Retourne les flags de la section si on la trouve, 0 sinon
*/
int                    Section::get_flag_section(unsigned long n) const
{
    int    ret;

    const_cast<Mutex*>(&(this->_mutex))->lock();
    if (n >= this->_sec.size())
        ret = 0;
    else
        ret = this->_sec[n]->_flag;
    const_cast<Mutex*>(&(this->_mutex))->unlock();
    
    return (ret);
}


/**
** \fn Section::Sec *find_sec(unsigned long addr) const
** \brief Gere la recherche d'une section a partir d'une adresse
**
** \param addr Addresse contenu dans la section a trouver
** \return Retourne un pointeur sur la section contenant l'adresse si on la trouve
*/
Section::Sec    *Section::find_sec(unsigned long addr) const
{
    std::vector<Section::Sec*>::const_iterator    it;

    for (it=this->_sec.begin(); it!=this->_sec.end(); it++)
    {
        if (((*it)->_addr <= addr) && (addr < ((*it)->_addr + (*it)->_size)))
            return (*it);
    }

    return (NULL);
}

/**
** \fn void del_section_no_mutex(unsigned long addr, unsigned long size, int flags)
** \brief Gere la suppression d'une section du le gestionnaire
**
** \param addr Adresse de la section a supprimer
** \param size Taille de la section a supprimer
** \param flags Flags de la section a supprimer
** \return Retourne rien
*/
void    Section::del_section_no_mutex(unsigned long addr, unsigned long size, int flags)
{
    std::vector<Section::Sec*>::iterator    it;

    for (it=this->_sec.begin(); it!=this->_sec.end(); )
    {
        if (((*it)->_addr == addr) && ((*it)->_size == size) && ((*it)->_flag == flags))
        {
            delete (*it);
            this->_sec.erase(it);
            it = this->_sec.begin();
        }
        else
            it++;
    }
}

