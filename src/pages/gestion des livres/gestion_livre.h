//
// Created by iliass on 1/7/21.
//

#ifndef PROJET_C_GESTION_LIVRE_H
#define PROJET_C_GESTION_LIVRE_H
#include "../../headers/header.h"

typedef struct _auteur{
    char nom_aut[30];//nom de l'auteur
    char prenom_aut[30];//prenom de l'auteur
}auteur;


typedef struct _livre{
    int num_liv;//le numero de livre
    char titre_livre[30];//le titre de livre
    int ctegorie;//la categorie de livre
    auteur auteur_liv;//l'auteur de livre
    int emprunteur_liv;//le num de l'adherent emprunte ce livre
}livre;

typedef struct _listeLivre{
    livre *info;//les information de livre
    struct _listeLivre *suiv;//le pointeur sur l'element suiv de la liste
}listeLivre;

livre *saisieLivre(char *titre,int cat,char *nom,char *pre);//la creation d'un nouveau livre
listeLivre* modifierLivre(listeLivre *l,char *titre,char *newTitre,int cat,auteur *aut);//la modification des information d'un livre
listeLivre *creerLivre(livre *elm);//la creation d'un nouvelle ellement de la liste des livres
listeLivre *insererListeEnTete(listeLivre *tete,livre *elm);//l'insersion a l'entete de la liste de livres
listeLivre *insererEnOrdre(listeLivre *, livre *);//l'insertion dans une liste ordoner
listeLivre *listeEmprinter(listeLivre *);//recuperation de la liste des livres empuntes
listeLivre *listeDisponible(listeLivre *);//la recuperation de la liste des livres desponible dans le stock
listeLivre *listeUniqueLivre(listeLivre *);//retourner un livre de chaque exemplaire
listeLivre *listeOrdoneCteg(listeLivre *l);//trier la liste par categorie
void compteurLivres(char *titre,int *emprunte,int *dispo,listeLivre *l);//recuperation de nomb des exemplaire desponibl et empruntes
listeLivre *supprimerLivre(int,char *);//la supprission d'un livre
livre *RechercheLivre(char *titre,listeLivre *l);//la recherche d'un livre par son titre
listeLivre *trouverPlageLivre(char*,listeLivre *);//la liste des livre qui son pas desponible
listeLivre* chargerLivres();//chager le liste des livre appartir d'un fichier
void sauvgarderLivres();//engestrer la liste des livres das un fichier
void afichageListeLivre(listeLivre *tete, app_widgets_livres *appWidgetsLivre);//l'affichage de la liste des livre avec gtk
void afficherUnLive(int num,app_widgets_livres *appWidgetsLivre);//l'affichage des information d'un livre donnee
void gestionLivres(app_widgets_home *appWidgetsHome);//la fonction qui permet de fiaire la gestion de la page des livres
#endif //PROJET_C_GESTION_LIVRE_H
