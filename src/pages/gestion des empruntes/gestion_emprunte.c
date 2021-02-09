//
// Created by iliass on 1/8/21.
//
#include "gestion_emprunte.h"
//variable global qui contient les déferents catégories
char _CATEGS[4][15]={"poétique","narratif","argumentatif","théâtre"};
//liste global des adherents
listeAdherents liste_adhers = NULL;
//liste global des livres
listeLivre *liste_livres=NULL;
//variable global des widgets
app_widgets_empr *globalWidgEmpr=NULL;

//fonction qui gere les erreurs
void afficheErreurEmpr(char* message, app_widgets_empr *app_wdgts){
    //preparer le dialog pour afficher l'erreur
    gtk_message_dialog_format_secondary_text((GtkMessageDialog *) app_wdgts->erreur_dialog, "%s", message);
    //afficher l'erreur
    gtk_dialog_run(GTK_DIALOG (app_wdgts->erreur_dialog));
    gtk_widget_hide(app_wdgts->erreur_dialog);
}
//fonction coder de numeros dans un seul pour optenir le num de l action emprunt
int inCodeEmprunte(int numAdhr,int numLivre){
    return numLivre*1000+numAdhr;
}
//fonction decode le numero adherent depuis de emprunte
int deCodeADR(int numEmpr){
    return numEmpr%1000;
}
//fonction decode le numero de livre depuis de emprunte
int deCodeLVR(int numEmpr){
    return numEmpr/1000;
}
//fonction qui emprunte un livre
int emprunteLivre(int numAdhr,int numLivre){
    char titre[30];
    adherent* p=liste_adhers;
    listeLivre* l=liste_livres;
    while (p && numAdhr!=p->infos->num_adh) //chercher l'adherent dont le num est passé en params
        p=p->suivant;
    if (p){ //si existe
        if (p->infos->nbre_emprunts_adh < 3){ // on increment le nbr des empruntes si <=3
            p->infos->nbre_emprunts_adh ++;
        } else{return -1;}
    } else{return -1;}
    //chercher livre dont le num est passé en params
    while (l) {//chercher le titre de livre dont le num est passé en params
        if(numLivre==l->info->num_liv){
            strcpy(titre,l->info->titre_livre);
            break;
        }
        l = l->suiv;
    }

    if (l){//si existe
        while (l){
            if(strcmp(titre,l->info->titre_livre)==0 && (l->info->emprunteur_liv == -1)){
                break;
            }
            l=l->suiv;
        }
        if (l) {//si existe
            l->info->emprunteur_liv = inCodeEmprunte(numAdhr, l->info->num_liv);//decoder et inserer le numero d'emprunte
        }else return -1;
    } else{return -1;}
    return 1;
}
//fonction qui cherche tous les emprunteurs d'un livre
listeAdherents adherEmpruntDe(char *titreLivre){
    int i;
    donnees *data;
    listeAdherents temp=NULL;
    listeLivre *tabLivres=trouverPlageLivre(titreLivre,liste_livres);//on obtenir tous les livres dont le titre est passé en params
    while (tabLivres){ //parcours du livre
        data=trouverAdherentParID(deCodeADR(tabLivres->info->emprunteur_liv),liste_adhers);//on cherche l'emprunteur de chaque livre
        temp=insererAdherent(data,temp);//on l insere dans une liste
        tabLivres=tabLivres->suiv;
    }
    return temp;

}
//handler du bottun rendre livre
void on_rendre_livre_clicked(GtkButton *button, app_widgets_empr *app_wdgts){
    int idAdher=atoi(gtk_widget_get_name((GtkWidget *) button)); //obtenir ID depuis le nom du bottun
    listeLivre *ltemp=liste_livres;
    char titre[30];
    strcpy(titre,gtk_label_get_text((GtkLabel *) app_wdgts->emprunteurs_label)); //copier le titre dans la variable titre
    rendreLivre(idAdher,titre);//appele la fonction pour rendre le livre
    gtk_widget_show(app_wdgts->liste_empruntes);//afficher la listes des empruntes
    gtk_widget_hide(app_wdgts->liste_emprunteurs);//cacher la liste des emprunteurs
    afficheLivreEmpruntes(liste_livres,app_wdgts);//afficher la liste
}
//handler du bottun details
void on_emprunte_detail_clicked(GtkButton *button, app_widgets_empr *app_wdgts){
    char titre[30],label[100];
    listeAdherents listeAdher=NULL;
    GtkWidget *tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);// l'inialisation du tempBox
    GtkWidget *lnom,*lprenom,*butDetail;
    char id[4];
    int nbDispo=0,nbEmp=0;
    char CnbDispo[4],CnbEmp[4];
    strcpy(titre,gtk_widget_get_name((GtkWidget *) button));//obtenir le titre de livre depuis le nom du botton
    listeAdher=adherEmpruntDe(titre);//trouver les adherent emprunteurs de livre
    GList *children, *iter;
    //slectioner tous les anciens fils du liste
    children = gtk_container_get_children(GTK_CONTAINER(app_wdgts->table_emprunteurs));
    //on les supprime
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    gtk_label_set_text((GtkLabel *) app_wdgts->emprunteurs_label, titre);//mettre dans un label le titre de livre
    //si il n'exist pas des emprunteurs
    if (!listeAdher)
    {
        //le processus pour afficher un tableau avec contenu "vide"
        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);//initialise le box
        lnom=gtk_label_new("Vide");//remplir sans contenu
        gtk_box_pack_start((GtkBox *) tempBox, lnom, 1, 1, 0);//ajoute le label au box
        gtk_box_pack_start((GtkBox *) app_wdgts->table_emprunteurs, tempBox, 0, 1, 0);//ajout du box à la liste
    }
    //parcourant la liste des emprunteurs
    while(listeAdher){
        sprintf(id, "%d", listeAdher->infos->num_adh);//transformer la liste
        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);//l'initialisation de a tempox pour cree un new box
        lnom=gtk_label_new(listeAdher->infos->nom_adh);//remplir le nom de l'adhrent appartir de la liste des adhrents
        lprenom=gtk_label_new(listeAdher->infos->prenom_adh);//remplir le prenom de de l'adhrent appartir de la liste des adhrents
        butDetail = gtk_button_new_with_mnemonic("Rendre le livre");//initialisatiion de la button qui va nous apremet d'afficher les detailles d'un adhrents
        gtk_widget_set_name(butDetail,id);//donner un identifiant a cette button
        g_signal_connect( GTK_WIDGET(butDetail), "clicked", G_CALLBACK( on_rendre_livre_clicked ),app_wdgts);//la creation de signal sur sette button
        gtk_box_pack_start((GtkBox *) tempBox, lnom, 1, 1, 0);//positioner le champ de nom dans la table des adhrents
        gtk_box_pack_start((GtkBox *) tempBox, lprenom, 1, 1, 0);//positioner le champ de prenom de l'adhrent dans la table des adrents
        gtk_box_pack_start((GtkBox *) tempBox, butDetail, 1, 1, 0);//positioner la buton des detailles  de l'adrent dans la table
        gtk_box_set_homogeneous((GtkBox *) tempBox, 1);//donner une taille egale a les enfants de tempBox
        gtk_box_pack_start((GtkBox *) app_wdgts->table_emprunteurs, tempBox, 0, 1, 0);//positioner cette tempBox dans box qui contient la table des adhrents

        listeAdher=listeAdher->suivant;
    }
    //afficher tous les widget que on est cree d'une maniere recursive
    gtk_widget_show_all(app_wdgts->table_emprunteurs);

    gtk_widget_hide(app_wdgts->liste_empruntes);//cacher la listes des empruntes
    gtk_widget_show(app_wdgts->liste_emprunteurs);//afficher la liste des emprunteurs
}
void afficheLivreEmpruntes(listeLivre* livres,app_widgets_empr *app_wdgts){
    listeLivre * liste=listeEmprinter(livres);//obtenir la liste des livres empruntés
    liste=listeUniqueLivre(liste);//suprimer les doublant pour avoir un seul livre par exemplaire
    GtkWidget *tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);//l'initia;lisation de la widget
    GtkWidget *ltitre,*lcategorie,*butDetail;
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(app_wdgts->table_empr));//la recuperation des enfants de la container
    //parcorire tous les children pour desormie
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);//liberie les pointeur qui point sur ses children
    //    afficher les livres
    //si la liste est null
    if(liste==NULL){
        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        ltitre=gtk_label_new("Vide");
        gtk_box_pack_start((GtkBox *) tempBox, ltitre, 1, 1, 0);
        gtk_box_pack_start((GtkBox *) app_wdgts->table_empr, tempBox, 0, 1, 0);
    }
    else{
        //parcours la liste
        while(liste!=NULL){
            /*
             * mettre les informations dans des labels
             * mettre les labels dans des box
             * mettre les boxes dans le container
             */
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
    //afficher le widget
    gtk_widget_show_all(app_wdgts->table_empr);
}
//fonction qui rendre les livres
int rendreLivre(int idADher,char *titreLivre){
    adherent* p=liste_adhers;//obtenir la liste des adhernets
    listeLivre* l=liste_livres;//obtenir la liste des livres
    int tempId;
    while (p && idADher!=p->infos->num_adh) //recherche l adherent dont l id est passé dans params
        p=p->suivant;
    if (p){//si trouvé
        if (p->infos->nbre_emprunts_adh > 0){//si nbr d'empruntes >0 on décrément
            p->infos->nbre_emprunts_adh --;
        } else{return -1;}
    } else{return -1;}
    while (l) {//parcours la liste des livre
        if (strcmp(titreLivre,l->info->titre_livre)==0 && l->info->emprunteur_liv != -1) {//on cherche par titre
            tempId = deCodeADR(l->info->emprunteur_liv);
            if (tempId == idADher) break;//si l emprenteur est le meme qu on a passé en params on sort
        }
        l = l->suiv;
    }
    if (l){//si trouvé on supprime l id emprunte
        l->info->emprunteur_liv=-1;
    } else{return -1;}
    return 1;
}
//handler de botton valider l 'emprunte
void on_valider_emprunter_clicked(GtkButton *button, app_widgets_empr *app_wdgts){
    gtk_dialog_response((GtkDialog *) app_wdgts->dialog, GTK_RESPONSE_OK);//envoi de signal pour dialog
}
//handler de botton annuler emprunte
void on_cancel_emprunter_clicked(GtkButton *button, app_widgets_empr *app_wdgts){
    gtk_dialog_response((GtkDialog *) app_wdgts->dialog, GTK_RESPONSE_DELETE_EVENT);//envoi de signal pour dialog
}
//handler de botton de retour vers liste d'empruntes
void on_btn_retourner_empr_clicked(GtkButton *button, app_widgets_empr *app_wdgts){
    gtk_widget_show(app_wdgts->liste_empruntes);//afficher liste d empruntes
    gtk_widget_hide(app_wdgts->liste_emprunteurs);//cacher liste des emprunteurs
    afficheLivreEmpruntes(liste_livres,app_wdgts);//afficher les livres emprunté
}
//handler de botton emprunte
void on_btn_emprunte_clicked(GtkButton *button, app_widgets_empr *app_wdgts){
    GList* list_items=NULL;
    char *idL,*idA;
    int idLivr,idAdhr;
    int resp;
    int position=0;
    char nomAdher[50],idAdher[4];
    char nomLivre[50],idLivre[4];
    listeAdherents temp_adherents=liste_adhers;//obtenir la liste des adherents
    listeLivre* temp_livres=listeDisponible(liste_livres);//obtenir la listes des empruntes disponibles
    temp_livres=listeUniqueLivre(temp_livres);//filtrer la liste
    /*
     * initialiser les widget du dialog
     */
    app_wdgts->dialog= GTK_WIDGET(gtk_builder_get_object(app_wdgts->builder, "dialog_empr"));
    app_wdgts->cancel_emprunte= GTK_WIDGET(gtk_builder_get_object(app_wdgts->builder, "cancel_emprunte"));
    app_wdgts->valider_emprunte= GTK_WIDGET(gtk_builder_get_object(app_wdgts->builder, "valider_emprunte"));
    app_wdgts->choix_livre= GTK_WIDGET(gtk_builder_get_object(app_wdgts->builder, "choix_livre"));
    app_wdgts->choix_adhr= GTK_WIDGET(gtk_builder_get_object(app_wdgts->builder, "choix_adhr"));
    /*
     * ajouter les signaux
     */
    g_signal_connect(GTK_WIDGET(app_wdgts->valider_emprunte), "clicked", G_CALLBACK(on_valider_emprunter_clicked ), app_wdgts);
    g_signal_connect(GTK_WIDGET(app_wdgts->cancel_emprunte), "clicked", G_CALLBACK(on_cancel_emprunter_clicked ), app_wdgts);
    /*
     * vider les GTKcomboboxes
     */
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(app_wdgts->choix_adhr));
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(app_wdgts->choix_livre));
    /*
     * remplir les deux comboboxex par les livre et les adherents
     */
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
    /*
     * deallocation des listes
     */
    free(temp_adherents);
    free(temp_livres);
    /*
     * renitialiser la position des texte selectioné dans combo box
     */
    gtk_combo_box_set_active((GtkComboBox *) app_wdgts->choix_adhr, 0);
    gtk_combo_box_set_active((GtkComboBox *) app_wdgts->choix_livre, 0);
    resp=gtk_dialog_run(GTK_DIALOG (app_wdgts->dialog));
    switch (resp) {
        case GTK_RESPONSE_OK:
            idL=gtk_combo_box_get_active_id((GtkComboBox *) app_wdgts->choix_livre);
            idA=gtk_combo_box_get_active_id((GtkComboBox *) app_wdgts->choix_adhr);
            //si il y a qlq chose selectioné
            if( idA && idL) {
                strcpy(idA,gtk_combo_box_get_active_id((GtkComboBox *) app_wdgts->choix_adhr));
                strcpy(idL,gtk_combo_box_get_active_id((GtkComboBox *) app_wdgts->choix_livre));
                idAdhr=atoi(idA);//obtenir l id adherent
                idLivr=atoi(idL);//obtenir l'id livre
                emprunteLivre(idAdhr,idLivr);//emprunter un livre
                gtk_dialog_response((GtkDialog *) app_wdgts->dialog, GTK_RESPONSE_DELETE_EVENT);//envoi de signal pour dialog
                afficheLivreEmpruntes(liste_livres, app_wdgts);//afficher les empruntes
            }else{
                afficheErreurEmpr("on n a pas pu faire cette operation",globalWidgEmpr);//afficher message d erreur
            }
            break;
        case GTK_RESPONSE_DELETE_EVENT:
            break;
    }
    //afficher le dialog
    gtk_widget_hide(app_wdgts->dialog);
}
//handler du botton recherche livre emprunté
void on_recherche_empr_clicked(GtkButton *button,app_widgets_empr *appWidgets){
    char titre[50];
    listeLivre *liv=NULL;
    /*
     * gerer la visibilité des widgets
     */
    gtk_widget_hide(appWidgets->search_livre_empr);
    gtk_widget_hide(appWidgets->btn_chercher_livre_empr);
    gtk_widget_show(appWidgets->btn_cancel_chercher_livre_empr);
    //obtenir la chaine à recherche
    strcpy(titre,gtk_entry_get_text((GtkEntry *) appWidgets->search_livre_empr));
    //si elle est non vide
    if(strlen(titre)!=0){
        liv=trouverPlageLivre(titre,liste_livres);//trouve les livres emprunté par le titre
        liv=listeUniqueLivre(liv);//filtrer la liste
    }
    //afficher cette liste
    afficheLivreEmpruntes(liv,appWidgets);
}
//handler du botton annuler la recherche
void on_cancel_recherche_empr_clicked(GtkButton *button,app_widgets_empr *appWidgets){
    /*
     * gerer la visibilité des widgets
     */
    gtk_widget_show(appWidgets->search_livre_empr);
    gtk_widget_show(appWidgets->btn_chercher_livre_empr);
    gtk_widget_hide(appWidgets->btn_cancel_chercher_livre_empr);
    //afficher tous les empruntes
    afficheLivreEmpruntes(liste_livres,appWidgets);

}
void gestionEmprents(app_widgets_home *appWidgetsHome){
    g_slist_free((GSList *) globalWidgEmpr);//vidé le variable global
    app_widgets_empr *appWidgetsEmpr=g_slice_new(app_widgets_empr);//initialiser les widgets
    globalWidgEmpr=appWidgetsEmpr;//initialiser le variable global du structure
    liste_adhers=chargerAdherents();//obtenir la liste des adhernets
    liste_livres=chargerLivres();//obtenir la liste des livres
    /*
     * initialiser les widgets
     */
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
    /*
     * preparer les signaux
     */
    g_signal_connect(GTK_WIDGET(appWidgetsEmpr->btn_emprunte), "clicked", G_CALLBACK(on_btn_emprunte_clicked ), appWidgetsEmpr);
    g_signal_connect(GTK_WIDGET(appWidgetsEmpr->btn_retourner_liste_empr), "clicked", G_CALLBACK(on_btn_retourner_empr_clicked ), appWidgetsEmpr);
    g_signal_connect(GTK_WIDGET(appWidgetsEmpr->btn_chercher_livre_empr), "clicked", G_CALLBACK(on_recherche_empr_clicked ), appWidgetsEmpr);
    g_signal_connect(GTK_WIDGET(appWidgetsEmpr->btn_cancel_chercher_livre_empr), "clicked", G_CALLBACK(on_cancel_recherche_empr_clicked ), appWidgetsEmpr);
    //preparer les styles
    gtk_style_context_add_provider(gtk_widget_get_style_context(appWidgetsEmpr->btn_retourner_liste_empr), GTK_STYLE_PROVIDER(appWidgetsEmpr->styleProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    //modifier le titre du window
    gtk_window_set_title((GtkWindow *) appWidgetsEmpr->dialog, "Emprunter");
    //afficher les empruntes
    afficheLivreEmpruntes(liste_livres, appWidgetsEmpr);
    //connect les signaux
    gtk_builder_connect_signals(appWidgetsEmpr->builder, appWidgetsEmpr);
    gtk_widget_hide(appWidgetsEmpr->btn_cancel_chercher_livre_empr);//cacher la bottons de annuler recherche
    gtk_widget_hide(appWidgetsEmpr->liste_emprunteurs);//afficher la liste des empruntes
}