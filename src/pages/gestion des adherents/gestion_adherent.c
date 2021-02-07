#include "gestion_adherent.h"

listeAdherents liste = NULL;
void on_valider_ajouter_clicked(GtkButton *button, app_widgets_adher *app_wdgts);
void on_cancel_ajouter_clicked(GtkButton *button, app_widgets_adher *app_wdgts);
void on_detail_clicked(GtkButton *button, app_widgets_adher *app_wdgts);
// Fonction pour vérifier le caractère
// est un alphabet ou non
int isChar(char c){
    return ((c >= 'a' && c <= 'z')
            || (c >= 'A' && c <= 'Z'));
}

// Fonction pour vérifier le caractère
// est un chiffre ou non
int isDigit(const char c){
    return (c >= '0' && c <= '9');
}

// Fonction pour vérifier l’id e-mail est
// valide ou non
int isValidEmail(char* email){
    int i;
    // Vérifiez le premier caractère
    // est un alphabet ou non
    if (!isChar(email[0])) {

        // Si ce n’est pas un alphabet
        // id e-mail n’est pas valide
        return 0;
    }
    // Position variable au magasin
    // de At et Dot
    int At = -1, Dot = -1;

    // Traversez sur l’id e-mail
    // chaîne pour trouver la position de
    // Dot et At
    for (i = 0;i < strlen(email); i++) {

        // si le charactre est '@'
        if (email[i] == '@') {
            At = i;
        }
            // si le charactere est  '.'
        else if (email[i] == '.') {
            Dot = i;
        }
    }

    // si At ou Dot n’est pas présent
    if (At == -1 || Dot == -1)
        return 0;

    // si Dot est present Avant At
    if (At > Dot)
        return 0;

    // si Dot est présent à la fin
    return Dot < (strlen(email) - 1);
}
int createUniqueCode(){
    FILE *f=NULL;
    int count,read;
    f=fopen(adhCount,"rb");//ouvre le fichier pointéen mode lecture (binaire).
    if (!f){//si il y a un erreur au moment d'ouverture de la fiche
        count=0;
    } else {
        fread(&count,sizeof(int),1,f);//la recuperation de nombre des element de la liste qui existe dans le fichier
        fclose(f);//la furmuture du fichier
    }
    f=fopen(adhCount,"wb");//ouvre le fichier pointé en mode ecriture(binaire).
    if (!f){//si il y a un erreur au moment d'ouverture de la fiche
        perror("Err");
    } else {
        count++;//incrumenter le nombre max par 1
        fwrite(&count,sizeof (int),1,f);//et en l'ecrire dans le fichier
    }
    fclose(f);//la furmuture de fichier
    return count-1;//returner l'indice
}
donnees* creerAdherent(char *nom_adh,char *prenom_adh,char *adresse_email,char *adresse_personnelle){
    donnees* p=Malloc(donnees);//l'allocation memoire pour les information de l'adhrent
    if (strlen(nom_adh)!=0 && strlen(prenom_adh)!=0 && strlen(adresse_email)!=0 && strlen(adresse_personnelle)!=0) {//si tous les information son bien saisie
        if(isValidEmail(adresse_email)){//et si l'adresse email est valide
            p->num_adh = createUniqueCode();//generer le code de la nouvelle adhrent a ajouter
            strcpy(p->nom_adh, nom_adh);//remplissage le nom de l'adhérent
            strcpy(p->prenom_adh, prenom_adh);//remplissage du prenom de l'adhérent
            strcpy(p->adresse_email, adresse_email);//remplissage de l'email de l'adhérent
            strcpy(p->adresse_personnelle, adresse_personnelle);//remplissage de l'adresse  de l'adhérent
            p->nbre_emprunts_adh = 0;//l'initialisation toujours de nombre d'emprunts à 0
            return p;//retorner l'adhrent que en est cree
        }
    }
    return NULL;//si une des conditions n'est pas vérifier

}
donnees* cpyAdherent(int num,char *nom_adh,char *prenom_adh,char *adresse_email,char *adresse_personnelle,int nbre_emprunts_adh){
    donnees* p=Malloc(donnees);//l'allocation mémoire pour la zone dans la quelle en mettre une copy de notre adhèrent
    if(isValidEmail(adresse_email)){//et si l'adresse email est valide
        //la remplissage de toute les information concernent la copy  de l'adhérent
        p->num_adh = num;//copie le code de la  adhrent
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
    adherent *new=Malloc(adherent);//la location memoire pour la nouvelle adhrent a ajouter
    new->infos=data;//l'initialisation de les information de la nouvelle adh par le s data que on est passe dans les parametre de la fonction
    adherent *p=tete;//declaration d'un variav=ble temp pour parcoraire la liste
    if (!p || strcmp(data->nom_adh,tete->infos->nom_adh)<=0)//si la liste est vide en ajoute en tete
    {
        new->suivant=tete;
        tete=new;
        return tete;
    }
    p=tete;
    while(p->suivant && strcmp(data->nom_adh,p->suivant->infos->nom_adh)>=0)//en parcoure la liste pour atendre la fin de la liste
    {
        p=p->suivant;
    }
    new->suivant=p->suivant;//en ajoute la nouvele adh a la fin de la liste
    p->suivant=new;
    return tete;
}
listeAdherents chargerAdherents(){
    liste=NULL;
    printf("data %s\n",adhFile);
    donnees * data=Malloc(donnees);//l'allocation memoire d=pour les donner de l'adhrent
    FILE *f=fopen(adhFile,"rb");//ouverture de la fichier des adhrents en mode lecture
    if (!f) return NULL;//au cas d'un erreur d'ouverture de fichier
    else{
        while (fread(data, sizeof(donnees),1,f)==1){// la recuperation d'un seulle adh de fichier
            printf("%ld\n",ftell(f));
            liste=insererAdherent(data,liste);//l'ajout de ce element dans la liste des adhrents
            data=Malloc(donnees);
        }

    }
    fclose(f);//fermeture de fichier des adhrents
    return liste;
}
void sauvgarderAdherent(){
    listeAdherents p=liste;
    FILE *f=fopen(adhFile,"wb");//l'ouvrture de ficher des adhrents en mode ecriture
    if (!f) perror("err");//au cas d'un erreur d'ouverture
    else{
        while (p){
            fwrite(p->infos, sizeof(donnees),1,f);//ajouter adhrent par adhrent dans le fichier
            p=p->suivant;//le passage a l'element suivant
        }
    }
    fclose(f);//fermeture de fichier des adhrents
}
donnees* rechercherAdherent(char* nom,char* prenom){
    listeAdherents p=liste;
    //parcourire de la liste jusqua trouver l'adhrent a rechercher par son nom  ou a la fin de la lise
    while (p && (strcmp(nom,p->infos->nom_adh)!=0 || strcmp(prenom,p->infos->prenom_adh)!=0))
    if (p) return p->infos;
    return NULL;
}
donnees* trouverAdherentParID(int numAdr,listeAdherents tete){
    listeAdherents p=tete;
    //rechercher l'adhrent par son numero
    while (p && p->infos->num_adh!=numAdr)
        p=p->suivant;
    if (p) return p->infos;
    return NULL;
}
void modifierAdherent(int num,listeAdherents tete,char *nom_adh,char *prenom_adh,char *adresse_email,char *adresse_personnelle){
    adherent* p=tete;
    //rechercher l'adhrent pour modifier par les nouveles valeur que en est passer en parameters
    while (p && num!=p->infos->num_adh)
        p=p->suivant;
    if (p){//si il existe on va appliquer la modification
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
    strcpy(nom,gtk_entry_get_text((GtkEntry *) app_wdgts->input_nom));
    strcpy(prenom,gtk_entry_get_text((GtkEntry *) app_wdgts->input_prenom));
    strcpy(email,gtk_entry_get_text((GtkEntry *) app_wdgts->input_email));
    strcpy(adrs,gtk_entry_get_text((GtkEntry *) app_wdgts->input_adrs));

    gtk_entry_set_text((GtkEntry *) app_wdgts->input_nom, "");
    gtk_entry_set_text((GtkEntry *) app_wdgts->input_prenom, "");
    gtk_entry_set_text((GtkEntry *) app_wdgts->input_email, "");
    gtk_entry_set_text((GtkEntry *) app_wdgts->input_adrs, "");

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

}
