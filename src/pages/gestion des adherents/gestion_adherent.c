//
// Created by iliass on 1/6/21.
//

#include "gestion_adherent.h"

listeAdherents liste = NULL;
void on_valider_ajouter_clicked(GtkButton *button, app_widgets_adher *app_wdgts);
void on_cancel_ajouter_clicked(GtkButton *button, app_widgets_adher *app_wdgts);
void on_detail_clicked(GtkButton *button, app_widgets_adher *app_wdgts);//app_widgets_home *app_wdgts);
// Function to check the character
// is an alphabet or not
int isChar(char c){
    return ((c >= 'a' && c <= 'z')
            || (c >= 'A' && c <= 'Z'));
}

// Function to check the character
// is an digit or not
int isDigit(const char c){
    return (c >= '0' && c <= '9');
}

// Function to check email id is
// valid or not
int isValidEmail(char* email){
    int i;
    // Check the first character
    // is an alphabet or not
    if (!isChar(email[0])) {

        // If it's not an alphabet
        // email id is not valid
        return 0;
    }
    // Variable to store position
    // of At and Dot
    int At = -1, Dot = -1;

    // Traverse over the email id
    // string to find position of
    // Dot and At
    for (i = 0;i < strlen(email); i++) {

        // If the character is '@'
        if (email[i] == '@') {
            At = i;
        }
            // If character is '.'
        else if (email[i] == '.') {
            Dot = i;
        }
    }

    // If At or Dot is not present
    if (At == -1 || Dot == -1)
        return 0;

    // If Dot is present before At
    if (At > Dot)
        return 0;

    // If Dot is present at the end
    return Dot < (strlen(email) - 1);
}
int createUniqueCode(){
    FILE *f=NULL;
    int count,read;
    f=fopen(adhCount,"rb");
    if (!f){
        count=0;
    } else {
        fread(&count,sizeof(int),1,f);
        fclose(f);
    }
    f=fopen(adhCount,"wb");
    if (!f){
        perror("Err");
    } else {
        count++;
        fwrite(&count,sizeof (int),1,f);
    }
    fclose(f);
    return count-1;
}
donnees* creerAdherent(char *nom_adh,char *prenom_adh,char *adresse_email,char *adresse_personnelle){
    donnees* p=Malloc(donnees);
    if (strlen(nom_adh)!=0 && strlen(prenom_adh)!=0 && strlen(adresse_email)!=0 && strlen(adresse_personnelle)!=0) {
        if(isValidEmail(adresse_email)){
            p->num_adh = createUniqueCode();
            strcpy(p->nom_adh, nom_adh);
            strcpy(p->prenom_adh, prenom_adh);
            strcpy(p->adresse_email, adresse_email);
            strcpy(p->adresse_personnelle, adresse_personnelle);
            p->nbre_emprunts_adh = 0;
            return p;
        }
    }
    return NULL;

}
donnees* cpyAdherent(int num,char *nom_adh,char *prenom_adh,char *adresse_email,char *adresse_personnelle,int nbre_emprunts_adh){
    donnees* p=Malloc(donnees);
    if(isValidEmail(adresse_email)){
        p->num_adh = num;
        strcpy(p->nom_adh, nom_adh);
        strcpy(p->prenom_adh, prenom_adh);
        strcpy(p->adresse_email, adresse_email);
        strcpy(p->adresse_personnelle, adresse_personnelle);
        p->nbre_emprunts_adh = nbre_emprunts_adh;
        return p;
    }
    return NULL;
}
listeAdherents insererAdherent(donnees * data,listeAdherents tete){
    adherent *new=Malloc(adherent);
    new->infos=data;
    adherent *p=tete;
    if (!p || strcmp(data->nom_adh,tete->infos->nom_adh)<=0)
    {
        new->suivant=tete;
        tete=new;
        return tete;
    }
    p=tete;
    while(p->suivant && strcmp(data->nom_adh,p->suivant->infos->nom_adh)>=0)
    {
        p=p->suivant;
    }
    new->suivant=p->suivant;
    p->suivant=new;
    return tete;
}
listeAdherents chargerAdherents(){
    liste=NULL;
    printf("data %s\n",adhFile);
    donnees * data=Malloc(donnees);
    FILE *f=fopen(adhFile,"rb");
    if (!f) return NULL;
    else{
        while (fread(data, sizeof(donnees),1,f)==1){
            printf("%ld\n",ftell(f));
            liste=insererAdherent(data,liste);
            data=Malloc(donnees);
        }

    }
    fclose(f);
    return liste;
}
void sauvgarderAdherent(){
    listeAdherents p=liste;
    FILE *f=fopen(adhFile,"wb");
    if (!f) perror("err");
    else{
        while (p){
            fwrite(p->infos, sizeof(donnees),1,f);
            p=p->suivant;
        }
    }
    fclose(f);
}
donnees* rechercherAdherent(char* nom,char* prenom){
    listeAdherents p=liste;
    while (p && (strcmp(nom,p->infos->nom_adh)!=0 || strcmp(prenom,p->infos->prenom_adh)!=0))
        p=p->suivant;
    if (p) return p->infos;
    return NULL;
}
donnees* trouverAdherentParID(int numAdr,listeAdherents tete){
    listeAdherents p=tete;
    while (p && p->infos->num_adh!=numAdr)
        p=p->suivant;
    if (p) return p->infos;
    return NULL;
}
void modifierAdherent(int num,listeAdherents tete,char *nom_adh,char *prenom_adh,char *adresse_email,char *adresse_personnelle){
    adherent* p=tete;
    while (p && num!=p->infos->num_adh)
        p=p->suivant;
    if (p){
        p->infos=cpyAdherent(p->infos->num_adh,nom_adh,prenom_adh,adresse_email,adresse_personnelle,p->infos->nbre_emprunts_adh);
    }
}
void afficherAdherent(listeAdherents tete,app_widgets_adher *appWidgetsAdher){
    adherent* p=tete;
    GtkWidget *tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    GtkWidget *lnom,*lprenom,*butDetail;
    char id[4];
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(appWidgetsAdher->table_adr));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);
    //le titre
    lnom=gtk_label_new("Nom");
    lprenom=gtk_label_new("Prénom");
    gtk_box_pack_start((GtkBox *) tempBox, lnom, 1, 0, 0);
    gtk_box_pack_start((GtkBox *) tempBox, lprenom, 1, 0, 0);
    gtk_box_pack_start((GtkBox *) tempBox, gtk_label_new("Actions"), 1, 1, 0);
    gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
    gtk_box_pack_start((GtkBox *) appWidgetsAdher->table_adr, tempBox, 0, 1, 0);
    if (!tete)
    {
        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        lnom=gtk_label_new("Vide");
        gtk_box_pack_start((GtkBox *) tempBox, lnom, 1, 1, 0);
        gtk_box_pack_start((GtkBox *) appWidgetsAdher->table_adr, tempBox, 0, 1, 0);
        printf("Liste est vide\n");
    }
    while(p){
        //dans GTK
        sprintf(id, "%d", p->infos->num_adh);
        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        lnom=gtk_label_new(p->infos->nom_adh);
        lprenom=gtk_label_new(p->infos->prenom_adh);
        butDetail = gtk_button_new_with_mnemonic("détails");
        gtk_widget_set_name(butDetail,id);
        g_signal_connect( GTK_WIDGET(butDetail), "clicked", G_CALLBACK( on_detail_clicked ),appWidgetsAdher);
        gtk_box_pack_start((GtkBox *) tempBox, lnom, 1, 0, 0);
        gtk_box_pack_start((GtkBox *) tempBox, lprenom, 1, 0, 0);
        gtk_box_pack_start((GtkBox *) tempBox, butDetail, 1, 1, 0);
        gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
        gtk_box_pack_start((GtkBox *) appWidgetsAdher->table_adr, tempBox, 0, 1, 0);
        //consol
        printf("------------------------------\n");
        printf("| Code:		%d\t\t|\n",p->infos->num_adh);
        printf("| Nom:		%s\t\t|\n",p->infos->nom_adh);
        printf("| Nom:		%s\t\t|\n",p->infos->prenom_adh);
        printf("| Adresse:	%s\t\t|\n",p->infos->adresse_personnelle);
        printf("| Adresse:	%s\t\t|\n",p->infos->adresse_email);
        printf("| Domaine:	%d\t\t|\n",p->infos->nbre_emprunts_adh);
        printf("------------------------------\n");
        printf("               |\n");
        printf("               V\n");
        p=p->suivant;
    }
    printf("-----------------------------\n");
    printf("|            VIDE           |\n");
    printf("-----------------------------\n");
    gtk_widget_show_all(appWidgetsAdher->table_adr);
}
void afficherUnAdherent(int numAdhr,app_widgets_adher *appWidgetsAdher){
    adherent* p=liste;
    GList *children, *iter;
    char id[4];
    GtkWidget *tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    GtkWidget *lnom,*lprenom,*butDetail;
    children = gtk_container_get_children(GTK_CONTAINER(appWidgetsAdher->table_adr));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);
    lnom=gtk_label_new("Nom");
    lprenom=gtk_label_new("Prénom");
    gtk_box_pack_start((GtkBox *) tempBox, lnom, 1, 0, 0);
    gtk_box_pack_start((GtkBox *) tempBox, lprenom, 1, 0, 0);
    gtk_box_pack_start((GtkBox *) tempBox, gtk_label_new("Actions"), 1, 1, 0);
    gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
    gtk_box_pack_start((GtkBox *) appWidgetsAdher->table_adr, tempBox, 0, 1, 0);
    if (!liste || numAdhr==-1)
    {
        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        lnom=gtk_label_new("Non Trouvé");
        gtk_box_pack_start((GtkBox *) tempBox, lnom, 0, 1, 0);
        gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
        gtk_box_pack_start((GtkBox *) appWidgetsAdher->table_adr, tempBox, 0, 1, 0);
        printf("Liste est vide\n");
    }
    while (p && numAdhr!=p->infos->num_adh)
        p=p->suivant;
    if (p){
        sprintf(id, "%d", p->infos->num_adh);
        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        lnom=gtk_label_new(p->infos->nom_adh);
        lprenom=gtk_label_new(p->infos->prenom_adh);
        butDetail = gtk_button_new_with_mnemonic("détails");
        gtk_widget_set_name(butDetail,id);
        g_signal_connect( GTK_WIDGET(butDetail), "clicked", G_CALLBACK( on_detail_clicked ),appWidgetsAdher);
        gtk_box_pack_start((GtkBox *) tempBox, lnom, 1, 0, 0);
        gtk_box_pack_start((GtkBox *) tempBox, lprenom, 1, 0, 0);
        gtk_box_pack_start((GtkBox *) tempBox, butDetail, 1, 1, 0);
        gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
        gtk_box_pack_start((GtkBox *) appWidgetsAdher->table_adr, tempBox, 0, 1, 0);
        printf("------------------------------\n");
        printf("| Code:		%d\t\t|\n",p->infos->num_adh);
        printf("| Nom:		%s\t\t|\n",p->infos->nom_adh);
        printf("| Nom:		%s\t\t|\n",p->infos->prenom_adh);
        printf("| Adresse:	%s\t\t|\n",p->infos->adresse_personnelle);
        printf("| Adresse:	%s\t\t|\n",p->infos->adresse_email);
        printf("| Domaine:	%d\t\t|\n",p->infos->nbre_emprunts_adh);
        printf("------------------------------\n");
    }
    gtk_widget_show_all(appWidgetsAdher->table_adr);
}
listeAdherents supprimerAdherent(int num,listeAdherents tete){
    adherent* p=tete;
    if (!p)
    {
        printf("La liste est vide\n");
        return tete;
    }

    if(num==p->infos->num_adh)
    {
        tete=p->suivant;
        free(p);
        return tete;
    }
    while(p->suivant && num!=p->suivant->infos->num_adh)
    {
        p=p->suivant;
    }
    if (p->suivant)
    {
        adherent *q=p->suivant;
        p->suivant=q->suivant;
        free(q);
    }else{
        printf("L'element nombre_total_livres'est pas trouve\nombre_total_livres");
    }
    return tete;
}
/*
void on_ajouter_adher_clicked(GtkButton *button, app_widgets_home *app_wdgts){
     // Check return value from Open Text File dialog box to see if user clicked the Open button
     if (gtk_dialog_run(GTK_DIALOG (app_wdgts->dialog)) == GTK_RESPONSE_OK) {

     }
     // Finished with the "Open Text File" dialog box, so hide it
     gtk_widget_hide(app_wdgts->dialog);
}
 */


