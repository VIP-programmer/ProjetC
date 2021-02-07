//
// Created by iliass on 1/7/21.
//

#ifndef PROJET_C_GESTION_LIVRE_H
#define PROJET_C_GESTION_LIVRE_H
#include "../../headers/header.h"

typedef struct _auteur{
    char nom_aut[30];
    char prenom_aut[30];
}auteur;


typedef struct _livre{
    int num_liv;
    char titre_livre[30];
    int ctegorie;
    auteur auteur_liv;
    //le num de l'adherent emprunte ce livre
    int emprunteur_liv;
}livre;

typedef struct _listeLivre{
    livre *info;
    struct _listeLivre *suiv;
}listeLivre;

livre *saisieLivre(int num,char *titre,int cat,char *nom,char *pre);
livre *cpyLivre(int *num,char *titre,int cat,char *nom,char *pre,int emprunteur_livre);
listeLivre* modifierLivre(listeLivre *l,char *titre,int cat,auteur *aut,int num);
listeLivre *creerLivre(int *num,char *titre,int cat,char *nom,char *pre);
listeLivre *insererListeEnTete(listeLivre *tete,int *num,char *titre,int cat,char *nom,char *pre);
listeLivre *insererEnOrdre(listeLivre *, livre *);
listeLivre *ordonerListeLivre(listeLivre *);
listeLivre *listeEmprinter(listeLivre *);
void compteurLivres(char *titre,int *emprunte,int *dispo,listeLivre *l);
listeLivre *supprimerLivre(int,listeLivre *);
livre *RechercheLivre(char *titre,listeLivre *l);
livre *RechercheLivreParId(int id,listeLivre *l);
livre **trouverPlageLivre(char*,listeLivre *);//hadi hiya dyli
void afficherLivre(livre*);
//listeLivre *ouvrirfichier(char *,char *,int*);
void ouvrirfichier(char *ch,char *ch1,int *n);
//void fermergestionLivre(char *,char *,listeLivre *,int *);
void fermergestionLivre(char *ch,char *ch1);
void afiichageListeLivre(listeLivre *tete,app_widgets_livres *appWidgetsLivre);
void afficherUnLive(int num,app_widgets_livres *appWidgetsLivre);
void gestionLivres(app_widgets_home *appWidgetsHome);
#endif //PROJET_C_GESTION_LIVRE_H
