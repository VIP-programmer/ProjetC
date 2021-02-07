//
// Created by adnane on 1/7/21.
//

#include"gestion_livre.h"
listeLivre *liteLivre=NULL;
char CATEGS[4][15]={"poétique","narratif","argumentatif","théâtre"};
int *nombre_total_livres;
app_widgets_home *globalAppWidget=NULL;

int createUniqueCodeLivre(){
    FILE *f=NULL;
    int count,read;
    f=fopen(livreCount,"rb");//ouvre le fichier pointéen mode lecture (binaire).
    if (!f){//si il y a un erreur au moment d'ouverture de la fiche
        count=0;
    } else {
        fread(&count,sizeof(int),1,f);//la recuperation de nombre des element de la liste qui existe dans le fichier
        fclose(f);//la furmuture du fichier
    }
    f=fopen(livreCount,"wb");//ouvre le fichier pointé en mode ecriture(binaire).
    if (!f){//si il y a un erreur au moment d'ouverture de la fiche
        perror("Err");
    } else {
        count++;//incrumenter le nombre max par 1
        fwrite(&count,sizeof (int),1,f);//et on l'ecrire dans le fichier
    }
    fclose(f);//la furmuture de fichier
    return count-1;//returner l'indice
}
livre *saisieLivre(char *titre,int cat,char *nom,char *pre){
    livre* l;
    l=(livre*)malloc(sizeof(livre));
    // ta ndir hna teste wax khawyan wla la
    l->num_liv=createUniqueCodeLivre();
    strcpy(l->titre_livre,titre);
    l->ctegorie= cat;
    //strcpy(l->categ_liv,cat);
    strcpy(l->auteur_liv.nom_aut, nom);
    strcpy(l->auteur_liv.prenom_aut,pre);
    (l->emprunteur_liv)=-1;
    return l;
}
livre *cpyLivre(int *num,char *titre,int cat,char *nom,char *pre,int emprunteur_livre){
    livre* l=(livre*)malloc(sizeof(livre));
    l->num_liv=(*num);
    strcpy(l->titre_livre,titre);
    l->ctegorie= cat;
    //strcpy(l->categ_liv,cat);
    strcpy(l->auteur_liv.nom_aut, nom);
    strcpy(l->auteur_liv.prenom_aut,pre);
    (l->emprunteur_liv)=emprunteur_livre;
    return l;
}
listeLivre * modifierLivre(listeLivre *l,char *titre,char* newTitre,int cat,auteur *aut){

    listeLivre *temp=NULL;
    temp=l;
    while(temp != NULL)
    {
        if (strcmp(temp->info->titre_livre,titre)==0){
            strcpy(temp->info->titre_livre,newTitre);
            temp->info->ctegorie= cat;
            strcpy(temp->info->auteur_liv.nom_aut, aut->nom_aut);
            strcpy(temp->info->auteur_liv.prenom_aut,aut->prenom_aut);
        }
        temp=temp->suiv;
    }

    return l;
}

listeLivre *creerLivre(livre *elm) {
    listeLivre *nouv =(listeLivre*)malloc(sizeof(listeLivre));
    nouv->info=elm;
    nouv->suiv=NULL;
    return nouv;
}

listeLivre *insererListeEnTete(listeLivre *tete,livre *elm){
    listeLivre *nouveau=creerLivre(elm);
    nouveau->suiv=tete;
    tete = nouveau ;
    return tete ;
}

listeLivre *insererEnOrdre(listeLivre *l, livre *elm){
    listeLivre* nv =(listeLivre*)malloc(sizeof(listeLivre));
    nv->info=elm;
    listeLivre *temp = l;

    if((temp == NULL) || temp->info->num_liv >= elm->num_liv)
    {
        nv->suiv=l;
        l=nv;
    }
    else{
        while((temp->suiv != NULL) && temp->info->num_liv < elm->num_liv )
            temp = temp->suiv;

        nv->suiv = temp->suiv;
        temp->suiv = nv;
    }
    return l;
}

