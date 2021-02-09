#include "gestion_adherent.h"
#include "../page accueil/home.h"

listeAdherents liste = NULL;
app_widgets_adher *globalWidgAdr=NULL;
void on_valider_ajouter_clicked(GtkButton *button, app_widgets_adher *app_wdgts);
void on_cancel_ajouter_clicked(GtkButton *button, app_widgets_adher *app_wdgts);
void on_detail_clicked(GtkButton *button, app_widgets_adher *app_wdgts);
// Fonction pour vérifier le caractère
// est un alphabet ou non
int isChar(char c){
    if((c >= 'a' && c <= 'z')|| (c >= 'A' && c <= 'Z')) return 1;
    return 0;
}

// Fonction pour vérifier le caractère
// est un chiffre ou non
int isDigit(const char c){
    if (c >= '0' && c <= '9') return 1;
    return 0;
}

// Fonction pour vérifier l’id e-mail est
// valide ou non
int isValidEmail(char* email){
    int i;
    // Vérifiez le premier caractère
    // est un alphabet ou non
    if (isChar(email[0])==0) {

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
    if(Dot < (strlen(email) - 1))return 1;
    return 0;
}

void afficheErreurAdher(char* message, app_widgets_adher *app_wdgts){
    gtk_message_dialog_format_secondary_text((GtkMessageDialog *) app_wdgts->erreur_dialog, "%s", message);
    gtk_dialog_run(GTK_DIALOG (app_wdgts->erreur_dialog));
    gtk_widget_hide(app_wdgts->erreur_dialog);
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
donnees* creerAdherent(char *nom_adh,char *prenom_adh,char *adresse_email,char *adresse_personnelle,app_widgets_adher *appWidgetsAdher){
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
        }else{
            afficheErreurAdher("Email non valide, l'ajout a été annulé",appWidgetsAdher);
        }
    }
    else{
        afficheErreurAdher("l'un des champs est vide, l'ajout a été annulé",appWidgetsAdher);
    }
    return NULL;//si une des conditions n'est pas vérifier

}
donnees* cpyAdherent(int num,char *nom_adh,char *prenom_adh,char *adresse_email,char *adresse_personnelle,int nbre_emprunts_adh,app_widgets_adher *appWidgetsAdher){
    donnees* p=Malloc(donnees);//l'allocation mémoire pour la zone dans la quelle en mettre une copy de notre adhèrent
    if(isValidEmail(adresse_email)){//et si l'adresse email est valide
        if(isValidEmail(adresse_email)) {//et si l'adresse email est valide
            //la remplissage de toute les information concernent la copy  de l'adhérent
            p->num_adh = num;//copie le code de la  adhrent
            strcpy(p->nom_adh, nom_adh);//remplissage le nom de l'adherant
            strcpy(p->prenom_adh, prenom_adh);//remplissage du prenom de l'adherant
            strcpy(p->adresse_email, adresse_email);//remplissage de l'email de l'adherant
            strcpy(p->adresse_personnelle, adresse_personnelle);//remplissage de l'adresse  de l'adherant
            p->nbre_emprunts_adh = nbre_emprunts_adh;//copy de meme la nombre des livre qui sont empruntes par cette adherant
            return p;//retourner copye de l'adherant
        }else{
            //si l'adresse mail n'est valide
            afficheErreurAdher("Email non valide, la modification a été annulé",appWidgetsAdher);
        }
    }
    else{
        //si un des champs eat vide
        afficheErreurAdher("l'un des champs est vide, la modification a été annulé",appWidgetsAdher);
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
    //en ajoute la nouvele adh a la fin de la liste
    new->suivant=p->suivant;
    p->suivant=new;
    return tete;
}
listeAdherents chargerAdherents(){
    liste=NULL;
    donnees * data=Malloc(donnees);//l'allocation memoire d=pour les donner de l'adhrent
    FILE *f=fopen(adhFile,"rb");//ouverture de la fichier des adhrents en mode lecture
    if (!f) return NULL;//au cas d'un erreur d'ouverture de fichier
    else{
        while (fread(data, sizeof(donnees),1,f)==1){// la recuperation d'un seulle adh de fichier
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
    while (p && (strcmp(nom,p->infos->nom_adh)!=0 || strcmp(prenom,p->infos->prenom_adh)!=0)){
        p=p->suivant;
    }
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
void modifierAdherent(int num,listeAdherents tete,char *nom_adh,char *prenom_adh,char *adresse_email,char *adresse_personnelle,app_widgets_adher *appWidgetsAdher){
    adherent* p=tete;
    //rechercher l'adhrent pour modifier par les nouveles valeur que en est passer en parameters
    while (p && num!=p->infos->num_adh)
        p=p->suivant;
    if (p){//si il existe on va appliquer la modification
        p->infos=cpyAdherent(p->infos->num_adh,nom_adh,prenom_adh,adresse_email,adresse_personnelle,p->infos->nbre_emprunts_adh,appWidgetsAdher);
    }
}
void afficherAdherent(listeAdherents tete,app_widgets_adher *appWidgetsAdher){
    adherent* p=tete;
    GtkWidget *tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    GtkWidget *lnom,*lprenom,*butDetail;
    char id[4];
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(appWidgetsAdher->table_adr));//une liste nouvellement attribuée des enfants non internes du conteneur.
    //parcorire tous les enfants et retirer les reference
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    //libiration de la memoire
    g_list_free(children);
    //si la liste est vide
    if (!tete)
    {
        //l'initalisaion de tempBox
        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        //initialisie le table par le son nom par le mot vide pour indiquer que la liste est vide
        lnom=gtk_label_new("Vide");
        //ajouter ce childe a la box
        gtk_box_pack_start((GtkBox *) tempBox, lnom, 1, 1, 0);
        //ajouter la table des adhrents dans la box qui va contient la table
        gtk_box_pack_start((GtkBox *) appWidgetsAdher->table_adr, tempBox, 0, 1, 0);
    }
    while(p){
        //dans GTK
        sprintf(id, "%d", p->infos->num_adh);
        //l'initialisation de a tempox pour cree un new box
        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        //remplir le nom de l'adhrent appartir de la liste des adhrents
        lnom=gtk_label_new(p->infos->nom_adh);
        //remplir le prenom de de l'adhrent appartir de la liste des adhrents
        lprenom=gtk_label_new(p->infos->prenom_adh);
        //initialisatiion de la button qui va nous apremet d'afficher les detailles d'un adhrents
        butDetail = gtk_button_new_with_mnemonic("détails");
        //donner un identifiant a cette button
        gtk_widget_set_name(butDetail,id);
        //la creation de signal sur sette button
        g_signal_connect( GTK_WIDGET(butDetail), "clicked", G_CALLBACK( on_detail_clicked ),appWidgetsAdher);
        //positioner le champ de nom dans la table des adhrents
        gtk_box_pack_start((GtkBox *) tempBox, lnom, 1, 1, 0);
        //positioner le champ de prenom de l'adhrent dans la table des adrents
        gtk_box_pack_start((GtkBox *) tempBox, lprenom, 1, 1, 0);
        //positioner la buton des detailles  de l'adrent dans la table
        gtk_box_pack_start((GtkBox *) tempBox, butDetail, 1, 1, 0);
        //donner une taille egale a les enfants de tempBox
        gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
        //positioner cette tempBox dans box qui contient la table des adhrents
        gtk_box_pack_start((GtkBox *) appWidgetsAdher->table_adr, tempBox, 0, 1, 0);
        p=p->suivant;
    }
    //afficher tous les widget que on est cree d'une maniere recursive
    gtk_widget_show_all(appWidgetsAdher->table_adr);
}
void afficherUnAdherent(int numAdhr,app_widgets_adher *appWidgetsAdher){
    adherent* p=liste;
    GList *children, *iter;
    char id[4];
    //la creation d'une nouvelle box
    GtkWidget *tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    GtkWidget *lnom,*lprenom,*butDetail;
    //envoyer les enfants non interne de continer
    children = gtk_container_get_children(GTK_CONTAINER(appWidgetsAdher->table_adr));
    //parcorire tous les enfants et retirer les reference
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    //liberie la liste des enfants de memoire
    g_list_free(children);
    //si l'adhrent n'existe pas dans la table
    if (!liste || numAdhr==-1)
    {
        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        lnom=gtk_label_new("Non Trouvé");
        gtk_box_pack_start((GtkBox *) tempBox, lnom, 0, 1, 0);
        gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
        gtk_box_pack_start((GtkBox *) appWidgetsAdher->table_adr, tempBox, 0, 1, 0);
    }
    //chercher l'adherant
    while (p && numAdhr!=p->infos->num_adh)
        p=p->suivant;
    //si n'est null
    if (p){
        //reformer le numero de l'adherant a une chaine de caractere
        sprintf(id, "%d", p->infos->num_adh);
        //l'initialisation de la temBox
        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        //recuperer le nom de l'adherant
        lnom=gtk_label_new(p->infos->nom_adh);
        //la recuperation de prenom de l'adherant
        lprenom=gtk_label_new(p->infos->prenom_adh);
        //l'initialisation de la boutton detaille de l'adherant
        butDetail = gtk_button_new_with_mnemonic("détails");
        //on donne un identificateur a le boutton
        gtk_widget_set_name(butDetail,id);
        //l'initialisation de signale de cette boutton
        g_signal_connect( GTK_WIDGET(butDetail), "clicked", G_CALLBACK( on_detail_clicked ),appWidgetsAdher);
        //l'apositionment des ellemnt de tableau dans e box
        gtk_box_pack_start((GtkBox *) tempBox, lnom, 1, 0, 0);
        gtk_box_pack_start((GtkBox *) tempBox, lprenom, 1, 0, 0);
        gtk_box_pack_start((GtkBox *) tempBox, butDetail, 1, 1, 0);
        gtk_box_set_homogeneous((GtkBox *) tempBox, 1);//donner une taille egale a les enfants de tempBox
        //positioner cette tempBox dans box qui contiant la table des adhrents
        gtk_box_pack_start((GtkBox *) appWidgetsAdher->table_adr, tempBox, 0, 1, 0);
    }
    //affichage de tous le contenu de la table des adhrenat (son visible)
    gtk_widget_show_all(appWidgetsAdher->table_adr);
}
listeAdherents supprimerAdherent(int num,listeAdherents tete,app_widgets_adher *appWidgetsAdher){
    adherent* p=tete;
    //si la liste est vide
    if (!p)
    {
        afficheErreurAdher("Liste  est vide, la suppression a été annulé",appWidgetsAdher);
        return tete;
    }
    //si on trouve l'adrent que en traine de rechercher dans la tete de la liste
    if(num==p->infos->num_adh && p->infos->nbre_emprunts_adh==0)
    {
        tete=p->suivant;
        //liberie l'espace memoire qui alloyer cette adhrent
        free(p);
        //renitiaisie la tete de liste
        return tete;
    }else{
        afficheErreurAdher("C'est un emprunteur d'un livre, vous ne pouvez pas le supprimer",appWidgetsAdher);
        return tete;
    }
    while(p->suivant && num!=p->suivant->infos->num_adh)
    {
        p=p->suivant;
    }
    if (p->suivant && p->infos->nbre_emprunts_adh==0)
    {
        adherent *q=p->suivant;
        p->suivant=q->suivant;
        free(q);
    }else{
        if(p->infos->nbre_emprunts_adh!=0) afficheErreurAdher("C'est un emprunteur d'un livre, vous ne pouvez pas le supprimer",appWidgetsAdher);
        if(! p->suivant) afficheErreurAdher("Adherent non trouvé",appWidgetsAdher);
    }
    return tete;
}

void on_ajouter_adr_clicked(GtkButton *button,app_widgets_adher *appWidgets){
    char nom[20];
    char prenom[20];
    char email[100];
    char adrs[100];
    donnees *data;
    //on indique que on est entraine ajouter un nouvelle adherant
    appWidgets->modify_or_add=1;
    //ll'initialisation de la dialog
    appWidgets->dialog= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "dialog_adher"));
    //l'initialisation de la button anuller
    appWidgets->anuller_ajout= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "cancel_ajouter"));
    //l'initailisation de la button validie
    appWidgets->valider_ajout= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "valider_ajouter"));
    //ilialisation du signal de validation d'ajoute
    g_signal_connect(GTK_WIDGET(appWidgets->valider_ajout), "clicked", G_CALLBACK(on_valider_ajouter_clicked ), appWidgets);
    //linitialisaton du signal de la button anuller
    g_signal_connect(GTK_WIDGET(appWidgets->anuller_ajout), "clicked", G_CALLBACK(on_cancel_ajouter_clicked ), appWidgets);
    //la recuperation de l'evenement
    int resp=gtk_dialog_run(GTK_DIALOG (appWidgets->dialog));
    //gtk_widget_show(appWidgets->dialog);
    //gtk_widget_hide(appWidgets->dialog);
    switch (resp) {
        case GTK_RESPONSE_OK:
            data=NULL;
            strcpy(nom,gtk_entry_get_text((GtkEntry *) appWidgets->input_nom));
            strcpy(prenom,gtk_entry_get_text((GtkEntry *) appWidgets->input_prenom));
            strcpy(email,gtk_entry_get_text((GtkEntry *) appWidgets->input_email));
            strcpy(adrs,gtk_entry_get_text((GtkEntry *) appWidgets->input_adrs));

            gtk_entry_set_text((GtkEntry *) appWidgets->input_nom, "");
            gtk_entry_set_text((GtkEntry *) appWidgets->input_prenom, "");
            gtk_entry_set_text((GtkEntry *) appWidgets->input_email, "");
            gtk_entry_set_text((GtkEntry *) appWidgets->input_adrs, "");


            data=creerAdherent(nom,prenom,email,adrs,appWidgets);
            if(data){
                if(appWidgets->modify_or_add==1)
                    liste=insererAdherent(data,liste);
                else modifierAdherent(appWidgets->modify_id,liste,data->nom_adh,data->prenom_adh,data->adresse_email,data->adresse_personnelle,appWidgets);
            }
            afficherAdherent(liste,appWidgets);
            break;
        case GTK_RESPONSE_DELETE_EVENT:
            break;
        case GTK_RESPONSE_CLOSE:break;
    }
    gtk_widget_hide(appWidgets->dialog);
}
//charcher par nom un adherant de la liste
void chercher_par_nom(GtkButton *button,app_widgets_adher *appWidgets){
    char mot[100],nom[20],prenom[20];
    donnees *data;
    //cacher la zone de la recherche
    gtk_widget_hide(appWidgets->search);
    //cacher la button de validation de la recherche
    gtk_widget_hide(appWidgets->btn_chercher);
    //mettre en ouvre la buttun de retoure a la table initiale
    gtk_widget_show(appWidgets->btn_cancel_chercher);
    //recupuration du texte cree dans la zone de la recherche
    strcpy(mot,gtk_entry_get_text((GtkEntry *) appWidgets->search));
    //si on est entrer un texte dans la zone de recherche
    if(strlen(mot)!=0){
        //la division de texte entrer par l'utilisateur en 2 le nom et le prenom a rechercher
        char * pch;
        pch = strtok (mot," ");
        strcpy(nom,pch);
        pch = strtok (NULL, " ");
        if (pch) {
            strcpy(prenom,pch);
            //l'appele a la fonction qui permet de rechercher un adherant par son nom et son prenom
            data=rechercherAdherent(nom,prenom);
            //si il existe on va l'afficher et on desormie le traitment
            if(data) afficherUnAdherent(data->num_adh,appWidgets);
            return;
        }
    }
    afficherUnAdherent(-1,appWidgets);

}
void fermer_recherche(GtkButton *button,app_widgets_adher *appWidgets){
    //metre en oeuvre la zone de texte de recherche
    gtk_widget_show(appWidgets->search);
    //mettre en oeuvre de la button de submit de recherche
    gtk_widget_show(appWidgets->btn_chercher);
    //cacher la button qui nous apermet de retourner a la table de tous les adhrents
    gtk_widget_hide(appWidgets->btn_cancel_chercher);
    //appelle a la fonction de la fichege de la liste des adhrents
    afficherAdherent(liste, appWidgets);
}
//la fonction que en va appeller si on click sur button supprimer un adhrent
void on_supprimer_adherent(GtkButton *button, app_widgets_adher *appWidgets){
    //en cherche l'adhrent a supprimer
    donnees *data=rechercherAdherent(gtk_label_get_text(appWidgets->nom_ader_details),gtk_label_get_text((GtkLabel *) appWidgets->prenom_ader_details));
    //supprimer l'adhrents de la liste des adhrents
    liste=supprimerAdherent(data->num_adh,liste,appWidgets);
    //l'afichage de la liste des adhrents a nouveaux
    afficherAdherent(liste, appWidgets);
    //cacher la zone qui nous appermet d'afficher les dettails d'un adhrent
    gtk_widget_hide(appWidgets->box_detail);
    //et reaficher la zone qui contient la table des adhrents
    gtk_widget_show(appWidgets->box_table);
}
//l'amodification d'Un adhrent
void on_modifier_adherent(GtkButton *button, app_widgets_adher *appWidgets){
    char nom[20];
    char prenom[20];
    char email[100];
    char adrs[100];
    //en recupere les donners de l'adhrent a rechercher
    strcpy(nom,gtk_label_get_text((GtkLabel *) appWidgets->nom_ader_details));
    strcpy(prenom,gtk_label_get_text((GtkLabel *) appWidgets->prenom_ader_details));
    donnees *data=rechercherAdherent(nom,prenom);
    //en mettre le nom de l'adhrent a modifier dans l'input de nom de la formular
    gtk_entry_set_text((GtkEntry *) appWidgets->input_nom, data->nom_adh);
    //en mettre le prenom de l'adhrent a modifier dans l'input de prenom de la formular
    gtk_entry_set_text((GtkEntry *) appWidgets->input_prenom, data->prenom_adh);
    //en mettre l'email de l'adhrent a modifier dans l'input de d'email de la formular
    gtk_entry_set_text((GtkEntry *) appWidgets->input_email, data->adresse_email);
    //en mettre l'adresse personnelle de l'adhrent a modifier dans l'input de l'adrese personnelle de la formular
    gtk_entry_set_text((GtkEntry *) appWidgets->input_adrs, data->adresse_personnelle);
    appWidgets->modify_id=data->num_adh;
    appWidgets->modify_or_add=0;
    //la construction de la dialog
    appWidgets->dialog= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "dialog_adher"));
    //la construction de la button de d'anullation
    appWidgets->anuller_ajout= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "cancel_ajouter"));
    //la construction de la tutton de validation
    appWidgets->valider_ajout= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "valider_ajouter"));
    //connect le signal de validation d'ajoute
    g_signal_connect(GTK_WIDGET(appWidgets->valider_ajout), "clicked", G_CALLBACK(on_valider_ajouter_clicked ), appWidgets);
    //connect le signal d'anullation
    g_signal_connect(GTK_WIDGET(appWidgets->anuller_ajout), "clicked", G_CALLBACK(on_cancel_ajouter_clicked ), appWidgets);
    //la recuperation de l'event
    int resp=gtk_dialog_run(GTK_DIALOG (appWidgets->dialog));
    switch (resp) {
        case GTK_RESPONSE_OK:
            data=NULL;
            strcpy(nom,gtk_entry_get_text((GtkEntry *) appWidgets->input_nom));
            strcpy(prenom,gtk_entry_get_text((GtkEntry *) appWidgets->input_prenom));
            strcpy(email,gtk_entry_get_text((GtkEntry *) appWidgets->input_email));
            strcpy(adrs,gtk_entry_get_text((GtkEntry *) appWidgets->input_adrs));

            gtk_entry_set_text((GtkEntry *) appWidgets->input_nom, "");
            gtk_entry_set_text((GtkEntry *) appWidgets->input_prenom, "");
            gtk_entry_set_text((GtkEntry *) appWidgets->input_email, "");
            gtk_entry_set_text((GtkEntry *) appWidgets->input_adrs, "");


            data=creerAdherent(nom,prenom,email,adrs,appWidgets);
            if(data){
                if(appWidgets->modify_or_add==1)
                    liste=insererAdherent(data,liste);
                else modifierAdherent(appWidgets->modify_id,liste,data->nom_adh,data->prenom_adh,data->adresse_email,data->adresse_personnelle,appWidgets);
            }
            afficherAdherent(liste,appWidgets);
            gtk_widget_hide(appWidgets->box_detail);
            gtk_widget_show(appWidgets->box_table);
            gtk_dialog_response((GtkDialog *) appWidgets->dialog, GTK_RESPONSE_CLOSE);
            break;
        case GTK_RESPONSE_CLOSE:break;
        case GTK_RESPONSE_DELETE_EVENT:break;
    }
    //cacher le dialog
    gtk_widget_hide(appWidgets->dialog);
}

