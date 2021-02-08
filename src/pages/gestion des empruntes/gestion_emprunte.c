//
// Created by iliass on 1/8/21.
//
#include "gestion_emprunte.h"
char _CATEGS[4][15]={"poétique","narratif","argumentatif","théâtre"};
listeAdherents liste_adhers = NULL;
listeLivre *liste_livres=NULL;
app_widgets_empr *globalWidgEmpr=NULL;

int inCodeEmprunte(int numAdhr,int numLivre){
    return numLivre*1000+numAdhr;
}
int deCodeADR(int numEmpr){
    return numEmpr%1000;
}
int deCodeLVR(int numEmpr){
    return numEmpr/1000;
}
int emprunteLivre(int numAdhr,int numLivre){
    adherent* p=liste_adhers;
    listeLivre* l=liste_livres;
    while (p && numAdhr!=p->infos->num_adh)
        p=p->suivant;
    if (p){
        if (p->infos->nbre_emprunts_adh <= 3){
            p->infos->nbre_emprunts_adh ++;
        } else{return -1;}
    } else{return -1;}
    while (l && (numLivre!=l->info->num_liv/* || l->info->num_liv!=-1*/))
        l=l->suiv;
    if (l){
        l->info->emprunteur_liv=inCodeEmprunte(numAdhr,numLivre);
    } else{return -1;}
    return 1;
}
listeAdherents adherEmpruntDe(char *titreLivre){
    int i;
    donnees *data;
    listeAdherents temp=NULL;
    listeLivre *tabLivres=trouverPlageLivre(titreLivre,liste_livres);
    while (tabLivres){
        data=trouverAdherentParID(deCodeADR(tabLivres->info->emprunteur_liv),liste_adhers);
        temp=insererAdherent(data,temp);
        tabLivres=tabLivres->suiv;
    }
    return temp;

}
void on_rendre_livre_clicked(GtkButton *button, app_widgets_empr *app_wdgts){
    int idAdher=atoi(gtk_widget_get_name((GtkWidget *) button));
    listeLivre *ltemp=liste_livres;
    char titre[30];
    strcpy(titre,gtk_label_get_text((GtkLabel *) app_wdgts->emprunteurs_label));
    rendreLivre(idAdher,titre);
    gtk_widget_show(app_wdgts->liste_empruntes);
    gtk_widget_hide(app_wdgts->liste_emprunteurs);
    afficheLivreEmpruntes(liste_livres,app_wdgts);
}
void on_emprunte_detail_clicked(GtkButton *button, app_widgets_empr *app_wdgts){
    char titre[30],label[100];
    listeAdherents listeAdher=NULL;
    GtkWidget *tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    GtkWidget *lnom,*lprenom,*butDetail;
    char id[4];
    int nbDispo=0,nbEmp=0;
    char CnbDispo[4],CnbEmp[4];
    strcpy(titre,gtk_widget_get_name((GtkWidget *) button));
    listeAdher=adherEmpruntDe(titre);
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(app_wdgts->table_emprunteurs));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);
    strcpy(label,titre);
    gtk_label_set_text((GtkLabel *) app_wdgts->emprunteurs_label, label);
    lnom=gtk_label_new("Nom");
    lprenom=gtk_label_new("Prénom");
    gtk_box_pack_start((GtkBox *) tempBox, lnom, 1, 0, 0);
    gtk_box_pack_start((GtkBox *) tempBox, lprenom, 1, 0, 0);
    gtk_box_pack_start((GtkBox *) tempBox, gtk_label_new("Actions"), 1, 1, 0);
    gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
    gtk_box_pack_start((GtkBox *) app_wdgts->table_emprunteurs, tempBox, 0, 1, 0);
    if (!listeAdher)
    {
        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        lnom=gtk_label_new("Vide");
        gtk_box_pack_start((GtkBox *) tempBox, lnom, 1, 1, 0);
        gtk_box_pack_start((GtkBox *) app_wdgts->table_emprunteurs, tempBox, 0, 1, 0);
    }
    while(listeAdher){
        sprintf(id, "%d", listeAdher->infos->num_adh);
        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        lnom=gtk_label_new(listeAdher->infos->nom_adh);
        lprenom=gtk_label_new(listeAdher->infos->prenom_adh);
        butDetail = gtk_button_new_with_mnemonic("Rendre le livre");
        gtk_widget_set_name(butDetail,id);
        g_signal_connect( GTK_WIDGET(butDetail), "clicked", G_CALLBACK( on_rendre_livre_clicked ),app_wdgts);
        gtk_box_pack_start((GtkBox *) tempBox, lnom, 1, 0, 0);
        gtk_box_pack_start((GtkBox *) tempBox, lprenom, 1, 0, 0);
        gtk_box_pack_start((GtkBox *) tempBox, butDetail, 1, 1, 0);
        gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
        gtk_box_pack_start((GtkBox *) app_wdgts->table_emprunteurs, tempBox, 0, 1, 0);

        listeAdher=listeAdher->suivant;
    }
    gtk_widget_show_all(app_wdgts->table_emprunteurs);

    gtk_widget_hide(app_wdgts->liste_empruntes);
    gtk_widget_show(app_wdgts->liste_emprunteurs);
}
void afficheLivreEmpruntes(listeLivre* livres,app_widgets_empr *app_wdgts){
    listeLivre * liste=listeEmprinter(livres);
    liste=listeUniqueLivre(liste);
    GtkWidget *tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    GtkWidget *ltitre,*lcategorie,*butDetail;
    GList *children, *iter;
    char id[4];
    children = gtk_container_get_children(GTK_CONTAINER(app_wdgts->table_empr));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);
    ltitre=gtk_label_new("Titre");
    lcategorie=gtk_label_new("Categorie");
    gtk_box_pack_start((GtkBox *) tempBox, ltitre, 1, 0, 0);
    gtk_box_pack_start((GtkBox *) tempBox, lcategorie, 1, 0, 0);
    gtk_box_pack_start((GtkBox *) tempBox, gtk_label_new("Actions"), 1, 1, 0);
    gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
    gtk_box_pack_start((GtkBox *) app_wdgts->table_empr, tempBox, 0, 1, 0);
    if(liste==NULL){
        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        ltitre=gtk_label_new("Vide");
        gtk_box_pack_start((GtkBox *) tempBox, ltitre, 1, 1, 0);
        gtk_box_pack_start((GtkBox *) app_wdgts->table_empr, tempBox, 0, 1, 0);
        printf("la liste est vide !!");
    }
    else{

        while(liste!=NULL){
            afficherLivre(liste->info);//l'affichage dans le console

            tempBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
            ltitre = gtk_label_new(liste->info->titre_livre);
            lcategorie = gtk_label_new(_CATEGS[liste->info->ctegorie]);
            butDetail = gtk_button_new_with_mnemonic("détails");
            gtk_widget_set_name(butDetail, liste->info->titre_livre);
            g_signal_connect(GTK_WIDGET(butDetail), "clicked", G_CALLBACK(on_emprunte_detail_clicked),
                             app_wdgts);
            gtk_box_pack_start((GtkBox *) tempBox, ltitre, 1, 0, 0);
            gtk_box_pack_start((GtkBox *) tempBox, lcategorie, 1, 0, 0);
            gtk_box_pack_start((GtkBox *) tempBox, butDetail, 1, 1, 0);
            gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
            gtk_box_pack_start((GtkBox *) app_wdgts->table_empr, tempBox, 0, 1, 0);
            liste = liste->suiv;
        }
    }
    gtk_widget_show_all(app_wdgts->table_empr);
}

