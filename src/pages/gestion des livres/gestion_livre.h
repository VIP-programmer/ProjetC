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

livre *saisieLivre(char *titre,int cat,char *nom,char *pre);
livre *cpyLivre(int *num,char *titre,int cat,char *nom,char *pre,int emprunteur_livre);
listeLivre* modifierLivre(listeLivre *l,char *titre,char *newTitre,int cat,auteur *aut);
listeLivre *creerLivre(livre *elm);
listeLivre *insererListeEnTete(listeLivre *tete,livre *elm);
listeLivre *insererEnOrdre(listeLivre *, livre *);
listeLivre *ordonerListeLivre(listeLivre *);
listeLivre *listeEmprinter(listeLivre *);
listeLivre *listeDisponible(listeLivre *);
listeLivre *listeUniqueLivre(listeLivre *);
listeLivre *copierLivres();
listeLivre *listeOrdoneCteg(listeLivre *l);
void compteurLivres(char *titre,int *emprunte,int *dispo,listeLivre *l);
listeLivre *supprimerLivre(int,char *);
livre *RechercheLivre(char *titre,listeLivre *l);
livre *RechercheLivreParId(int id,listeLivre *l);
listeLivre *trouverPlageLivre(char*,listeLivre *);//hadi hiya dyli
void afficherLivre(livre*);
//listeLivre *ouvrirfichier(char *,char *,int*);
listeLivre* chargerLivres();
//void sauvgarderLivres(char *,char *,listeLivre *,int *);
void sauvgarderLivres();
void afichageListeLivre(listeLivre *tete, app_widgets_livres *appWidgetsLivre);
void afficherUnLive(int num,app_widgets_livres *appWidgetsLivre);
void gestionLivres(app_widgets_home *appWidgetsHome);
#endif //PROJET_C_GESTION_LIVRE_H
