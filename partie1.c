#include <stdio.h>
#include "bibliothequeTfm.h"
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
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
        loginSaisi[strcspn(loginSaisi, "\n")] = '\0'; 
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
    mdpSaisi[strcspn(mdpSaisi, "\n")] = '\0'; 

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

int sousMenuAdmin(int c)
{
    int ch;
    if(c==3)
    {
        puts("classes:");
        puts("1-Dev web/mobile");
        puts("2-Dev Data");
        puts("3-Referent digital");
        scanf("%d",&ch);
    }
    return ch;
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

void marquerPresenceId(int id, const char *nomFichier) 
{
    FILE *fichierPresence = fopen("liste_presence.txt", "r+");
    if (fichierPresence == NULL) {
        printf("Erreur lors de l'ouverture du fichier de présence.\n");
        return;
    }

    fseek(fichierPresence, 0, SEEK_END);
    int tailleFichier = ftell(fichierPresence);
    if (tailleFichier == 0) {
        char date[20];
        time_t maintenant = time(NULL);
        struct tm *tm_maintenant = localtime(&maintenant);
        strftime(date, sizeof(date), "%Y-%m-%d", tm_maintenant);
        fprintf(fichierPresence, "Date:%s\n", date);
    } else {
        fseek(fichierPresence, -21, SEEK_END); // Se déplacer avant la fin pour lire la date
        char datePrecedente[20];
        fscanf(fichierPresence, "Date:%s\n", datePrecedente);

        // Lire la date actuelle
        char dateActuelle[20];
        time_t maintenant = time(NULL);
        struct tm *tm_maintenant = localtime(&maintenant);
        strftime(dateActuelle, sizeof(dateActuelle), "%Y-%m-%d", tm_maintenant);

        // Comparer les dates
        if (strcmp(dateActuelle, datePrecedente) != 0) {
            // Les dates sont différentes, écrire la nouvelle date
            fseek(fichierPresence, 0, SEEK_END); // Aller à la fin du fichier
            fprintf(fichierPresence, "\nDate:%s\n", dateActuelle);
        }
    }

    // Écrire la nouvelle entrée de présence
    char nom[50];
    char prenom[50];
    obtenirNomPrenom(id, nom, prenom);
    if (nom[0] != '\0' && prenom[0] != '\0') {
        fprintf(fichierPresence, "%d %s %s\n", id, prenom, nom);
        printf("Presence marquée pour %s %s.\n", prenom, nom);
    } else {
        printf("ID non trouvé dans la liste des étudiants.\n");
    }

    // Fermer le fichier de présence
    fclose(fichierPresence);
}

void marquerPresenceApprenant()
{
    int id;
    printf("Entrez votre ID: ");
    scanf("%d", &id);

    char nom[50];
    char prenom[50];
    obtenirNomPrenom(id, nom, prenom);

    if (nom[0] == '\0' || prenom[0] == '\0') {
        printf("ID non trouvé dans la liste des étudiants.\n");
        return;
    }

    FILE *fichierPresence = fopen("liste_presence.txt", "a");
    if (fichierPresence == NULL) {
        printf("Erreur lors de l'ouverture du fichier de présence.\n");
        return;
    }

    char date[20];
    time_t maintenant = time(NULL);
    struct tm *tm_maintenant = localtime(&maintenant);
    strftime(date, sizeof(date), "%Y-%m-%d", tm_maintenant);

    fprintf(fichierPresence, "%d %s %s %s\n", id, prenom, nom, date);
    printf("Presence marquée pour %s %s.\n", prenom, nom);

    fclose(fichierPresence);
}
void sousMenuApprenant(int c)
{
    
   switch(c)
    {
        case 1:
            marquerPresenceApprenant();
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
    }
}

void marquerPresenceAdmin(int cl)
{
    int id;
    switch(cl)
    {
        case 1:
            printf("Entrez l'ID de l'etudiant: ");
            scanf("%d", &id);
            marquerPresenceId(id, "listeDev.txt");
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
    }
}




//void marquerPresenceApprenant(int cl)
 




