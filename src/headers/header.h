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
//structure des widgets de page home
typedef struct {
    GtkWidget *window;//le window
    GtkBuilder  *builder;//le builder
    GtkWidget *notebook;//le notebook qui contient tous nos pages
    GtkCssProvider *styleProvider;//le style provider
} app_widgets_home;

//structure des widgets
typedef struct {
    GtkBuilder  *builder;//le builder
    GtkWidget *retourner;//botton pour aller vers le table du adherents
    GtkWidget *box_detail;//table des details
    GtkCssProvider *styleProvider;//style provider
    GtkWidget *box_table;//container de table des adherents
    GtkWidget *table_adr;//table des adherents
    GtkWidget *TH_table;//header de table
    GtkWidget *ajouter_adr;//botton ajouter adherent
    GtkWidget *modifier_adr;//botton modifier adherent
    GtkWidget *supprimer_adr;//botton supprimer adherent
    GtkWidget *container_adher_detail;//container du table des details d'adherent
    GtkWidget *nom_ader_details;//label nom adherent
    GtkWidget *prenom_ader_details;//label prénom adherent
    GtkWidget *email_ader_details;//label email adherent
    GtkWidget *adrs_ader_details;//label adresse adherent
    GtkWidget *nb_empr_adr;//label nombre de livres emprunté par adherent
    GtkWidget *dialog;//dialog pour ajouter ou modifier
    GtkWidget *input_nom;//entrée pour nom adherent
    GtkWidget *input_prenom;//entrée pour prénom adherent
    GtkWidget *input_email;//entrée pour email adherent
    GtkWidget *input_adrs;//entrée pour adresse adherent
    GtkWidget *valider_ajout;//botton pour valider  l'ajout
    GtkWidget *anuller_ajout;//botton pour annuler  l'ajout
    GtkWidget *retour_table;//botton pour retourner vers la table
    GtkWidget *search;//entrée de recherche
    GtkWidget *btn_chercher;//botton pour chercher un adherent
    GtkWidget *btn_modifier;//botton pour modifier adherent
    GtkWidget *btn_supprimer;//botton pour supprimer adherent
    GtkWidget *btn_cancel_chercher;//botton pour annuler la recherche
    int modify_or_add;//0 modify 1 add
    int modify_id;//id d'adherent à recherché
    GtkWidget *erreur_dialog;//dialog d'erreur
} app_widgets_adher;
//structure des widgets adherent
typedef struct {
    GtkBuilder  *builder;//builder
    GtkWidget *retourner;//botton retour vers liste empruntes
    GtkWidget *emprunter;//botton emprunter
    GtkWidget *dialog;//dialog d action emprunter
    GtkCssProvider *styleProvider;//le provider du style
    GtkWidget *btn_emprunte;//botton de valider l'emprint
    GtkWidget *liste_emprunteurs;//la tables des adherent emprunteurs
    GtkWidget *TH_table_empr;//le header du tablea
    GtkWidget *liste_empruntes;//la tables des livres empruntés
    GtkWidget *search_livre_empr;//le field pour ecrire le titre à cherhce
    GtkWidget *btn_chercher_livre_empr;//botton pour cherche par titre
    GtkWidget *btn_cancel_chercher_livre_empr;//botton annuler la recherche
    GtkWidget *btn_retourner_liste_empr;//botton retour vers liste empruntes
    GtkWidget *table_empr;//tableau de liste des empruntes
    GtkWidget *comtainer_empr;//container de liste des empruntes
    GtkWidget *table_emprunteurs;//la tables des adherent emprunteurs
    GtkWidget *emprunteurs_label;//le label du titre de livre emprunté
    GtkWidget *valider_emprunte;//botton pour valider l'emprunte
    GtkWidget *cancel_emprunte;//botton pour annuler l'emprunte
    GtkWidget *choix_livre;//combobox des livre
    GtkWidget *choix_adhr;//combobox de adherents
    GtkWidget *erreur_dialog;//dialog des erreurs
} app_widgets_empr;
typedef struct {
    GtkBuilder  *builder;
    GtkWidget *box_detail;
    GtkWidget *box_table;
    GtkWidget *TH_table_livre;
    GtkCssProvider *styleProvider;
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
    GtkWidget *erreur_dialog;
    int modify_or_add;//0 modify 1 add
    char modify_titre[50];
}app_widgets_livres;
#endif //PROJET_C_HEADER_H