int rendreLivre(int idADher,char *titreLivre){
    adherent* p=liste_adhers;
    listeLivre* l=liste_livres;
    int tempId;
    while (p && idADher!=p->infos->num_adh)
        p=p->suivant;
    if (p){
        if (p->infos->nbre_emprunts_adh > 0){
            p->infos->nbre_emprunts_adh --;
        } else{return -1;}
    } else{return -1;}
    while (l) {
        if (strcmp(titreLivre,l->info->titre_livre)==0 && l->info->emprunteur_liv != 0) {
            tempId = deCodeADR(l->info->emprunteur_liv);
            if (tempId == idADher) break;
        }
        l = l->suiv;
    }
    if (l){
        l->info->emprunteur_liv=-1;
    } else{return -1;}
    return 1;
}
void on_valider_emprunter_clicked(GtkButton *button, app_widgets_empr *app_wdgts){
    int idLivre,idAdher;
    idAdher=atoi(gtk_combo_box_get_active_id((GtkComboBox *) app_wdgts->choix_adhr));
    idLivre=atoi(gtk_combo_box_get_active_id((GtkComboBox *) app_wdgts->choix_livre));
    emprunteLivre(idAdher,idLivre);
    gtk_dialog_response((GtkDialog *) app_wdgts->dialog, GTK_RESPONSE_DELETE_EVENT);
    afficheLivreEmpruntes(liste_livres,app_wdgts);
}
void on_cancel_emprunter_clicked(GtkButton *button, app_widgets_empr *app_wdgts){
    gtk_dialog_response((GtkDialog *) app_wdgts->dialog, GTK_RESPONSE_DELETE_EVENT);
}
void on_btn_retourner_empr_clicked(GtkButton *button, app_widgets_empr *app_wdgts){
    gtk_widget_show(app_wdgts->liste_empruntes);
    gtk_widget_hide(app_wdgts->liste_emprunteurs);
    afficheLivreEmpruntes(liste_livres,app_wdgts);
}
void on_btn_emprunte_clicked(GtkButton *button, app_widgets_empr *app_wdgts){
    GList* list_items=NULL;
    int position=0;
    char nomAdher[50],idAdher[4];
    char nomLivre[50],idLivre[4];
    listeAdherents temp_adherents=liste_adhers;
    listeLivre* temp_livres=listeDisponible(liste_livres);
    temp_livres=listeUniqueLivre(temp_livres);
    app_wdgts->dialog= GTK_WIDGET(gtk_builder_get_object(app_wdgts->builder, "dialog_empr"));
    app_wdgts->cancel_emprunte= GTK_WIDGET(gtk_builder_get_object(app_wdgts->builder, "cancel_emprunte"));
    app_wdgts->valider_emprunte= GTK_WIDGET(gtk_builder_get_object(app_wdgts->builder, "valider_emprunte"));
    app_wdgts->choix_livre= GTK_WIDGET(gtk_builder_get_object(app_wdgts->builder, "choix_livre"));
    app_wdgts->choix_adhr= GTK_WIDGET(gtk_builder_get_object(app_wdgts->builder, "choix_adhr"));
    g_signal_connect(GTK_WIDGET(app_wdgts->valider_emprunte), "clicked", G_CALLBACK(on_valider_emprunter_clicked ), app_wdgts);
    g_signal_connect(GTK_WIDGET(app_wdgts->cancel_emprunte), "clicked", G_CALLBACK(on_cancel_emprunter_clicked ), app_wdgts);
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(app_wdgts->choix_adhr));
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(app_wdgts->choix_livre));
    while (temp_adherents){
        sprintf(idAdher,"%d",temp_adherents->infos->num_adh);
        sprintf(nomAdher,"%s %s",temp_adherents->infos->nom_adh,temp_adherents->infos->prenom_adh);
        gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(app_wdgts->choix_adhr), position++, idAdher,nomAdher);
        temp_adherents=temp_adherents->suivant;
    }
    position=0;
    while (temp_livres){
        sprintf(idLivre,"%d",temp_livres->info->num_liv);
        sprintf(nomLivre,"%s",temp_livres->info->titre_livre);
        gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(app_wdgts->choix_livre), position++, idLivre,nomLivre);
        temp_livres=temp_livres->suiv;
    }
    free(temp_adherents);
    free(temp_livres);
    gtk_combo_box_set_active((GtkComboBox *) app_wdgts->choix_adhr, 0);
    gtk_combo_box_set_active((GtkComboBox *) app_wdgts->choix_livre, 0);
    gtk_dialog_run(GTK_DIALOG (app_wdgts->dialog));

    gtk_widget_hide(app_wdgts->dialog);
}

