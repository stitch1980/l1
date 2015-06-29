/*
** Mutex.cpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 15:28:10 joran herve
** Last update Sat, 27 Jun 2015 15:28:10 joran herve
**
** Status : OK
*/

#include    "Mutex.hpp"


Mutex::Mutex():
    _mutex()
{
    pthread_mutex_init(&(this->_mutex), NULL);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&(this->_mutex));
}

/**
** \fn int lock()
** \brief Verrouille le mutex
**
** \return Retourne 1 si OK, 0 sinon
*/
int    Mutex::lock()
{
    pthread_mutex_lock(&(this->_mutex));
    return (1);
}

/**
** \fn int try_lock()
** \brief Essai de verrouiller le mutex
**
** \return Retourne 1 si OK, 0 sinon
*/
int    Mutex::try_lock()
{
    if (pthread_mutex_trylock(&(this->_mutex)) == EBUSY)
        return (0);
    return (1);
}

/**
** \fn int unlock()
** \brief Libere le mutex
**
** \return Retourne 1 si OK, 0 sinon
*/
int    Mutex::unlock()
{
    pthread_mutex_unlock(&(this->_mutex));
    return (1);
}