void on_detail_clicked(GtkButton *button, app_widgets_adher *app_wdgts){//app_widgets_home *app_wdgts){
    //la recuperation de l'id de l'adherant qui en est entraine de click sur lui
    int id=atoi(gtk_widget_get_name((GtkWidget *) button));
    char nombre_emrintes[4];
    // la recupiration des information de cette adhrent
    donnees* data=trouverAdherentParID(id,liste);
    //changer le nombre des livres emrinter par cette adhrent a une chane de caractere
    sprintf(nombre_emrintes,"%d",data->nbre_emprunts_adh);
    //l'affichege de nom de l'adhrent
    gtk_label_set_label((GtkLabel *) app_wdgts->nom_ader_details, data->nom_adh);
    //l'affichege de prenom de l\adhrent
    gtk_label_set_label((GtkLabel *) app_wdgts->prenom_ader_details, data->prenom_adh);
    //l'affichage de l'email de l'adhrent
    gtk_label_set_label((GtkLabel *) app_wdgts->email_ader_details, data->adresse_email);
    //l'afdichage de l'adresse personnel de l'adherent
    gtk_label_set_label((GtkLabel *) app_wdgts->adrs_ader_details, data->adresse_personnelle);
    //l'affichage le toutale de nombre des livres emprunter par l'adherent
    gtk_label_set_label((GtkLabel *) app_wdgts->nb_empr_adr, nombre_emrintes);
    //cacher la zone qui contient la table des adhrents
    gtk_widget_hide(app_wdgts->box_table);
    //afficher la zone qui va contient les details de l'adhrent
    gtk_widget_show(app_wdgts->box_detail);
}
//la fonction qui nous donne la possibiliter de retourner a la table de adrents
void on_retourner_clicked(GtkButton *button, app_widgets_adher *app_wdgts){
    gtk_widget_hide(app_wdgts->box_detail);
    gtk_widget_show(app_wdgts->box_table);
}
// ;la fonction qui nous appermet de valider l'ajoute de l'adh
void on_valider_ajouter_clicked(GtkButton *button, app_widgets_adher *app_wdgts){
    gtk_dialog_response((GtkDialog *) app_wdgts->dialog, GTK_RESPONSE_OK);
}
//la fonction qui nous appermet d'anuller l'ajoute
void on_cancel_ajouter_clicked(GtkButton *button, app_widgets_adher *app_wdgts){
    gtk_dialog_response((GtkDialog *) app_wdgts->dialog, GTK_RESPONSE_DELETE_EVENT);
}