void on_recherche_empr_clicked(GtkButton *button,app_widgets_empr *appWidgets){
    char titre[50];
    listeLivre *liv=NULL;
    gtk_widget_hide(appWidgets->search_livre_empr);
    gtk_widget_hide(appWidgets->btn_chercher_livre_empr);
    gtk_widget_show(appWidgets->btn_cancel_chercher_livre_empr);
    strcpy(titre,gtk_entry_get_text((GtkEntry *) appWidgets->search_livre_empr));
    if(strlen(titre)!=0){
        liv=trouverPlageLivre(titre,liste_livres);
        liv=listeUniqueLivre(liv);
    }
    afficheLivreEmpruntes(liv,appWidgets);
}
void on_cancel_recherche_empr_clicked(GtkButton *button,app_widgets_empr *appWidgets){
    gtk_widget_show(appWidgets->search_livre_empr);
    gtk_widget_show(appWidgets->btn_chercher_livre_empr);
    gtk_widget_hide(appWidgets->btn_cancel_chercher_livre_empr);
    afficheLivreEmpruntes(liste_livres,appWidgets);

}
void afficheErreurEmpr(char* message, app_widgets_adher *app_wdgts){
    gtk_message_dialog_format_secondary_text((GtkMessageDialog *) app_wdgts->erreur_dialog, "%s", message);
    gtk_dialog_run(GTK_DIALOG (app_wdgts->erreur_dialog));
    gtk_widget_hide(app_wdgts->erreur_dialog);
}
void gestionEmprents(app_widgets_home *appWidgetsHome){
    g_slist_free((GSList *) globalWidgEmpr);
    app_widgets_empr *appWidgetsEmpr=g_slice_new(app_widgets_empr);
    globalWidgEmpr=appWidgetsEmpr;
    liste_adhers=chargerAdherents();
    liste_livres=chargerLivres();
    appWidgetsEmpr->builder=appWidgetsHome->builder;
    appWidgetsEmpr->styleProvider=appWidgetsHome->styleProvider;
    appWidgetsEmpr->liste_empruntes= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "liste_empr"));
    appWidgetsEmpr->TH_table_empr= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "TH_table_empr"));
    appWidgetsEmpr->liste_emprunteurs= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "liste_adr_empr"));
    appWidgetsEmpr->dialog= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "dialog_empr"));
    appWidgetsEmpr->search_livre_empr= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "chercher_titre"));
    appWidgetsEmpr->btn_chercher_livre_empr= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "btn_chercher_titre"));
    appWidgetsEmpr->btn_cancel_chercher_livre_empr= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "btn_clear_recherche"));
    appWidgetsEmpr->btn_retourner_liste_empr= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "btn_retourner_liste_empr"));
    appWidgetsEmpr->table_empr= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "table_empr"));
    appWidgetsEmpr->table_emprunteurs= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "table_emprunteurs"));
    appWidgetsEmpr->btn_emprunte= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "btn_emprunte"));
    appWidgetsEmpr->emprunteurs_label= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "emprunteurs_label"));
    appWidgetsEmpr->erreur_dialog= GTK_WIDGET(gtk_builder_get_object(appWidgetsEmpr->builder, "dialog_erreur"));
    g_signal_connect(GTK_WIDGET(appWidgetsEmpr->btn_emprunte), "clicked", G_CALLBACK(on_btn_emprunte_clicked ), appWidgetsEmpr);
    g_signal_connect(GTK_WIDGET(appWidgetsEmpr->btn_retourner_liste_empr), "clicked", G_CALLBACK(on_btn_retourner_empr_clicked ), appWidgetsEmpr);
    g_signal_connect(GTK_WIDGET(appWidgetsEmpr->btn_chercher_livre_empr), "clicked", G_CALLBACK(on_recherche_empr_clicked ), appWidgetsEmpr);
    g_signal_connect(GTK_WIDGET(appWidgetsEmpr->btn_cancel_chercher_livre_empr), "clicked", G_CALLBACK(on_cancel_recherche_empr_clicked ), appWidgetsEmpr);
    gtk_style_context_add_provider(gtk_widget_get_style_context(appWidgetsEmpr->btn_retourner_liste_empr), GTK_STYLE_PROVIDER(appWidgetsEmpr->styleProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    afficheLivreEmpruntes(liste_livres, appWidgetsEmpr);
    gtk_builder_connect_signals(appWidgetsEmpr->builder, appWidgetsEmpr);
    gtk_widget_hide(appWidgetsEmpr->btn_cancel_chercher_livre_empr);
    gtk_widget_hide(appWidgetsEmpr->liste_emprunteurs);
}