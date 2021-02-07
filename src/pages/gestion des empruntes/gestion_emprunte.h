//
// Created by iliass on 1/8/21.
//

#ifndef PROJET_C_GESTION_EMPRUNTE_H
#define PROJET_C_GESTION_EMPRUNTE_H
#include "../gestion des livres/gestion_livre.h"
#include "../gestion des adherents/gestion_adherent.h"
int inCodeEmprunte(int numAdhr,int numLivre);
int deCodeADR(int numEmpr);
int deCodeLVR(int numEmpr);
int emprunteLivre(int numAdhr,int numLivre);
void afficheLivreEmpruntes(listeLivre* livres,app_widgets_empr *app_wdgts);
listeAdherents adherEmpruntDe(char *titreLivre);
int rendreLivre(int idADher,char* titreLivre);
void gestionEmprents(app_widgets_home *appWidgetsHome);
#endif //PROJET_C_GESTION_EMPRUNTE_H