void on_ajouter_adr_clicked(GtkButton *button,app_widgets_adher *appWidgets){
    appWidgets->modify_or_add=1;
    appWidgets->dialog= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "dialog_adher"));
    appWidgets->anuller_ajout= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "cancel_ajouter"));
    appWidgets->valider_ajout= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "valider_ajouter"));
    g_signal_connect(GTK_WIDGET(appWidgets->valider_ajout), "clicked", G_CALLBACK(on_valider_ajouter_clicked ), appWidgets);
    g_signal_connect(GTK_WIDGET(appWidgets->anuller_ajout), "clicked", G_CALLBACK(on_cancel_ajouter_clicked ), appWidgets);

    gtk_dialog_run(GTK_DIALOG (appWidgets->dialog));
    //gtk_widget_show(appWidgets->dialog);
    //gtk_widget_hide(appWidgets->dialog);
    gtk_widget_hide(appWidgets->dialog);
}
void chercher_par_nom(GtkButton *button,app_widgets_adher *appWidgets){
    char mot[100],nom[20],prenom[20];
    donnees *data;
    gtk_widget_hide(appWidgets->search);
    gtk_widget_hide(appWidgets->btn_chercher);
    gtk_widget_show(appWidgets->btn_cancel_chercher);
    strcpy(mot,gtk_entry_get_text((GtkEntry *) appWidgets->search));
    if(strlen(mot)!=0){
        char * pch;
        pch = strtok (mot," ");
        strcpy(nom,pch);
        pch = strtok (NULL, " ");
        if (pch) {
            strcpy(prenom,pch);
            data=rechercherAdherent(nom,prenom);
            if(data) afficherUnAdherent(data->num_adh,appWidgets);
            return;
        }
    }
    afficherUnAdherent(-1,appWidgets);

}
void fermer_recherche(GtkButton *button,app_widgets_adher *appWidgets){
    gtk_widget_show(appWidgets->search);
    gtk_widget_show(appWidgets->btn_chercher);
    gtk_widget_hide(appWidgets->btn_cancel_chercher);
    afficherAdherent(liste, appWidgets);
}
void on_supprimer_adherent(GtkButton *button, app_widgets_adher *appWidgets){
    donnees *data=rechercherAdherent(gtk_label_get_label(appWidgets->nom_ader_details),gtk_label_get_label((GtkLabel *) appWidgets->prenom_ader_details));
    liste=supprimerAdherent(data->num_adh,liste);
    afficherAdherent(liste, appWidgets);
    gtk_widget_hide(appWidgets->box_detail);
    gtk_widget_show(appWidgets->box_table);
}
void on_modifier_adherent(GtkButton *button, app_widgets_adher *appWidgets){
    donnees *data=rechercherAdherent(gtk_label_get_label(appWidgets->nom_ader_details),gtk_label_get_label((GtkLabel *) appWidgets->prenom_ader_details));
    gtk_entry_set_text((GtkEntry *) appWidgets->input_nom, data->nom_adh);
    gtk_entry_set_text((GtkEntry *) appWidgets->input_prenom, data->prenom_adh);
    gtk_entry_set_text((GtkEntry *) appWidgets->input_email, data->adresse_email);
    gtk_entry_set_text((GtkEntry *) appWidgets->input_adrs, data->adresse_personnelle);
    appWidgets->modify_id=data->num_adh;
    appWidgets->modify_or_add=0;
    appWidgets->dialog= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "dialog_adher"));
    appWidgets->anuller_ajout= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "cancel_ajouter"));
    appWidgets->valider_ajout= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "valider_ajouter"));
    g_signal_connect(GTK_WIDGET(appWidgets->valider_ajout), "clicked", G_CALLBACK(on_valider_ajouter_clicked ), appWidgets);
    g_signal_connect(GTK_WIDGET(appWidgets->anuller_ajout), "clicked", G_CALLBACK(on_cancel_ajouter_clicked ), appWidgets);

    int resp=gtk_dialog_run(GTK_DIALOG (appWidgets->dialog));
    if(resp==GTK_RESPONSE_DELETE_EVENT) {
        gtk_widget_hide(appWidgets->box_detail);
        gtk_widget_show(appWidgets->box_table);
    }

    gtk_widget_hide(appWidgets->dialog);
}

