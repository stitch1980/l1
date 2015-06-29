/*
** Mutex.hpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 15:28:10 joran herve
** Last update Sat, 27 Jun 2015 15:28:10 joran herve
**
** Status : OK
*/

#ifndef        MUTEX_HPP_
#define        MUTEX_HPP_

#include    <pthread.h>
#include    <errno.h>

/**
** \class Mutex
** \brief Classe permattant d'utiliser des mutex sans C++11
*/
class    Mutex
{
public:
    Mutex();
    ~Mutex();
    
    /**
    ** \fn int lock()
    ** \brief Verrouille le mutex
    **
    ** \return Retourne 1 si OK, 0 sinon
    */
    int    lock();
    
    /**
    ** \fn int try_lock()
    ** \brief Essai de verrouiller le mutex
    **
    ** \return Retourne 1 si OK, 0 sinon
    */
    int    try_lock();
    
    /**
    ** \fn int unlock()
    ** \brief Libere le mutex
    **
    ** \return Retourne 1 si OK, 0 sinon
    */
    int    unlock();

protected:
    /** Le mutex a encapsuler */
    pthread_mutex_t    _mutex;
};

#endif