listeLivre *ordonerListeLivre(listeLivre *tete){
    listeLivre *temp,*orListe=NULL;
    temp=tete;
    while(temp!=NULL){
        orListe=insererEnOrdre(orListe,temp->info);
        temp=temp->suiv;
    }
    return orListe;
}


listeLivre *supprimerLivre(int nombre,char* titre){
    listeLivre *temp=NULL;
    temp=liteLivre;
    while(strcmp(liteLivre->info->titre_livre,titre)==0 && nombre>0 && liteLivre->info->emprunteur_liv==-1){
        liteLivre=liteLivre->suiv;
        nombre --;
    }
    while(temp->suiv != NULL && nombre>0)
    {
        if(strcmp(temp->suiv->info->titre_livre,titre)==0 && temp->suiv->info->emprunteur_liv==-1){
            temp->suiv=temp->suiv->suiv;
            nombre --;
            continue;
        }
        temp=temp->suiv;
    }

    return liteLivre;
}
void compteurLivres(char *titre,int *emprunte,int *dispo,listeLivre *l){
    listeLivre *temp;
    temp=l;
    while(temp!=NULL){
        if(strcmp(temp->info->titre_livre,titre)==0){
            if (temp->info->emprunteur_liv==-1) *(dispo)+=1;
            else *(emprunte)+=1;
        }
        temp=temp->suiv;
    }
}
livre *RechercheLivre(char *titre,listeLivre *l){
    listeLivre *temp;
    temp=l;
    while(temp!=NULL){
        if(strcmp(temp->info->titre_livre,titre)==0){
            return temp->info;
        }
        temp=temp->suiv;
    }
    return NULL;
}
listeLivre *trouverPlageLivre(char* titre,listeLivre *livres){
    listeLivre* tab=NULL;
    int index=0;
    listeLivre *temp;
    temp=livres;
    while(temp!=NULL){
        if(strcmp(temp->info->titre_livre,titre)==0 && temp->info->emprunteur_liv!=-1){
            tab=insererEnOrdre(tab,temp->info);
        }
        temp=temp->suiv;
    }
    return tab;
}