void on_detail_clicked(GtkButton *button, app_widgets_adher *app_wdgts){//app_widgets_home *app_wdgts){
    int id=atoi(gtk_widget_get_name((GtkWidget *) button));
    char nombre_emrintes[4];
    donnees* data=trouverAdherentParID(id,liste);
    sprintf(nombre_emrintes,"%d",data->nbre_emprunts_adh);
    gtk_label_set_label((GtkLabel *) app_wdgts->nom_ader_details, data->nom_adh);
    gtk_label_set_label((GtkLabel *) app_wdgts->prenom_ader_details, data->prenom_adh);
    gtk_label_set_label((GtkLabel *) app_wdgts->email_ader_details, data->adresse_email);
    gtk_label_set_label((GtkLabel *) app_wdgts->adrs_ader_details, data->adresse_personnelle);
    gtk_label_set_label((GtkLabel *) app_wdgts->nb_empr_adr, nombre_emrintes);
    gtk_widget_hide(app_wdgts->box_table);
    gtk_widget_show(app_wdgts->box_detail);
}
void on_retourner_clicked(GtkButton *button, app_widgets_adher *app_wdgts){
    gtk_widget_hide(app_wdgts->box_detail);
    gtk_widget_show(app_wdgts->box_table);
}

void on_valider_ajouter_clicked(GtkButton *button, app_widgets_adher *app_wdgts){
    char nom[20];
    char prenom[20];
    char email[100];
    char adrs[100];

    donnees *data;
    //gtk_widget_hide(app_wdgts->dialog);
    //gtk_window_close((GtkWindow *) app_wdgts->dialog);
    strcpy(nom,gtk_entry_get_text((GtkEntry *) app_wdgts->input_nom));
    strcpy(prenom,gtk_entry_get_text((GtkEntry *) app_wdgts->input_prenom));
    strcpy(email,gtk_entry_get_text((GtkEntry *) app_wdgts->input_email));
    strcpy(adrs,gtk_entry_get_text((GtkEntry *) app_wdgts->input_adrs));

    gtk_entry_set_text((GtkEntry *) app_wdgts->input_nom, "");
    gtk_entry_set_text((GtkEntry *) app_wdgts->input_prenom, "");
    gtk_entry_set_text((GtkEntry *) app_wdgts->input_email, "");
    gtk_entry_set_text((GtkEntry *) app_wdgts->input_adrs, "");

    //gtk_widget_hide(app_wdgts->dialog);//cmnt
    gtk_dialog_response((GtkDialog *) app_wdgts->dialog, GTK_RESPONSE_DELETE_EVENT);
    data=creerAdherent(nom,prenom,email,adrs);
    if(data){
        if(app_wdgts->modify_or_add==1) liste=insererAdherent(data,liste);
        else modifierAdherent(app_wdgts->modify_id,liste,data->nom_adh,data->prenom_adh,data->adresse_email,data->adresse_personnelle);
    }
    afficherAdherent(liste,app_wdgts);

}
void on_cancel_ajouter_clicked(GtkButton *button, app_widgets_adher *app_wdgts){
    gtk_dialog_response((GtkDialog *) app_wdgts->dialog, GTK_RESPONSE_DELETE_EVENT);
}
void gestionAdherents(app_widgets_home *appWidgetsHome){
    GList *children, *iter;
    donnees *data=NULL;
    app_widgets_adher *appWidgetsAdher=g_slice_new(app_widgets_adher);
    appWidgetsAdher->builder=appWidgetsHome->builder;
    appWidgetsAdher->table_adr= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "table_adr"));
    appWidgetsAdher->box_detail= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "details_adr"));
    appWidgetsAdher->box_table= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "body_adr"));
    appWidgetsAdher->ajouter_adr= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "ajouter_adr"));
    appWidgetsAdher->modifier_adr= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "modifier_adr"));
    appWidgetsAdher->supprimer_adr= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "supprimer_adr"));
    appWidgetsAdher->nom_ader_details= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "nom_ader_details"));
    appWidgetsAdher->prenom_ader_details= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "prenom_ader_details"));
    appWidgetsAdher->email_ader_details= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "email_ader_details"));
    appWidgetsAdher->adrs_ader_details= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "adrs_ader_details"));
    appWidgetsAdher->nb_empr_adr= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "nb_empr_adher"));
    appWidgetsAdher->dialog= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "dialog_adher"));
    appWidgetsAdher->input_nom= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "nom_adher"));
    appWidgetsAdher->input_prenom= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "prenom_adher"));
    appWidgetsAdher->input_email= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "email_adher"));
    appWidgetsAdher->input_adrs= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "adrs_adher"));
    appWidgetsAdher->anuller_ajout= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "cancel_ajouter"));
    appWidgetsAdher->valider_ajout= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "valider_ajouter"));
    appWidgetsAdher->retour_table= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "retourner_adr"));
    appWidgetsAdher->search= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "chercher_adr"));
    appWidgetsAdher->btn_chercher= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "btn_chercher"));
    appWidgetsAdher->btn_cancel_chercher= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "btn_clear"));
    appWidgetsAdher->btn_supprimer= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "supprimer_adr"));
    appWidgetsAdher->btn_modifier= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "modifier_adr"));
    g_signal_connect(GTK_WIDGET(appWidgetsAdher->ajouter_adr), "clicked", G_CALLBACK(on_ajouter_adr_clicked ), appWidgetsAdher);
    g_signal_connect(GTK_WIDGET(appWidgetsAdher->valider_ajout), "clicked", G_CALLBACK(on_valider_ajouter_clicked ), appWidgetsAdher);
    g_signal_connect(GTK_WIDGET(appWidgetsAdher->anuller_ajout), "clicked", G_CALLBACK(on_cancel_ajouter_clicked ), appWidgetsAdher);
    g_signal_connect(GTK_WIDGET(appWidgetsAdher->retour_table), "clicked", G_CALLBACK(on_retourner_clicked ), appWidgetsAdher);
    g_signal_connect(GTK_WIDGET(appWidgetsAdher->btn_chercher), "clicked", G_CALLBACK(chercher_par_nom ), appWidgetsAdher);
    g_signal_connect(GTK_WIDGET(appWidgetsAdher->btn_cancel_chercher), "clicked", G_CALLBACK(fermer_recherche ), appWidgetsAdher);
    g_signal_connect(GTK_WIDGET(appWidgetsAdher->btn_modifier), "clicked", G_CALLBACK(on_modifier_adherent), appWidgetsAdher);
    g_signal_connect(GTK_WIDGET(appWidgetsAdher->btn_supprimer), "clicked", G_CALLBACK(on_supprimer_adherent), appWidgetsAdher);


    afficherAdherent(liste, appWidgetsAdher);
    gtk_builder_connect_signals(appWidgetsAdher->builder, appWidgetsAdher);
    gtk_widget_hide(appWidgetsAdher->btn_cancel_chercher);
    gtk_widget_hide(appWidgetsAdher->box_detail);

