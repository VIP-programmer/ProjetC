//
// Created by iliass on 1/18/21.
//

#ifndef PROJET_C_HEADER_H
#define PROJET_C_HEADER_H
//include les biblios
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
//definir MALLOC ET LES CONSTANTES
#define Malloc(type) (type*) malloc(sizeof(type))
#define adhFile "../database/adherents"
#define livreFile "../database/livres"
#define adhCount "../database/lastIDAdherent"
#define livreCount "../database/lastIDLivre"

//les structures
typedef struct {
    GtkWidget *window;
    GtkBuilder  *builder;
    GtkWidget *notebook;
} app_widgets_home;

//structure des widgets
typedef struct {
    GtkBuilder  *builder;
    GtkWidget *ges_adher;
    GtkWidget *details;
    GtkWidget *retourner;
    GtkWidget *box_detail;
    GtkWidget *box_table;
    GtkWidget *body;
    GtkWidget *table_adr;
    GtkWidget *ajouter_adr;
    GtkWidget *modifier_adr;
    GtkWidget *supprimer_adr;
    GtkWidget *nom_ader_details;
    GtkWidget *prenom_ader_details;
    GtkWidget *email_ader_details;
    GtkWidget *adrs_ader_details;
    GtkWidget *nb_empr_adr;
    GtkWidget *dialog;
    GtkWidget *input_nom;
    GtkWidget *input_prenom;
    GtkWidget *input_email;
    GtkWidget *input_adrs;
    GtkWidget *valider_ajout;
    GtkWidget *anuller_ajout;
    GtkWidget *retour_table;
    GtkWidget *search;
    GtkWidget *btn_chercher;
    GtkWidget *btn_modifier;
    GtkWidget *btn_supprimer;
    GtkWidget *btn_cancel_chercher;
    int modify_or_add;//0 modify 1 add
    int modify_id;
} app_widgets_adher;
//structure des widgets
typedef struct {
    GtkBuilder  *builder;
    GtkWidget *retourner;
    GtkWidget *emprunter;
    GtkWidget *dialog;
    GtkWidget *btn_emprunte;
    GtkWidget *liste_emprunteurs;
    GtkWidget *liste_empruntes;
    GtkWidget *search_livre_empr;
    GtkWidget *btn_chercher_livre_empr;
    GtkWidget *btn_cancel_chercher_livre_empr;
    GtkWidget *btn_retourner_liste_empr;
    GtkWidget *table_empr;
    GtkWidget *comtainer_empr;
    GtkWidget *table_emprunteurs;
    GtkWidget *emprunteurs_label;
    GtkWidget *valider_emprunte;
    GtkWidget *cancel_emprunte;
    GtkWidget *choix_livre;
    GtkWidget *choix_adhr;
} app_widgets_empr;//
typedef struct {
    GtkBuilder  *builder;
    GtkWidget *ges_livre;
    GtkWidget *box_detail;
    GtkWidget *box_table;
    GtkWidget *container_table;
    GtkWidget *dialog_livre;
    GtkWidget *chercher_livre,
            *annuler_chercher_livre,
            *input_chercher_livre,
            *categorie,
            *ajouter_livre,
            *table_livres,
            *retourner_livre,
            *titre_liv_details,
            *catregorie_liv_details,
            *aut_nom_liv_details,
            *aut_prenom_liv_details,
            *nb_empr_liv,
            *nb_desp_liv,
            *supprimer_liv,
            *modifier_liv;

    GtkWidget *input_titre_liv,
            *input_categorie_liv,
            *input_prenom_atr,
            *input_nom_autr;
    GtkWidget *valider_ajouter,
            *cancel_ajouter,
            *nbr_exemplaire;
    int modify_or_add;//0 modify 1 add
    char modify_titre[50];
}app_widgets_livres;
#endif //PROJET_C_HEADER_H
