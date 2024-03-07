#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include "bibliothequeTfm.h"
#define TAILLE_MAX 1000

int main()
{
    //marquerPresenceAdmin();
    int choix;
    int sousMenuChoix;
    sousMenuChoix=authentification();
    if(sousMenuChoix==2)
    {
        choix=afficheMenuApprenant();
    }
    else if(sousMenuChoix==1)
    {
        choix=afficheMenuAdmin();
        if(choix==3)
        {
            marquerPresenceAdmin();
        }
    }
   

    return 0;
}



