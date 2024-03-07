#include <stdio.h>
#include <stdlib.h> 
#include "bibliothequeTfm.h"
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#define TAILLE_MAX 100

#ifdef _WIN32
#include <conio.h> 
#else 
#include <termios.h> 
#include <unistd.h> 
#endif
int getch() 
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int afficheMenuAdmin() 
{
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

    int verification = 0; 


    FILE* infoApprenant = fopen(Fichier, "r");
    if (infoApprenant == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 0; 
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
                    return 1; 
                }
            }
        }
    }

    fclose(infoApprenant);
    return 0; 
}

int authentification() {
    char loginSaisi[TAILLE_MAX];
    char mdpSaisi[TAILLE_MAX];
    int c;

    do {
        printf("Login: ");
        fgets(loginSaisi, TAILLE_MAX, stdin);
        loginSaisi[strcspn(loginSaisi, "\n")] = '\0'; 
        if (strlen(loginSaisi) == 0) {
            printf("Le login est obligatoire. Veuillez saisir un login.\n");
        }
    } while (strlen(loginSaisi) == 0);

    printf("Mot de passe: ");

    int i = 0;
    char b;
    while (i < TAILLE_MAX - 1 && (b = getch()) != '\n') {
        putchar('*'); 
        mdpSaisi[i++] = b;
    }
    mdpSaisi[i] = '\0'; 

    printf("\n");

    if(verifierLoginMdp("infoAdmin.txt", loginSaisi, mdpSaisi)) {
        c=1;
        return c;
    } else if(verifierLoginMdp("infoApprenant.txt", loginSaisi, mdpSaisi)) {
        c=2;
        return c;
    } else {
        puts("Login ou mot de passe invalide.");
        return 0;
    }
}

void obtenirNomPrenom(int id, char *nom, char *prenom) 
{
    FILE *fichier = fopen("listeDev.txt", "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier de liste des étudiants.\n");
        return;
    }

    int idFichier;
    while (fscanf(fichier, "%d %s %s", &idFichier, prenom, nom) == 3) {
        if (idFichier == id) {
            fclose(fichier);
            return; 
        }
    }

    fclose(fichier);
    nom[0] = '\0'; 
    prenom[0] = '\0';
}

void marquerPresenceAdmin()
{
    char choix[TAILLE_MAX]; // Utiliser une chaîne de caractères pour stocker l'entrée de l'utilisateur
    do {
        printf("Entrez votre code ou 'q' pour quitter : ");
        scanf("%s", choix);

        if (strcmp(choix, "q") == 0) 
        {
            break;
        }

        int id = atoi(choix); // Convertir la chaîne en entier

        char nom[TAILLE_MAX];
        char prenom[TAILLE_MAX];
        obtenirNomPrenom(id, nom, prenom);

        if (nom[0] == '\0' || prenom[0] == '\0') 
        {
            printf("L'etudiant n'existe pas.\n");
        } else {
            FILE *fichierPresence = fopen("liste_presence.txt", "r");
            if (fichierPresence == NULL) {
                printf("Erreur lors de l'ouverture du fichier de présence.\n");
                return;
            }

            char date[20];
            time_t maintenant = time(NULL);
            struct tm *tm_maintenant = localtime(&maintenant);
            strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm_maintenant);

            int dejaPresent = 0;
            char ligne[TAILLE_MAX];
            while (fgets(ligne, sizeof(ligne), fichierPresence) != NULL) {
                int idFichier;
                sscanf(ligne, "%d", &idFichier);
                if (idFichier == id) 
                {
                    dejaPresent = 1;
                    break;
                }
            }
            fclose(fichierPresence);

            if (dejaPresent) {
                printf("L'étudiant est déjà marqué présent.\n");
            } else {
                fichierPresence = fopen("liste_presence.txt", "a");
                if (fichierPresence == NULL) {
                    printf("Erreur lors de l'ouverture du fichier de présence.\n");
                    return;
                }

                fprintf(fichierPresence, "%d %s %s %s\n", id, prenom, nom, date);
                printf("Presence marquée pour %s %s à %s.\n", prenom, nom, date);

                fclose(fichierPresence);
            }
        }

    } while (1);
}















//void marquerPresenceApprenant(int cl)
 




