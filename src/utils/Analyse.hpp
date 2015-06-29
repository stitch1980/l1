/*
** Analyse.hpp for Lecter
**
** Made by joran herve
**
** Started on  Sat, 27 Jun 2015 14:34:37 joran herve
** Last update Sat, 27 Jun 2015 14:34:37 joran herve
**
** Status : OK (thread-safe)
*/

#ifndef    ANALYSE_HPP_
#define    ANALYSE_HPP_

#include    <string>
#include    <map>

#include    "Mutex.hpp"

/**
** \class Analyse
** \brief Classe contenant les analyses effectuees par les plugins (thread-safe)
*/
class    Analyse
{
public:
    /**
    ** \fn Analyse()
    ** \brief Constructeur par defaut de l'analyse
    */
    Analyse();

    /**
    ** \fn Analyse(const std::string &s)
    ** \brief Constructeur d'analyse avec son contenu
    **
    ** \param s Contenu de l'analyse
    */
    Analyse(const std::string &s);

    /**
    ** \fn ~Analyse()
    ** \brief Destructeur de l'analyse
    */
    ~Analyse();

protected:
    /**
    ** \fn Analyse(const Analyse &a)
    ** \brief Constructeur par copie de l'analyse (inutilisable)
    **
    ** \param a Analyse a copier
    */
    Analyse(const Analyse &a);
    
    /**
    ** \fn Analyse &operator = (const Analyse &a)
    ** \brief Surcharge de l'operateur = pour les analyses (inutilisable)
    **
    ** \param a Analyse a copier
    ** \return Retourne une reference sur l'analyse
    */
    Analyse    &operator = (const Analyse &a);

public:
    /**
    ** \fn void clear()
    ** \brief Gere la suppression du contenu de l'analyse
    **
    ** \return Retourne rien
    */
    void             clear();
    
    
    /**
    ** \fn void set_txt_analyse(const std::string &s)
    ** \brief Assesseur permettant de modifier le texte de l'analyse
    **
    ** \param t Nouveau texte de l'analyse
    ** \return Retourne rien
    */
    void             set_txt_analyse(const std::string &s);
    
    /**
    ** \fn std::string get_txt_analyse() const
    ** \brief Assesseur permettant de recuperer le texte de l'analyse
    **
    ** \return Retourne le texte de l'analyse (pas celui des analyses filles)
    */
    std::string      get_txt_analyse() const;
    
    
    /**
    ** \fn void add_analyse_child(const std::string &title, const std::string &content)
    ** \brief Ajoute/remplace une analyse fille
    **
    ** \param title Titre de l'analyse fille
    ** \param content Contenu de l'analyse fille
    ** \return Retourne rien
    */
    void             add_analyse_child(const std::string &title, const std::string &content);
    
    /**
    ** \fn void add_analyse_child(const std::string &title, Analyse *a)
    ** \brief Ajoute/remplace une analyse fille (elle sera delete avec clear() et del_analyse_child(...))
    **
    ** \param title Titre de l'analyse fille
    ** \param a Nouvelle analyse fille
    ** \return Retourne rien
    */
    void             add_analyse_child(const std::string &title, Analyse *a);
    
    /**
    ** \fn void del_analyse_child(const std::string &title)
    ** \brief Gere la suppression d'une analyse fille si elle existe
    **
    ** \param title Titre de l'analyse fille a supprimer
    ** \return Retourne rien
    */
    void             del_analyse_child(const std::string &title);
    
    
    /**
    ** \fn std::string get_txt_analyse_child(const std::string &title) const
    ** \brief Assesseur parmettent d'acceder au contenu d'une analyse fille grace a son titre
    **
    ** \param title Titre de l'analyse fille
    ** \return Retourne le contenu de l'analyse fille si OK, "" sinon
    */
    std::string      get_txt_analyse_child(const std::string &title) const;
    
    /**
    ** \fn Analyse *get_analyse_child(const std::string &title) const
    ** \brief Assesseur parmettent d'acceder a une analyse fille grace a son titre
    **
    ** \param title Titre de l'analyse fille
    ** \return Retourne un pointeur sur l'analyse fille si OK, NULL sinon
    */
    Analyse          *get_analyse_child(const std::string &title) const;
    
    
    /**
    ** \fn unsigned long get_nbs_analyse_child() const
    ** \brief Assesseur permettant de connaitre le nombre d'analyses filles
    **
    ** \return Retourne le nombre d'analyses filles
    */
    unsigned long    get_nbs_analyse_child() const;
    
    /**
    ** \fn std::string get_title_analyse_child_num(unsigned long n) const
    ** \brief Assesseur parmettant d'acceder au titre de la n-ieme analyse fille
    **
    ** \param n Numero de l'analyse fille
    ** \return Retourne le titre de l'analyse fille si OK, "" sinon
    */
    std::string      get_title_analyse_child_num(unsigned long n) const;
    
    /**
    ** \fn std::string get_txt_analyse_child_num(unsigned long n) const
    ** \brief Assesseur parmettant d'acceder au contenu de la n-ieme analyse fille
    **
    ** \param n Numero de l'analyse fille
    ** \return Retourne le contenu de l'analyse fille si OK, "" sinon
    */
    std::string      get_txt_analyse_child_num(unsigned long n) const;
    
    /**
    ** \fn Analyse *get_analyse_child_num(unsigned long n) const
    ** \brief Assesseur parmettant d'acceder a la n-ieme analyse fille
    **
    ** \param n Numero de l'analyse fille
    ** \return Retourne un pointeur sur l'analyse fille si OK, NULL sinon
    */
    Analyse          *get_analyse_child_num(unsigned long n) const;
    
protected:
    /** String contenant l'analyse */
    std::string                        _analyse;
    /** Map contenant les sous-analyses indexees par leur titre */
    std::map<std::string, Analyse*>    _analyse_child;
    
    /** Mutex pour rendre la classe thread-safe */
    Mutex                              _mutex;
};

#endif

