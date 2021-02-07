//
// Created by iliass on 1/8/21.
//
#include "gestion_emprunte.h"

listeAdherents liste_adhers = NULL;
listeLivre *liste_livres=NULL;

int inCodeEmprunte(int numAdhr,int numLivre){
    return numLivre*1000+numAdhr;
}
int deCodeADR(int numEmpr){
    return numEmpr%1000;
}
int deCodeLVR(int numEmpr){
    return numEmpr/1000;
}
int emprunteLivre(int numAdhr,int numLivre,listeAdherents adherents,listeLivre* livres){
    adherent* p=adherents;
    listeLivre* l=livres;
    while (p && numAdhr!=p->infos->num_adh)
        p=p->suivant;
    if (p){
        if (p->infos->nbre_emprunts_adh < 3){
            p->infos->nbre_emprunts_adh ++;
        } else{return -1;}
    } else{return -1;}
    while (l && (numLivre!=l->info->num_liv || l->info->num_liv!=-1))
        l=l->suiv;
    if (l){
        l->info->emprunteur_liv=inCodeEmprunte(numAdhr,numLivre);
    } else{return -1;}
    return 1;
}
void afficheLivreEmpruntes(listeLivre* livres,app_widgets_empr *app_wdgts){
    listeLivre * l=livres;//TODO: maxi livres
    if (!livres)
    {
        printf("Liste est vide\n");
    }
    while(l){
        printf("------------------------------\n");
        printf("| Code:		%d\t\t|\n", l->info->num_liv);
        printf("| Nom:		%s\t\t|\n", l->info->titre_livre);
        printf("------------------------------\n");
        printf("               |\n");
        printf("               V\n");
        l=l->suiv;
    }
    printf("-----------------------------\n");
    printf("|            VIDE           |\n");
    printf("-----------------------------\n");
}
void afficherAdherEmpruntDe(char *titreLivre,listeLivre* livres,listeAdherents adherents){
    int i;
    donnees *data;
    listeAdherents temp=NULL;
    livre **tabLivres=trouverPlageLivre(titreLivre,livres);
    for (i=0;i<sizeof(tabLivres)/ sizeof(livre**) && tabLivres[i]->emprunteur_liv!=-1;++i){
        data=trouverAdherentParID(deCodeADR(tabLivres[i]->emprunteur_liv),adherents);
        insererAdherent(data,temp);
    }
    afficherAdherent(temp,NULL);//TODO

}
int rendreLivre(int numEmpr,listeAdherents adherents,listeLivre* livres){
    adherent* p=adherents;
    listeLivre* l=livres;
    int idAdr=deCodeADR(numEmpr),idLVR=deCodeLVR(numEmpr);
    while (p && idAdr!=p->infos->num_adh)
        p=p->suivant;
    if (p){
        if (p->infos->nbre_emprunts_adh > 0){
            p->infos->nbre_emprunts_adh --;
        } else{return -1;}
    } else{return -1;}
    while (l && idLVR!=l->info->num_liv)
        l=l->suiv;
    if (l){
        l->info->emprunteur_liv=-1;
    } else{return -1;}
    return 1;
}
void on_valider_emprunter_clicked(GtkButton *button, app_widgets_empr *app_wdgts){

    gtk_dialog_response((GtkDialog *) app_wdgts->dialog, GTK_RESPONSE_DELETE_EVENT);
}
void on_cancel_emprunter_clicked(GtkButton *button, app_widgets_empr *app_wdgts){
    gtk_dialog_response((GtkDialog *) app_wdgts->dialog, GTK_RESPONSE_DELETE_EVENT);
}
void on_btn_emprunte_clicked(GtkButton *button, app_widgets_empr *app_wdgts){
    GList* list_items=NULL;
    int position=1;
    char nomAdher[50],idAdher[4];
    listeAdherents temp_adherents=liste_adhers;
    app_wdgts->dialog= GTK_WIDGET(gtk_builder_get_object(app_wdgts->builder, "dialog_empr"));
    app_wdgts->cancel_emprunte= GTK_WIDGET(gtk_builder_get_object(app_wdgts->builder, "cancel_emprunte"));
    app_wdgts->valider_emprunte= GTK_WIDGET(gtk_builder_get_object(app_wdgts->builder, "valider_emprunte"));
    app_wdgts->choix_livre= GTK_WIDGET(gtk_builder_get_object(app_wdgts->builder, "choix_livre"));
    app_wdgts->choix_adhr= GTK_WIDGET(gtk_builder_get_object(app_wdgts->builder, "choix_adhr"));
    g_signal_connect(GTK_WIDGET(app_wdgts->valider_emprunte), "clicked", G_CALLBACK(on_valider_emprunter_clicked ), app_wdgts);
    g_signal_connect(GTK_WIDGET(app_wdgts->cancel_emprunte), "clicked", G_CALLBACK(on_cancel_emprunter_clicked ), app_wdgts);

    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(app_wdgts->choix_adhr), 0, "none","choisir");
    while (temp_adherents){
        sprintf(idAdher,"%d",temp_adherents->infos->num_adh);
        sprintf(nomAdher,"%s %s",temp_adherents->infos->nom_adh,temp_adherents->infos->prenom_adh);
        gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(app_wdgts->choix_adhr), position++, idAdher,nomAdher);
        gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(app_wdgts->choix_livre), position++, idAdher,nomAdher);
        temp_adherents=temp_adherents->suivant;
    }
    free(temp_adherents);
    gtk_combo_box_set_active((GtkComboBox *) app_wdgts->choix_adhr, 0);
    gtk_dialog_run(GTK_DIALOG (app_wdgts->dialog));

    gtk_widget_hide(app_wdgts->dialog);
}

void gestionEmprents(app_widgets_home *appWidgetsHome){
    app_widgets_empr *appWidgetsEmpr=g_slice_new(app_widgets_empr);
    liste_adhers=chargerAdherents();
    appWidgetsEmpr->builder=appWidgetsHome->builder;
    appWidgetsEmpr->liste_empruntes= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "liste_empr"));
    appWidgetsEmpr->liste_emprunteurs= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "liste_adr_empr"));
    appWidgetsEmpr->dialog= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "dialog_empr"));
    appWidgetsEmpr->search_livre_empr= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "chercher_titre"));
    appWidgetsEmpr->btn_chercher_livre_empr= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "btn_chercher_titre"));
    appWidgetsEmpr->btn_cancel_chercher_livre_empr= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "btn_clear_recherche"));
    appWidgetsEmpr->search_adhr_empr= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "chercher_emprunteur"));
    appWidgetsEmpr->btn_chercher_adhr_empr= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "btn_chercher_emprunteur"));
    appWidgetsEmpr->btn_cancel_chercher_adhr_empr= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "btn_clear_recherche_emprunteur"));
    appWidgetsEmpr->table_empr= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "table_empr"));
    appWidgetsEmpr->table_emprunteurs= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "table_emprunteurs"));
    appWidgetsEmpr->btn_emprunte= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "btn_emprunte"));
    appWidgetsEmpr->emprunteurs_label= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "emprunteurs_label"));
    g_signal_connect(GTK_WIDGET(appWidgetsEmpr->btn_emprunte), "clicked", G_CALLBACK(on_btn_emprunte_clicked ), appWidgetsEmpr);
    afficheLivreEmpruntes(liste_livres, appWidgetsEmpr);
    gtk_builder_connect_signals(appWidgetsEmpr->builder, appWidgetsEmpr);
    gtk_widget_hide(appWidgetsEmpr->btn_cancel_chercher_livre_empr);
    gtk_widget_hide(appWidgetsEmpr->liste_emprunteurs);
}