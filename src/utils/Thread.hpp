/*
** Thread.hpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 15:28:10 joran herve
** Last update Sat, 27 Jun 2015 15:28:10 joran herve
**
** Status : OK (thread-safe)
*/

#ifndef    THREAD_HPP_
#define    THREAD_HPP_

#include    <pthread.h>
#include    "Mutex.hpp"

/**
** \class Thread
** \brief Classe permettant d'utiliser des threads (le reste des modules doit etre thread-safe)
*/
class    Thread
{
public:
    /**
    ** \fn Thread()
    ** \brief Constructeur par defaut d'un Thread
    */
    Thread();
    
    /**
    ** \fn ~Thread()
    ** \brief Destructeur d'un Thread
    */
    ~Thread();

protected:
    /**
    ** \fn Thread(const Thread&)
    ** \brief Constructeur par copie du thread (inutilisable)
    */
    Thread(const Thread&);
    /**
    ** \fn Thread &operator = (const Thread&)
    ** \brief Surcharge de l'operateur = pour les threads (inutilisable)
    **
    ** \return Retourne une reference sur le thread
    */
    Thread &operator = (const Thread&);

public:
    /**
    ** \fn int is_running() const
    ** \brief Fonction permettant de savoir si le thread est lance
    **
    ** \return Retourne 1 si le thread est lance, 0 sinon
    */
    int     is_running() const;
    
    /**
    ** \fn void launch(void *(*function)(void*), void *data)
    ** \brief Fonction permettant de lancer un thread
    **
    ** \param function Fonction a executer
    ** \param data Parametres a donner a cette fonction
    ** \return Retourne rien
    */
    void    launch(void *(*function)(void*), void *data);
    
    /**
    ** \fn void stop()
    ** \brief Fonction permettant d'arreter le thread (attend qui'il finisse)
    **
    ** \return Retourne rien
    */
    void    stop();

protected:
    /** Thread POSIX a utiliser */
    pthread_t    _thread;
    /** Vaut 1 si le thread est lancer */
    int          _is_running;
    
    /** Mutex rendant la classe de Thread thread-safe */
    Mutex        _mutex;
};

#endif

