/*
** plugin_clean_nomme_fonction.cpp for plugin_clean_nomme_fonction
**
** Made by joran herve
**
** Started on  Sun, 28 Jun 2015 11:05:57 joran herve
** Last update Sun, 28 Jun 2015 11:05:57 joran herve
**
** Status : OK (documentation faite)
*/

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>

#include    "Info.hpp"
#include    "Calcul.hpp"



#define PLUGIN_DESCR_TYPE        PLUGIN_TYPE__CLEAN
#define PLUGIN_DESCR_PRIORITE    "30000"
#define PLUGIN_DESCR_NAME        "plugin_clean_nomme_fonction"
#define PLUGIN_DESCR_VERSION     "0"
#define PLUGIN_NAME              PLUGIN_DESCR_TYPE "." PLUGIN_DESCR_PRIORITE "." PLUGIN_DESCR_NAME "." PLUGIN_DESCR_VERSION

extern "C"
{
    const char    *PLUGIN_FUNC_GET_PLUGIN_NAME();
    int           PLUGIN_FUNC_EXECUTE(Info *info);
    const char    *PLUGIN_FUNC_APROPOS();
}

/*
** Tableau contenant les noms de fonctions utilisables si on ne trouve pas de symboles
*/
const char    *tab_name[] =
{
    "poney",

    "abeille",	"baleine",	"cacatoes",	"daim",		"ecureuil",
    "faisan",	"gavial",	"hamster",	"ibis",		"lama",
    "macaque",	"narval",	"raton_laveur",	"salamandre",	"taon",
    "acarien",	"barracuda",	"cachalot",	"dauphin",	"ecrevisse",
    "faon",	"gazelle",	"hanneton",	"iguane",	"lamantin",
    "mamba",	"nasique",	"renard",	"sanglier",	"tapir",
    "agneau",	"basilic",	"cafard",	"diable_de_tasmanie","effraie",
    "faucon",	"geai",		"hareng",	"impala",	"langouste",
    "manchot",	"ocelot",	"renne",	"sangsue",	"tarentule",
    "aigle",	"becasse",	"caille",	"dinde"		"elan",

    "fauvette",	"gecko",	"harpie",	"jaguar",	"lapin",
    "mandarin",	"oie",		"requin",	"sardine",	"tatou",
    "albatros",	"belette",	"caiman",	"dingo",	"elephant",
    "fennec",	"gibbon" ,	"herisson",	"jument",	"lemming",
    "mandrill",	"opossum",	"rhinoceros",	"saumon",	"taupe",
    "alligator","beluga",	"calmar",	"doryphore",	"elephant_de_mer",
    "flamant",	"girafe",	"hermine",	"kangourou",	"lemurien",
    "mangouste","orang_outan",	"rorqual",	"sauterelle",	"taureau",
    "alouette",	"bernard_l_ermite","cameleon",	"dragon",	"emeu",
    "fouine",	"glouton",	"heron",	"kiwi",		"leopard",

    "mante_religieuse","orignal","rouge_gorge",	"scarabee",	"termite",
    "alpaga",	"bison",	"campagnol",	"dromadaire",	"epaulard",
    "fourmi",	"gnou",		"hippocampe",	"koala",	"libellule",
    "marmotte",	"ornithorynque","roussette",	"scorpion",	"thon",
    "anaconda",	"blaireau",	"canard",	"epervier",	"fourmilier",
    "gobe_mouche","hippopotame", "licorne",	"marsouin",	"orque",
    "ane",	"blatte",	"canari",	"escargot",	"frelon",
    "goeland" ,	"hirondelle",	"lievre",	"martin_pecheur","oryx",
    "anemone",	"boa",		"capricorne",	"espadon",	"furet",
    "gorille",	"homard",	"limace",	"martinet",	"otarie",

    "anguille",	"boeuf",	"carcajou",	"esturgeon",	"grenouille",
    "huppe",	"lion",		"martre",	"ouistiti",	"tigre",
    "antilope",	"bonobo",	"carpe",	"etoile_de_mer","grillon",
    "huitre",	"loir",		"meduse",	"ours",		"tique",
    "ara",	"bouc",		"castor",	"etourneau",	"grive",
    "hyene",	"lombric",	"merle",	"oursin",	"seiche",
    "araignee",	"bouquetin",	"cerf",		"grizzly",	"loriot",
    "mesange",	"serpent",	"tortue",	"vache",	"wallaby",
    "autruche",	"bourdon",	"chacal",	"grue",		"loup",
    "mille_pattes","souris",	"toucan",	"varan",	"wallaby",

    "babouin",	"chameau",	"guepard",	"loutre",	"mite",
    "bousier",	"chamois",	"guepe",	"luciole",	"moineau",
    "brebis",	"charancon",	"lycaon",	"morse",	"puma",
    "brochet",	"chat",		"lynx",		"moucheron",	"punaise",
    "buffle",	"chauve_souris","mouche",	"putois",	"ragondin",
    "busard",	"chenille",	"mouette",	"python",	"raie",
    "buse",	"cheval",	"mouflon",	"rat",		"sphinx",
    "chevre",	"mouton",	"suricate",	"tourterelle",	"vautour",
    "chevreuil","mulet",	"triton",	"ver",		"wombat",
    "chien" ,	"mulot",	"truie",	"verrat",	"Yack",

    "chimpanze","musaraigne",	"truite",	"vipere",	"zebre",
    "chouette",	"mygale",	"vison",	"cloporte",	"panda",	
    "cobaye",	"pangolin",	"veuve_noire",	"cobra",	"panthere",	
    "coccinelle","paon",	"cochon",	"papillon",	"colibri",
    "paresseux","condor",	"pelican",	"coq", 		"perdrix",
    "corbeau",	"perroquet",	"cormoran",	"perruche",	"corneille",
    "phacochere","coucou",	"phoque",	"cougar",	"pie",
    "couleuvre","pieuvre",	"coyote",	"pigeon",	"crabe",
    "pingouin",	"crapaud",	"pintade",	"crecerelle",	"piranha",
    "crevette",	"porc",		"criquet",	"porc_epic",	"crocodile",

    "poule",	"crotale",	"puce",		"cygne",	"puceron",
    NULL,
};