//l'affichag d'un livre
void afficherLivre(livre* l){
    printf("le numero de livre est :%d\n",l->num_liv);
    printf("le titre de livre est :%s\n",l->titre_livre);
    printf("la categorie de livre est :%d\n",l->ctegorie);
    printf("le numero de l'emprunteur de ce livre :%d\n",l->emprunteur_liv);
    printf("les information de l'auteur :\n");
    printf("le nom :%s\n",l->auteur_liv.nom_aut);
    printf("le prenom :%s\n",l->auteur_liv.prenom_aut);
}
void on_livre_detail_clicked(GtkButton *button, app_widgets_livres *app_wdgts){//app_widgets_home *app_wdgts){
    char titre[30];
    int nbDispo=0,nbEmp=0;
    char CnbDispo[4],CnbEmp[4];
    strcpy(titre,gtk_widget_get_name((GtkWidget *) button));
    char nombre_empruntes[4];
    livre* donneesLivre=RechercheLivre(titre,liteLivre);
    compteurLivres(donneesLivre->titre_livre,&nbEmp,&nbDispo,liteLivre);
    sprintf(CnbDispo,"%d",nbDispo);
    sprintf(CnbEmp,"%d",nbEmp);
    gtk_label_set_label((GtkLabel *) app_wdgts->titre_liv_details, donneesLivre->titre_livre);
    gtk_label_set_label((GtkLabel *) app_wdgts->catregorie_liv_details, CATEGS[donneesLivre->ctegorie]);
    gtk_label_set_label((GtkLabel *) app_wdgts->aut_nom_liv_details, donneesLivre->auteur_liv.nom_aut);
    gtk_label_set_label((GtkLabel *) app_wdgts->nb_empr_liv, CnbEmp);
    gtk_label_set_label((GtkLabel *) app_wdgts->nb_desp_liv, CnbDispo);
    gtk_widget_hide(app_wdgts->container_table);
    gtk_widget_show(app_wdgts->box_detail);
}
//l'affichage de toute la iste des livres
void afichageListeLivre(listeLivre *tete, app_widgets_livres *appWidgetsLivre){
    listeLivre *temp;
    GtkWidget *tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    GtkWidget *ltitre,*lcategorie,*butDetail;
    GList *children, *iter;
    char id[4];
    temp=listeUniqueLivre(tete);
    temp=listeOrdoneCteg(temp);
    //temp=tete;
    children = gtk_container_get_children(GTK_CONTAINER(appWidgetsLivre->box_table));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);
    ltitre=gtk_label_new("Titre");
    lcategorie=gtk_label_new("Categorie");
    gtk_box_pack_start((GtkBox *) tempBox, ltitre, 1, 0, 0);
    gtk_box_pack_start((GtkBox *) tempBox, lcategorie, 1, 0, 0);
    gtk_box_pack_start((GtkBox *) tempBox, gtk_label_new("Actions"), 1, 1, 0);
    gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
    gtk_box_pack_start((GtkBox *) appWidgetsLivre->box_table, tempBox, 0, 1, 0);
    if(tete==NULL){tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        ltitre=gtk_label_new("Vide");
        gtk_box_pack_start((GtkBox *) tempBox, ltitre, 1, 1, 0);
        gtk_box_pack_start((GtkBox *) appWidgetsLivre->box_table, tempBox, 0, 1, 0);
        printf("la liste est vide !!");
    }
    else{

        while(temp!=NULL){
            afficherLivre(temp->info);//l'affichage dans le console

            tempBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
            ltitre = gtk_label_new(temp->info->titre_livre);
            lcategorie = gtk_label_new(CATEGS[temp->info->ctegorie]);
            butDetail = gtk_button_new_with_mnemonic("détails");
            gtk_widget_set_name(butDetail, temp->info->titre_livre);
            g_signal_connect(GTK_WIDGET(butDetail), "clicked", G_CALLBACK(on_livre_detail_clicked),
                             appWidgetsLivre);
            gtk_box_pack_start((GtkBox *) tempBox, ltitre, 1, 0, 0);
            gtk_box_pack_start((GtkBox *) tempBox, lcategorie, 1, 0, 0);
            gtk_box_pack_start((GtkBox *) tempBox, butDetail, 1, 1, 0);
            gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
            gtk_box_pack_start((GtkBox *) appWidgetsLivre->box_table, tempBox, 0, 1, 0);
            temp = temp->suiv;
        }
    }
    gtk_widget_show_all(appWidgetsLivre->box_table);
}