/*
    children = gtk_container_get_children(GTK_CONTAINER(app_wdgts->body));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    app_wdgts->body_a= GTK_WIDGET(gtk_builder_get_object(builder,"body_adher"));
    GtkWidget *first=GTK_WIDGET(gtk_builder_get_object(builder,"tool_bar_adher"));
    GtkWidget *sec=GTK_WIDGET(gtk_builder_get_object(builder,"scrolled_win"));
    GtkWidget *add=GTK_WIDGET(gtk_builder_get_object(builder,"ajouter_adher"));
    g_signal_connect (add, "clicked",
                      G_CALLBACK (on_valider_ajouter_clicked), NULL);
    app_wdgts->body=GTK_WIDGET(gtk_builder_get_object(app_wdgts->builder,"body_home"));
    gtk_widget_unparent(first);
    gtk_widget_unparent(sec);
    gtk_container_add((GtkContainer *) app_wdgts->body, first);
    gtk_container_add((GtkContainer *) app_wdgts->body, sec);
    gtk_builder_connect_signals(app_wdgts->builder, app_wdgts);
    /*
    children = gtk_container_get_children(GTK_CONTAINER(app_wdgts->body_a));
    for(iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_unparent(iter->data);
        gtk_container_add((GtkContainer *) app_wdgts->body, iter->data);
    }
    g_list_free(children);
     */
}