void gestionAdherents(app_widgets_home *appWidgetsHome){
    GList *children, *iter;
    donnees *data=NULL;
    //liberie la liste qui va contient la liste des adher
    g_slist_free((GSList *) globalWidgAdr);
    //l'initialisation de l'aa_widgets des adhrents
    app_widgets_adher *appWidgetsAdher=g_slice_new(app_widgets_adher);
    globalWidgAdr=appWidgetsAdher;
    appWidgetsAdher->styleProvider=appWidgetsHome->styleProvider;
    //l'initaiation de la builder de l'adhrent par le builder de la page home
    appWidgetsAdher->builder=appWidgetsHome->builder;
    //l'initialisation des widget de la table
    appWidgetsAdher->table_adr= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "table_adr"));
    appWidgetsAdher->TH_table= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "TH_table"));
    appWidgetsAdher->container_adher_detail= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "adher_detail"));
    //l'initialisation de la box de detail
    appWidgetsAdher->box_detail= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "details_adr"));
    //l'initialisation de la box qui continey la table
    appWidgetsAdher->box_table= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "body_adr"));
    //l'initialisation de la button ajouter un adhrent
    appWidgetsAdher->ajouter_adr= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "ajouter_adr"));
    //l'initialisation de la button modifier adhrent
    appWidgetsAdher->modifier_adr= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "modifier_adr"));
    //l'initialisaion de la button supprimer adrent
    appWidgetsAdher->supprimer_adr= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "supprimer_adr"));
    //l'inisialisation de label qui va contient le nom de l'adhrent dans la zone de detail
    appWidgetsAdher->nom_ader_details= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "nom_ader_details"));
    //l'initialisation de label prenom
    appWidgetsAdher->prenom_ader_details= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "prenom_ader_details"));
    //l'initialisation de le label d'email
    appWidgetsAdher->email_ader_details= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "email_ader_details"));
    //l'initialisation de la zone qui contient les details de l'adhrent
    appWidgetsAdher->adrs_ader_details= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "adrs_ader_details"));
    //l'initalisation de la label de nombre des livre emprinter par l'adhrent
    appWidgetsAdher->nb_empr_adr= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "nb_empr_adher"));
    //l'initialisation de la dialog de l'adhrent
    appWidgetsAdher->dialog= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "dialog_adher"));
    //l'initialisation de l'input de nom de l'adhent a ajouter ou a modifier
    appWidgetsAdher->input_nom= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "nom_adher"));
    //l'initialisation de l'input de prenom a ajouter ou a modifier
    appWidgetsAdher->input_prenom= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "prenom_adher"));
    //linitialisation de l'input de l'email de l'adhrent a ajouter ou a modifier
    appWidgetsAdher->input_email= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "email_adher"));
    //l'initialisation de l'input de l'adresse de l'adhret a modifier ou a ajouter
    appWidgetsAdher->input_adrs= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "adrs_adher"));
    //l'initialisation de la button anuller l'ajoute
    appWidgetsAdher->anuller_ajout= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "cancel_ajouter"));
    //l'initialisation de la button valider l'ajoute
    appWidgetsAdher->valider_ajout= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "valider_ajouter"));
    //l'initialisation de la button retourner de la detaille
    appWidgetsAdher->retour_table= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "retourner_adr"));
    //l'initialid=sation de la zone de recherche d'un adhrent
    appWidgetsAdher->search= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "chercher_adr"));
    //l'initialisation de la button de validation de la recherche
    appWidgetsAdher->btn_chercher= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "btn_chercher"));
    //l'initialisation de la button de retoure a la table apres la recherche
    appWidgetsAdher->btn_cancel_chercher= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "btn_clear"));
    //'l'initialisation de la button de la suuprission d'un adhrent
    appWidgetsAdher->btn_supprimer= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "supprimer_adr"));
    //l'initialisation de a v=button de la modification d'un adhrent
    appWidgetsAdher->btn_modifier= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "modifier_adr"));
    //l'initialisation de la zone d'erreur
    appWidgetsAdher->erreur_dialog= GTK_WIDGET(gtk_builder_get_object(appWidgetsAdher->builder, "dialog_erreur"));
    //l'initialisation de signal de la boutton d'ajout
    g_signal_connect(GTK_WIDGET(appWidgetsAdher->ajouter_adr), "clicked", G_CALLBACK(on_ajouter_adr_clicked ), appWidgetsAdher);
    //l'initilalisation de signal de la validation d'ajoute
    g_signal_connect(GTK_WIDGET(appWidgetsAdher->valider_ajout), "clicked", G_CALLBACK(on_valider_ajouter_clicked ), appWidgetsAdher);
    //l'initilalisation de signal d'anulation d'ajoute
    g_signal_connect(GTK_WIDGET(appWidgetsAdher->anuller_ajout), "clicked", G_CALLBACK(on_cancel_ajouter_clicked ), appWidgetsAdher);
    //l'initialisation de signal de retour a la table des adhrents
    g_signal_connect(GTK_WIDGET(appWidgetsAdher->retour_table), "clicked", G_CALLBACK(on_retourner_clicked ), appWidgetsAdher);
    //l'initialisatin de signal de la button de recherche
    g_signal_connect(GTK_WIDGET(appWidgetsAdher->btn_chercher), "clicked", G_CALLBACK(chercher_par_nom ), appWidgetsAdher);
    //l'initialisation de signal de la button retoune concernene apres le resulta de la recherche
    g_signal_connect(GTK_WIDGET(appWidgetsAdher->btn_cancel_chercher), "clicked", G_CALLBACK(fermer_recherche ), appWidgetsAdher);
    //l'initialisation de signal de la button de la modification d'un adhrent
    g_signal_connect(GTK_WIDGET(appWidgetsAdher->btn_modifier), "clicked", G_CALLBACK(on_modifier_adherent), appWidgetsAdher);
    //l'initialisation de la button de la supprision d'un adhrent
    g_signal_connect(GTK_WIDGET(appWidgetsAdher->btn_supprimer), "clicked", G_CALLBACK(on_supprimer_adherent), appWidgetsAdher);
    //les style css
    gtk_style_context_add_provider(gtk_widget_get_style_context(appWidgetsAdher->ajouter_adr), GTK_STYLE_PROVIDER(appWidgetsAdher->styleProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(gtk_widget_get_style_context(appWidgetsAdher->TH_table), GTK_STYLE_PROVIDER(appWidgetsAdher->styleProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(gtk_widget_get_style_context(appWidgetsAdher->container_adher_detail), GTK_STYLE_PROVIDER(appWidgetsAdher->styleProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(gtk_widget_get_style_context(appWidgetsAdher->modifier_adr), GTK_STYLE_PROVIDER(appWidgetsAdher->styleProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(gtk_widget_get_style_context(appWidgetsAdher->supprimer_adr), GTK_STYLE_PROVIDER(appWidgetsAdher->styleProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(gtk_widget_get_style_context(appWidgetsAdher->retour_table), GTK_STYLE_PROVIDER(appWidgetsAdher->styleProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(gtk_widget_get_style_context(appWidgetsAdher->erreur_dialog), GTK_STYLE_PROVIDER(appWidgetsAdher->styleProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    /*liste=insererAdherent(creerAdherent("DAHMAN","iliass","iliass@gmail.com","taounate",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("EZ-ZAIM","adnane","adnane@gmail.com","Fes",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("Hamdouni","ahmed","ahmed@gmail.com","Fes",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("Sadiki","nouha","nouha@gmail.com","Casa",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("samid","karima","karima@gmail.com","Tanger",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("Ali","mohamed","mohamed@gmail.com","Meknes",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("Khasim","said","said@gmail.com","Taza",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("Omari","omar","omar@gmail.com","Ifrane",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("kassimi","rachida","rachida@gmail.com","Safrou",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("salami","salma","salma@gmail.com","Rabat",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("khaldoni","khalid","khalid@gmail.com","Fes",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("Samadi","ahmed","ahmed@gmail.com","Taounate",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("Jarmouni","saad","saad@gmail.com","Marrakech",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("Sabahi","sabah","sabah@gmail.com","Ouajda",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("Jaddi","karima","karima@gmail.com","Titouane",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("kassimi","chakir","chakir@gmail.com","Kenitre",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("Ouazzani","mouna","mouna@gmail.com","casa",appWidgetsAdher),liste);
    liste=insererAdherent(creerAdherent("Assri","zakaria","zakaria@gmail.com","casa",appWidgetsAdher),liste);*/
    //l'affichege de la liste des adhrent
    afficherAdherent(liste, appWidgetsAdher);
    gtk_window_set_title((GtkWindow *) appWidgetsAdher->dialog, "Ajouter ou modifier");
    //connecter les signals
    gtk_builder_connect_signals(appWidgetsAdher->builder, appWidgetsAdher);
    //cacher la button de retourne apres la recherche
    gtk_widget_hide(appWidgetsAdher->btn_cancel_chercher);
    //cacher le box qui contient les dettail d'un adhrent
    gtk_widget_hide(appWidgetsAdher->box_detail);
}
