//
// Created by iliass on 1/8/21.
//

#ifndef PROJET_C_GESTION_EMPRUNTE_H
#define PROJET_C_GESTION_EMPRUNTE_H
#include "../gestion des livres/gestion_livre.h"
#include "../gestion des adherents/gestion_adherent.h"
int inCodeEmprunte(int numAdhr,int numLivre);//fonction coder de numeros dans un seul pour optenir le num de l action emprunt
int deCodeADR(int numEmpr);//fonction decode le numero adherent depuis de emprunte
int deCodeLVR(int numEmpr);//fonction decode le numero de livre depuis de emprunte
int emprunteLivre(int numAdhr,int numLivre);//fonction qui emprunte un livre
void afficheLivreEmpruntes(listeLivre* livres,app_widgets_empr *app_wdgts);//afficher la liste des livres empuntes
listeAdherents adherEmpruntDe(char *titreLivre);//fonction qui cherche tous les emprunteurs d'un livre
int rendreLivre(int idADher,char* titreLivre);//fonction qui rendre les livres
void gestionEmprents(app_widgets_home *appWidgetsHome);//permet de gerer la page des emprunts
#endif //PROJET_C_GESTION_EMPRUNTE_H