/**
** \fn const char *get_plugin_name()
** \brief Fonction permettant de connaitre le nom du plugin
**
** \return Retourne un pointeur vers le nom du plugin
*/
const char    *PLUGIN_FUNC_GET_PLUGIN_NAME()
{
    return (PLUGIN_NAME);
}

/**
** \fn int execute(Info *info)
** \brief Fonction permettant d'executer le plugin pour modifier les points d'entrees a analyser
**
** \param info Classe contenant les infos du programme a analyser
** \return Retourne 1 si OK, 0 si le plugin n'a pas ete execute et -1 en cas d'erreur
*/
int    PLUGIN_FUNC_EXECUTE(Info *info)
{
    unsigned long    i;

    if (info == NULL)
        return (-1);

    /* Parcours toutes les fonctions a la recherche de fonction sans noms */
    i = 0;
    for (std::map<unsigned long, Fonction*>::iterator it=info->function.begin();
         it!=info->function.end();
         it++)
    {
        if (info->sym.get_name(it->second->get_addr()).size() <= 0)
        {
            /* On cherche un nom de fonction non-utilise */
            while ((tab_name[i] != NULL) && (info->sym.get_addr(std::string("func_") + tab_name[i]) != 0))
                i++;

            /* Si on a trouve un nom, on s'en sert pour nommer la fonction */
            if (tab_name[i] != NULL)
                info->sym.add_symbole(it->second->get_addr(), std::string("func_") + tab_name[i], Symbole::FUNC);

            /* En dernier recours, on genere le nom de la fonction a partir de son adresse */
            else
                info->sym.add_symbole(it->second->get_addr(), Fonction::get_name_function(it->second, info), Symbole::FUNC);
        }
    }

    return (1);
}

/**
** \fn const char *get_plugin_name()
** \brief Fonction permettant de d'acceder a la description du plugin
**
** \return Retourne un pointeur vers la description du plugin
*/
const char    *PLUGIN_FUNC_APROPOS()
{
    return ("Plugin permettant de nommer les fonctions inconnues afin de rendre le code plus lisible.\n"
            "\n"
            "Type : " PLUGIN_DESCR_TYPE "\n"
            "Priorite : " PLUGIN_DESCR_PRIORITE "\n"
            "Nom du plugin : " PLUGIN_DESCR_NAME "\n"
            "Version : " PLUGIN_DESCR_VERSION "\n"
            "\n"
            "Infos necessaires :\n"
            " - Les fonctions a nommer doivent avoir ete deassemblees.\n"
            "\n"
            "Effets :\n"
            " - Cree un symbole a l'adresse de chaque fonction n'en ayant pas deja un.\n"
            "\n"
            "Valeurs de retour :\n"
            " - Retourne 1 si le plugin a pu s'executer correctement.\n"
            " - Retourne -1 en cas d'erreur.\n");
}