void afficherUnLive(int num,app_widgets_livres *appWidgetsLivre){
    listeLivre * temp=liteLivre;
    GList *children, *iter;
    char id[4];
    GtkWidget *tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    GtkWidget *ltitre,*lcategorie,*butDetail;
    children = gtk_container_get_children(GTK_CONTAINER(appWidgetsLivre->box_table));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);
    ltitre=gtk_label_new("Titre");
    lcategorie=gtk_label_new("Categorie");
    gtk_box_pack_start((GtkBox *) tempBox, ltitre, 1, 0, 0);
    gtk_box_pack_start((GtkBox *) tempBox, lcategorie, 1, 0, 0);
    gtk_box_pack_start((GtkBox *) tempBox, gtk_label_new("Actions"), 1, 1, 0);
    gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
    gtk_box_pack_start((GtkBox *) appWidgetsLivre->box_table, tempBox, 0, 1, 0);
    if (!liteLivre || num ==-1)
    {
        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        ltitre=gtk_label_new("Non Trouvé");
        gtk_box_pack_start((GtkBox *) tempBox, ltitre, 0, 1, 0);
        gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
        gtk_box_pack_start((GtkBox *) appWidgetsLivre->box_table, tempBox, 0, 1, 0);
        printf("Liste est vide\n");
    }
    while (temp && num!=temp->info->num_liv)
        temp=temp->suiv;
    if (temp){
        afficherLivre(temp->info);//l'affichage dans le console

        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        ltitre=gtk_label_new(temp->info->titre_livre);
        lcategorie=gtk_label_new(CATEGS[temp->info->ctegorie]);
        butDetail = gtk_button_new_with_mnemonic("détails");
        gtk_widget_set_name(butDetail,temp->info->titre_livre);
        g_signal_connect( GTK_WIDGET(butDetail), "clicked", G_CALLBACK( on_livre_detail_clicked ),appWidgetsLivre);
        gtk_box_pack_start((GtkBox *) tempBox, ltitre, 1, 0, 0);
        gtk_box_pack_start((GtkBox *) tempBox, lcategorie, 1, 0, 0);
        gtk_box_pack_start((GtkBox *) tempBox, butDetail, 1, 1, 0);
        gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
        gtk_box_pack_start((GtkBox *) appWidgetsLivre->box_table, tempBox, 0, 1, 0);
    }
    gtk_widget_show_all(appWidgetsLivre->box_table);
}
listeLivre *listeUniqueLivre(listeLivre *liste){
    listeLivre *ptrLivre=liste;
    listeLivre *listeFinale=NULL;
    char titre[50];
    if (ptrLivre) {
        strcpy(titre, ptrLivre->info->titre_livre);
        listeFinale=insererEnOrdre(listeFinale,ptrLivre->info);
        while (ptrLivre){
            if (strcmp(titre,ptrLivre->info->titre_livre)!=0){
                strcpy(titre, ptrLivre->info->titre_livre);
                listeFinale=insererEnOrdre(listeFinale,ptrLivre->info);
            }
            ptrLivre = ptrLivre->suiv;
        }
    }
    return listeFinale;
}
listeLivre *listeOrdoneCteg(listeLivre *l){
    listeLivre *ptrLivre;
    listeLivre *listeFinale=NULL;
    int indexCteg;
    for ( indexCteg = 3; indexCteg >=0; indexCteg--) {
        ptrLivre=l;
        while (ptrLivre){
            if (indexCteg==ptrLivre->info->ctegorie){
                listeFinale=insererListeEnTete(listeFinale,ptrLivre->info);
            }
            ptrLivre = ptrLivre->suiv;
        }
    }
    return listeFinale;
}

listeLivre *listeEmprinter(listeLivre *liste){
    listeLivre *ptrLivre=liste;
    listeLivre *listeFinale=NULL;
    while (ptrLivre){
        if (ptrLivre->info->emprunteur_liv != -1){
            listeFinale=insererEnOrdre(listeFinale,ptrLivre->info);
        }
        ptrLivre = ptrLivre->suiv;
    }
    return listeFinale;
}
listeLivre *listeDisponible(listeLivre* liste){
    listeLivre *ptrLivre=liste;
    listeLivre *listeFinale=NULL;
    while (ptrLivre){
        if (ptrLivre->info->emprunteur_liv == -1){
            listeFinale=insererEnOrdre(listeFinale,ptrLivre->info);
        }
        ptrLivre = ptrLivre->suiv;
    }
    return listeFinale;
}
listeLivre* chargerLivres(){//hna ta bxof wax khasseni n7ayad dk nombre_total_livres bax madirlix l maxakil dak wille kif lawal w ma tnssax txarchiga f lachae=rge des les autre
    FILE *f;
    livre* l=Malloc(livre);
    int i;
    liteLivre=NULL;
    f=fopen(livreFile,"rb");
    if(f==NULL)printf("erreur d'ouvertur\n");
    else{
        while (fread(l, sizeof(livre),1,f)==1){// la recuperation d'un seulle adh de fichier
            liteLivre=insererEnOrdre(liteLivre, l);
            l=Malloc(livre);
        }
        fclose(f);
    }
    return liteLivre;

}

