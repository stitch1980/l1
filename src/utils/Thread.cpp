/*
** Thread.cpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 15:28:10 joran herve
** Last update Sat, 27 Jun 2015 15:28:10 joran herve
**
** Status : OK (thread-safe)
*/

#include    "Thread.hpp"


/**
** \fn Thread()
** \brief Constructeur par defaut d'un Thread
*/
Thread::Thread():
    _thread(),
    _is_running(0),
    _mutex()
{
}
    
/**
** \fn ~Thread()
** \brief Destructeur d'un Thread
*/
Thread::~Thread()
{
    this->stop();
}

/**
** \fn Thread(const Thread&)
** \brief Constructeur par copie du thread (inutilisable)
*/
Thread::Thread(const Thread&):
    _thread(),
    _is_running(0),
    _mutex()
{
}

/**
** \fn Thread &operator = (const Thread&)
** \brief Surcharge de l'operateur = pour les threads (inutilisable)
**
** \return Retourne une reference sur le thread
*/
Thread &Thread::operator = (const Thread&)
{
    return (*this);
}


/**
** \fn int is_running() const
** \brief Fonction permettant de savoir si le thread est lance
**
** \return Retourne 1 si le thread est lance, 0 sinon
*/
int     Thread::is_running() const
{
    return (this->_is_running);
}

    
/**
** \fn void launch(void *(*function)(void*), void *data)
** \brief Fonction permettant de lancer un thread
**
** \param function Fonction a executer
** \param data Parametres a donner a cette fonction
** \return Retourne rien
*/
void    Thread::launch(void *(*function)(void*), void *data)
{
    this->stop();
    this->_mutex.lock();
    
    if (pthread_create(&(this->_thread), NULL, function, data) == 0)
        this->_is_running = 1;
    
    this->_mutex.unlock();
}

/**
** \fn void stop()
** \brief Fonction permettant d'arreter le thread (attend qui'il finisse)
**
** \return Retourne rien
*/
void    Thread::stop()
{
    this->_mutex.lock();
    
    if (this->_is_running > 0)
        pthread_join(this->_thread, NULL);
    this->_is_running = 0;
    
    this->_mutex.unlock();
}

