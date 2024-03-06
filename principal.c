#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include "bibliothequeTfm.h"
#define TAILLE_MAX 1000

int main()
{
    int choix,a,b;
   // int  1; // Exemple de choix de sous-menu
    //marquerPresenceAdmin(choixSousMenu);
    a=sousMenuAdmin(authentification());
    marquerPresenceAdmin(a);

    sousMenuApprenant(authentification());


    return 0;
}