void sauvgarderLivres(){
    listeLivre *liste=liteLivre;
    FILE *f=fopen(livreFile,"wb");
    if (!f) perror("err");
    else{
        while (liste){
            fwrite(liste->info, sizeof(livre),1,f);
            liste=liste->suiv;
        }
    }
    fclose(f);
}
void on_valider_ajouter_livre_clicked(GtkButton *button, app_widgets_livres *app_wdgts){
    char titre[20];
    int categorie;
    auteur aut;
    int nbDispo=0,nbEmpr=0;
    int nb_exempliare;
    char id[10];
    int i;

    livre *data;
    strcpy(id,gtk_combo_box_get_active_id((GtkComboBox *) app_wdgts->input_categorie_liv));
    strcpy(titre,gtk_entry_get_text((GtkEntry *) app_wdgts->input_titre_liv));
    categorie=atoi(id);
    strcpy(aut.nom_aut,gtk_entry_get_text((GtkEntry *) app_wdgts->input_nom_autr));
    strcpy(aut.prenom_aut,gtk_entry_get_text((GtkEntry *) app_wdgts->input_prenom_atr));
    strcpy(id,gtk_entry_get_text((GtkEntry *) app_wdgts->nbr_exemplaire));
    nb_exempliare=atoi(id);


    gtk_entry_set_text((GtkEntry *) app_wdgts->input_titre_liv, "");
    gtk_combo_box_set_active((GtkComboBox *) app_wdgts->input_categorie_liv,0);
    gtk_entry_set_text((GtkEntry *) app_wdgts->input_nom_autr, "");
    gtk_entry_set_text((GtkEntry *) app_wdgts->input_prenom_atr, "");
    gtk_entry_set_text((GtkEntry *) app_wdgts->nbr_exemplaire, "");

    gtk_dialog_response((GtkDialog *) app_wdgts->dialog_livre, GTK_RESPONSE_DELETE_EVENT);

    data=saisieLivre(titre, categorie, aut.nom_aut, aut.prenom_aut);
    if(data){
        if(app_wdgts->modify_or_add==1) {
            for (i = 0; i < nb_exempliare; ++i) {
                liteLivre = insererEnOrdre(liteLivre, data);
            }
        }
        else{
            compteurLivres(app_wdgts->modify_titre,&nbEmpr,&nbDispo,liteLivre);
            if (nbDispo>nb_exempliare)
                supprimerLivre(nbDispo-nb_exempliare,app_wdgts->modify_titre);
            else if (nbDispo<nb_exempliare){
                for (i = 0; i < nb_exempliare-nbDispo; ++i) {
                    liteLivre = insererEnOrdre(liteLivre, data);
                }
            }
            modifierLivre(liteLivre,app_wdgts->modify_titre,titre,categorie,&aut);
        }

    }
    afichageListeLivre(liteLivre, app_wdgts);
}
void on_cancel_ajouter_livre_clicked(GtkButton *button, app_widgets_livres *app_wdgts){
    gtk_dialog_response((GtkDialog *) app_wdgts->dialog_livre, GTK_RESPONSE_DELETE_EVENT);
}
void on_ajoute_livre_clicked(GtkButton *button,app_widgets_livres *appWidgets){
    appWidgets->modify_or_add=1;
    appWidgets->dialog_livre= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "dialog_livre"));
    appWidgets->valider_ajouter= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "valider_ajouter2"));
    appWidgets->cancel_ajouter= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "cancel_ajouter2"));
    g_signal_connect(GTK_WIDGET(appWidgets->valider_ajouter), "clicked", G_CALLBACK(on_valider_ajouter_livre_clicked ), appWidgets);
    g_signal_connect(GTK_WIDGET(appWidgets->cancel_ajouter), "clicked", G_CALLBACK(on_cancel_ajouter_livre_clicked ), appWidgets);

    gtk_dialog_run(GTK_DIALOG (appWidgets->dialog_livre));
    gtk_widget_hide(appWidgets->dialog_livre);
}
void on_retourn_clicked(GtkButton *button,app_widgets_livres *appWidgets){
    gtk_widget_hide(appWidgets->box_detail);
    gtk_widget_show(appWidgets->container_table);
}
void on_supprime_livre_clicked(GtkButton *button,app_widgets_livres *appWidgets){
    int nbDispo=0,nbEmpr=0;
    char titre[50];
    livre* data=NULL;
    strcpy(titre,gtk_label_get_text((GtkLabel *) appWidgets->titre_liv_details));
    data=RechercheLivre(titre,liteLivre);
    compteurLivres(data->titre_livre,&nbEmpr,&nbDispo,liteLivre);
    supprimerLivre(nbDispo,titre);
    afichageListeLivre(liteLivre,appWidgets);
    gtk_widget_hide(appWidgets->box_detail);
    gtk_widget_show(appWidgets->container_table);
}
void on_modifier_livre_clicked(GtkButton *button,app_widgets_livres *appWidgets){
    char titre[50];
    int nbDispo=0,nbEmpr=0;
    char CnbDispo[4];
    char categorie[4];
    livre* data=NULL;
    appWidgets->modify_or_add=0;
    strcpy(titre,gtk_label_get_text((GtkLabel *) appWidgets->titre_liv_details));
    data=RechercheLivre(titre,liteLivre);
    appWidgets->dialog_livre= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "dialog_livre"));
    appWidgets->valider_ajouter= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "valider_ajouter2"));
    appWidgets->cancel_ajouter= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "cancel_ajouter2"));
    g_signal_connect(GTK_WIDGET(appWidgets->valider_ajouter), "clicked", G_CALLBACK(on_valider_ajouter_livre_clicked ), appWidgets);
    g_signal_connect(GTK_WIDGET(appWidgets->cancel_ajouter), "clicked", G_CALLBACK(on_cancel_ajouter_livre_clicked ), appWidgets);

    compteurLivres(data->titre_livre,&nbEmpr,&nbDispo,liteLivre);
    sprintf(CnbDispo,"%d",nbDispo);
    sprintf(categorie,"%d",data->ctegorie);
    strcpy(appWidgets->modify_titre,titre);
    gtk_entry_set_text((GtkEntry *) appWidgets->input_titre_liv, data->titre_livre);
    gtk_combo_box_set_active_id((GtkComboBox *) appWidgets->input_categorie_liv,categorie);
    gtk_entry_set_text((GtkEntry *) appWidgets->input_nom_autr, data->auteur_liv.nom_aut);
    gtk_entry_set_text((GtkEntry *) appWidgets->input_prenom_atr, data->auteur_liv.prenom_aut);
    gtk_entry_set_text((GtkEntry *) appWidgets->nbr_exemplaire, CnbDispo);

    int resp=gtk_dialog_run(GTK_DIALOG (appWidgets->dialog_livre));
    if(resp==GTK_RESPONSE_DELETE_EVENT) {
        gtk_widget_hide(appWidgets->box_detail);
        gtk_widget_show(appWidgets->container_table);
    }
    gtk_widget_hide(appWidgets->dialog_livre);
}
void on_recherche_livre_clicked(GtkButton *button,app_widgets_livres *appWidgets){
    char titre[50];
    livre *liv;
    gtk_widget_hide(appWidgets->input_chercher_livre);
    gtk_widget_hide(appWidgets->chercher_livre);
    gtk_widget_show(appWidgets->annuler_chercher_livre);
    strcpy(titre,gtk_entry_get_text((GtkEntry *) appWidgets->input_chercher_livre));
    if(strlen(titre)!=0){
        liv=RechercheLivre(titre,liteLivre);
        if(liv) {
            afficherUnLive(liv->num_liv, appWidgets);
            return;
        }
    }
    afficherUnLive(-1,appWidgets);
}
void on_cancel_recherche_livre_clicked(GtkButton *button,app_widgets_livres *appWidgets){
    gtk_widget_show(appWidgets->input_chercher_livre);
    gtk_widget_show(appWidgets->chercher_livre);
    gtk_widget_hide(appWidgets->annuler_chercher_livre);
    afichageListeLivre(liteLivre,appWidgets);

}
void gestionLivres(app_widgets_home *appWidgetsHome){
    GList *children, *iter;
    livre *donnes=NULL;
    app_widgets_livres *appWidgetsLivre=g_slice_new(app_widgets_livres);
    appWidgetsLivre->builder=appWidgetsHome->builder;

    appWidgetsLivre->box_detail=GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "detail_liv"));
    appWidgetsLivre->box_table= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "table_livres"));
    appWidgetsLivre->container_table= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "body_livre"));
    // pour le tool bar
    appWidgetsLivre->ajouter_livre= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "ajouter_livre"));
    appWidgetsLivre->chercher_livre= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "recherche_livre"));
    appWidgetsLivre->annuler_chercher_livre= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "annuler_recherche_livre"));
    appWidgetsLivre->input_chercher_livre= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "input_chercher_livre"));
    appWidgetsLivre->categorie= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "categorie"));
    appWidgetsLivre->ajouter_livre= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "ajouter_livre"));
    //le traitment sur les detailes
    appWidgetsLivre->titre_liv_details= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "titre_liv_details"));
    appWidgetsLivre->catregorie_liv_details= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "catregorie_liv_details"));
    appWidgetsLivre->aut_nom_liv_details= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "aut_nom_liv_details"));
    appWidgetsLivre->aut_prenom_liv_details= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "aut_prenom_liv_details"));
    appWidgetsLivre->nb_empr_liv= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "nb_empr_liv"));
    appWidgetsLivre->nb_desp_liv= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "nb_desp_liv"));
    appWidgetsLivre->retourner_livre= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "retourner_livre"));
    appWidgetsLivre->supprimer_liv= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "supprimer_liv"));
    appWidgetsLivre->modifier_liv= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "modifier_liv"));
    // le traitement de la cambobox
    appWidgetsLivre->input_titre_liv= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "titre_liv"));
    appWidgetsLivre->input_categorie_liv= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "categorie_liv"));
    appWidgetsLivre->input_prenom_atr= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "prenom_atr"));
    appWidgetsLivre->input_nom_autr= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "nom_autr"));
    appWidgetsLivre->valider_ajouter= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "valider_ajouter_livre"));
    appWidgetsLivre->cancel_ajouter= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "cancel_ajouter_livre"));
    appWidgetsLivre->dialog_livre= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "dialog_livre"));
    appWidgetsLivre->nbr_exemplaire= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "nbr_exemplaire"));

    g_signal_connect(GTK_WIDGET(appWidgetsLivre->ajouter_livre), "clicked", G_CALLBACK(on_ajoute_livre_clicked ), appWidgetsLivre);
    g_signal_connect(GTK_WIDGET(appWidgetsLivre->valider_ajouter), "clicked", G_CALLBACK(on_valider_ajouter_livre_clicked), appWidgetsLivre);
    g_signal_connect(GTK_WIDGET(appWidgetsLivre->cancel_ajouter), "clicked", G_CALLBACK(on_cancel_ajouter_livre_clicked), appWidgetsLivre);
    g_signal_connect(GTK_WIDGET(appWidgetsLivre->retourner_livre), "clicked", G_CALLBACK(on_retourn_clicked ), appWidgetsLivre);
    g_signal_connect(GTK_WIDGET(appWidgetsLivre->supprimer_liv), "clicked", G_CALLBACK(on_supprime_livre_clicked ), appWidgetsLivre);
    g_signal_connect(GTK_WIDGET(appWidgetsLivre->modifier_liv), "clicked", G_CALLBACK(on_modifier_livre_clicked ), appWidgetsLivre);
    g_signal_connect(GTK_WIDGET(appWidgetsLivre->chercher_livre), "clicked", G_CALLBACK(on_recherche_livre_clicked), appWidgetsLivre);
    g_signal_connect(GTK_WIDGET(appWidgetsLivre->annuler_chercher_livre), "clicked", G_CALLBACK(on_cancel_recherche_livre_clicked ), appWidgetsLivre);

    afichageListeLivre(liteLivre, appWidgetsLivre);
    gtk_builder_connect_signals(appWidgetsLivre->builder, appWidgetsLivre);
    gtk_widget_hide(appWidgetsLivre->annuler_chercher_livre);
    gtk_widget_hide(appWidgetsLivre->box_detail);

}