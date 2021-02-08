#ifndef PROJET_C_GESTION_ADHERENT_H
#define PROJET_C_GESTION_ADHERENT_H
#include "../../headers/header.h"
//La structure de Données
typedef struct
{
    int num_adh;//le code d'adherent
    char nom_adh[20];//nom
    char prenom_adh[20];//nom
    char adresse_email[100];//adresse email
    char adresse_personnelle[100];//adresse
    int nbre_emprunts_adh;//nombre livre emprunte
}donnees;

//La structure d'adhérent
typedef struct _adherent
{
    donnees* infos;//les données
    struct _adherent* suivant;//le pointeur qui point sur l'élément suivant de la liste des adhèrent
}adherent,*listeAdherents;

//les fonctions
int createUniqueCode();//pour générer les numéros des l'adhérents
int isValidEmail(char* mail);//pour valider que c'est un email
donnees* creerAdherent(char *nom_adh,char *prenom_adh,char *adresse_email,char *adresse_personnelle,app_widgets_adher *appWidgetsAdher);//la création d'une nouvel adhèrent
donnees* cpyAdherent(int num,char *nom_adh,char *prenom_adh,char *adresse_email,char *adresse_personnelle,int nbre_emprunts_adh,app_widgets_adher *appWidgetsAdher);//la Copier de les informations de l'adhérents
listeAdherents insererAdherent(donnees * data,listeAdherents tete);//l'ajout de l'adhèrent dans la liste
listeAdherents chargerAdherents();//la récupération de la liste des adhérents qui existe dans le fichier (/database)
void sauvgarderAdherent();//la sauvgared de la liste des adhérents qui existe dans le fichier (/database)
donnees* rechercherAdherent(char* nom,char* prenom);//rechercher un adrents par son nom et son prenom
donnees* trouverAdherentParID(int numAdr,listeAdherents tete);//la recherhe d'un adhrant par son numero
void modifierAdherent(int num,listeAdherents tete,char *nom_adh,char *prenom_adh,char *adresse_email,char *adresse_personnelle,app_widgets_adher *appWidgetsAdher);//la odification des information d'un adhrant
void afficherAdherent(listeAdherents tete,app_widgets_adher *appWidgetsAdher);//l'affichage des adhrents
void afficherUnAdherent(int numAdhr,app_widgets_adher *appWidgetsAdher);//l'affichage d'un seul adhrent
listeAdherents supprimerAdherent(int num,listeAdherents tete,app_widgets_adher *appWidgetsAdher);//la supresion d'un adhrant de la liste
void gestionAdherents(app_widgets_home *appWidgetsHome);//la fonction qui prepare la page de gestion d'adhrents
#endif //PROJET_C_GESTION_ADHERENT_H
