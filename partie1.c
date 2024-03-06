#include <stdio.h>
#include "bibliothequeTfm.h"
#include <string.h>
#include <unistd.h>
#include <termios.h>
#define TAILLE_MAX 100

int afficheMenuAdmin() {
    int choix;
    do {
        printf("\n");
        printf("\033[1;36m"); // Change la couleur du texte en cyan clair
        printf("┌────────────────────────────────────┐\n");
        printf("│         MENU ADMINISTRATEUR        │\n");
        printf("├────────────────────────────────────┤\n");
        printf("│ 1. GESTION DES ETUDIANTS           │\n");
        printf("│ 2. GENERATION DE FICHIERS          │\n");
        printf("│ 3. MARQUER LES PRESENCES           │\n");
        printf("│ 4. ENVOYER UN MESSAGE              │\n");
        printf("│ 5. QUITTER                         │\n");
        printf("└────────────────────────────────────┘\n");
        printf("\033[0m"); // Réinitialise la couleur du texte
        printf("\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        if (!(choix >= 1 && choix <= 5)) {
            printf("Choix invalide. Veuillez choisir entre 1 et 5.\n");
        }
    } while (!(choix >= 1 && choix <= 5));
    return choix;
}

int afficheMenuApprenant()
{
    int choix;
    do {
        printf("\n");
        printf("\033[1;32m"); // Change la couleur du texte en cyan clair
        printf("┌────────────────────────────────────┐\n");
        printf("│         MENU APPRENANT             │\n");
        printf("├────────────────────────────────────┤\n");
        printf("│ 1. MARQUER MA PRESENCE             │\n");
        printf("│ 2. NOMBRE DE MINUTES D'ABSENCES    │\n");
        printf("│ 3. MES MESSAGES                    │\n");
        printf("│ 4. QUITTER                         │\n");                                                              
        printf("└────────────────────────────────────┘\n");
        printf("\033[0m"); // Réinitialise la couleur du texte
        printf("\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        if (!(choix >= 1 && choix <= 4)) {
            printf("Choix invalide. Veuillez choisir entre 1 et 5.\n");
        }
    } while (!(choix >= 1 && choix <= 4));
    return choix;
}



int verifierLoginMdp(const char* Fichier, const char* loginSaisi, const char* mdpSaisi)
{
    char ligne[TAILLE_MAX];
    char loginFichier[TAILLE_MAX];
    char mdpFichier[TAILLE_MAX];
    char* token;

    int verification = 0; // Initialisation à 0


    FILE* infoApprenant = fopen(Fichier, "r");
    if (infoApprenant == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 0; // Retourner 0 en cas d'erreur
    }

    while (fgets(ligne, TAILLE_MAX, infoApprenant) != NULL) {
        token = strtok(ligne, " ");
        if (token != NULL) {
            strcpy(loginFichier, token);

            token = strtok(NULL, "\n");
            if (token != NULL) {
                strcpy(mdpFichier, token);

                if (strcmp(loginSaisi, loginFichier) == 0 && strcmp(mdpSaisi, mdpFichier) == 0) {
                    fclose(infoApprenant);
                    return 1; // Retourner 1 si les identifiants sont valides
                }
            }
        }
    }

    fclose(infoApprenant);
    return 0; 
}

int authentification()
{
    char loginSaisi[TAILLE_MAX];
    char mdpSaisi[TAILLE_MAX];
    int c;

    do {
        printf("Login: ");
        fgets(loginSaisi, TAILLE_MAX, stdin);
        loginSaisi[strcspn(loginSaisi, "\n")] = '\0'; // Supprime le retour à la ligne
        if (strlen(loginSaisi) == 0) {
            printf("Le login est obligatoire. Veuillez saisir un login.\n");
        }
    } while (strlen(loginSaisi) == 0);

    printf("Mot de passe: ");

    struct termios oldTerm, newTerm;
    tcgetattr(STDIN_FILENO, &oldTerm);
    newTerm = oldTerm;
    newTerm.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerm);

    fgets(mdpSaisi, TAILLE_MAX, stdin);
    mdpSaisi[strcspn(mdpSaisi, "\n")] = '\0'; // Supprime le retour à la ligne

    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);
    printf("\n");

    if(verifierLoginMdp("infoApprenant.txt", loginSaisi, mdpSaisi)) {
        c = afficheMenuApprenant();
        return c;
    } else if(verifierLoginMdp("infoAdmin.txt", loginSaisi, mdpSaisi)) {
        c = afficheMenuAdmin();
        return c;
    } else {
        puts("Login ou mot de passe invalide.");
        return 0;
    }
}









