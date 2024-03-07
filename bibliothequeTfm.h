#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

//premier livrable

//prototype
int afficheMenuAdmin();
int afficheMenuApprenant();
int authentification();
int verifierLoginMdp(const char* Fichier, const char* loginSaisi, const char* mdpSaisi);
void marquerPresenceAdmin();
int verifierPresenceId(int id, const char *nomFichier);
void marquerPresenceApprenant();
int sousMenuAdmin(int c);
void sousMenuApprenant(int c);

//structure
 typedef struct
 {
    int id;
    char nom[50];
    char prenom[50];
 } APPRENANT;

#endif 