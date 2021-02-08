//
// Created by adnane on 1/7/21.
//

#include"gestion_livre.h"
listeLivre *liteLivre=NULL;
app_widgets_livres *globalWidgLivre=NULL;
char CATEGS[4][15]={"poétique","narratif","argumentatif","théâtre"};

void afficheErreurLivres(char* message, app_widgets_livres *app_wdgts){
    gtk_message_dialog_format_secondary_text((GtkMessageDialog *) app_wdgts->erreur_dialog, "%s", message);
    gtk_dialog_run(GTK_DIALOG (app_wdgts->erreur_dialog));
    gtk_widget_hide(app_wdgts->erreur_dialog);
}
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
    return count-1;//retourner l'indice
}
livre *saisieLivre(char *titre,int cat,char *nom,char *pre){
    livre* l=NULL;
    if (strlen(titre)!=0 && strlen(nom)!=0 && strlen(pre)!=0) {//si tous les information son bien saisie
        l = (livre *) malloc(sizeof(livre));
        // ta ndir hna teste wax khawyan wla la
        l->num_liv = createUniqueCodeLivre();
        strcpy(l->titre_livre, titre);
        l->ctegorie = cat;
        //strcpy(l->categ_liv,cat);
        strcpy(l->auteur_liv.nom_aut, nom);
        strcpy(l->auteur_liv.prenom_aut, pre);
        (l->emprunteur_liv) = -1;
    }else{
        afficheErreurLivres("l'un des champs est vide, l'ajout a été annulé",globalWidgLivre);
    }
    return l;
}
//copyer les informatons d'un livre dans i=un autre livre
livre *cpyLivre(int *num,char *titre,int cat,char *nom,char *pre,int emprunteur_livre){
    livre* l=(livre*)malloc(sizeof(livre));//l'alocation memoire pour la copie de livre
    l->num_liv=(*num);//en lui passse le numero de livre
    strcpy(l->titre_livre,titre);//copie le titre
    l->ctegorie= cat;//copie la categorie
    //strcpy(l->categ_liv,cat);
    strcpy(l->auteur_liv.nom_aut, nom);//copie le nom de l'auteur
    strcpy(l->auteur_liv.prenom_aut,pre);//copie le prenom de l'auteur de livre
    (l->emprunteur_liv)=emprunteur_livre;//copie le id de l'emprinteur de livre
    return l;//retourner la copye de livre
}
//cette fonction nous apremet de modifier un livre
listeLivre * modifierLivre(listeLivre *l,char *titre,char* newTitre,int cat,auteur *aut){

    listeLivre *temp=NULL;
    temp=l;
    //en parcorie la liste pour chercher le livre a modifier
    while(temp != NULL)
    {
        //si en trouve le livre
        if (strcmp(temp->info->titre_livre,titre)==0){
            strcpy(temp->info->titre_livre,newTitre);//la modification de titre de livre
            temp->info->ctegorie= cat;//la modification de la categorie de livre
            strcpy(temp->info->auteur_liv.nom_aut, aut->nom_aut);// la modification de nom de l'auteur de livre
            strcpy(temp->info->auteur_liv.prenom_aut,aut->prenom_aut);// la modification de prenom de l'auteur de livre
        }
        temp=temp->suiv;
    }

    return l;//retourne le livre modifier
}
// la fonction qui nous appermet de cree un nouveau livre
listeLivre *creerLivre(livre *elm) {
    listeLivre *nouv =(listeLivre*)malloc(sizeof(listeLivre));//la location memoire pour cette nouvelle livre a cree
    nouv->info=elm;//l'affictation de les information de la nouvelle livre a creee
    nouv->suiv=NULL;//le pointeur suiv de la nouvelle element de a liste on va linitialisie par le null
    return nouv;//retourner la nouvelle ellemnt que on est cree
}
//cette fonction nous apermet d'inserer a l'entete de la liste des livres
listeLivre *insererListeEnTete(listeLivre *tete,livre *elm){
    listeLivre *nouveau=creerLivre(elm);// la location memoire de la nouvelle element a cree
    nouveau->suiv=tete;//le suiv de la nouvelle ellemt que en est cree est sera pointer sur l'entete de la liste
    tete = nouveau ;//modifier l'entete de la liste avec la nouvelle element qui on est cree
    return tete ;// retourne l'entete de la liste
}
// cette fonction nous appermet d'inserer en ordre des numero les livres
listeLivre *insererEnOrdre(listeLivre *l, livre *elm){
    listeLivre* nv =(listeLivre*)malloc(sizeof(listeLivre));
    nv->info=elm;
    listeLivre *temp = l;
    //si le num de la nouvelle ellement est <= a l'lemet courant de la liste
    if((temp == NULL) || temp->info->num_liv >= elm->num_liv)
    {
        nv->suiv=l;
        l=nv;
    }
    else{
        while((temp->suiv != NULL) && temp->info->num_liv < elm->num_liv )
            temp = temp->suiv;
// l'ajoute de la nouvelle element soit a la fin de la liste soit au milieu de la liste
        nv->suiv = temp->suiv;
        temp->suiv = nv;
    }
    return l;
}
// trier la liste des livre deja non trier
listeLivre *ordonerListeLivre(listeLivre *tete){
    listeLivre *temp,*orListe=NULL;
    temp=tete;
    while(temp!=NULL){
        orListe=insererEnOrdre(orListe,temp->info);
        temp=temp->suiv;
    }
    return orListe;
}

// la suprision d'un livre de la liste
listeLivre *supprimerLivre(int nombre,char* titre){
    listeLivre *temp=NULL;
    //si le livre a suprimer n'est pas emprunte on va suprimer ce livre a rechercher
    while(liteLivre && strcmp(liteLivre->info->titre_livre,titre)==0 && nombre>0 && liteLivre->info->emprunteur_liv==-1){
        liteLivre=liteLivre->suiv;
        nombre --;
    }
    if (!liteLivre) return NULL;
    temp=liteLivre;
    //si le mombre a suprimer est non null et on est pas encoure ariver a la fin de la liste
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
//compteur d'un livre qui nous appermet de nous donnee le nombre des exemplaire emprunter et le nombre des exemplaire desponible
void compteurLivres(char *titre,int *emprunte,int *dispo,listeLivre *l){
    listeLivre *temp;
    temp=l;
    while(temp!=NULL){
        if(strcmp(temp->info->titre_livre,titre)==0){
            //si ce livre n'est pas emprunter on va incrumenter le compteur des desponible
            if (temp->info->emprunteur_liv==-1) *(dispo)+=1;
                //sinon on va incrumenter le compteur des emprunteurs
            else *(emprunte)+=1;
        }
        temp=temp->suiv;
    }
}
// cette fonction nous appermet de chercher un livre par son titre
livre *RechercheLivre(char *titre,listeLivre *l){
    listeLivre *temp;
    temp=l;
    //parcorir la liste des livre
    while(temp!=NULL){
        //si on troub=ve le livre a rechercher en va retourner c'est informations
        if(strcmp(temp->info->titre_livre,titre)==0){
            return temp->info;
        }
        temp=temp->suiv;
    }
    return NULL;
}
//retourner la liste des livres qui'ont la meme titre et qui sont desponible
listeLivre *trouverPlageLivre(char* titre,listeLivre *livres){
    listeLivre* tab=NULL;
    int index=0;
    listeLivre *temp;
    temp=livres;
    //parcorire la liste des livres
    while(temp!=NULL){
        //si n'est pas emprunte et avec le meme tetre que en est entraine de chercher
        if(strcmp(temp->info->titre_livre,titre)==0 && temp->info->emprunteur_liv!=-1){
            tab=insererEnOrdre(tab,temp->info);
        }
        temp=temp->suiv;
    }
    return tab;
}

//l'affichag des information d'un livre dans le console
void afficherLivre(livre* l){
    printf("le numero de livre est :%d\n",l->num_liv);
    printf("le titre de livre est :%s\n",l->titre_livre);
    printf("la categorie de livre est :%d\n",l->ctegorie);
    printf("le numero de l'emprunteur de ce livre :%d\n",l->emprunteur_liv);
    printf("les information de l'auteur :\n");
    printf("le nom :%s\n",l->auteur_liv.nom_aut);
    printf("le prenom :%s\n",l->auteur_liv.prenom_aut);
}
//si on click sur le boutton detaille d'un livre dans la table on va afficher les information sur cette livre
void on_livre_detail_clicked(GtkButton *button, app_widgets_livres *app_wdgts){//app_widgets_home *app_wdgts){
    char titre[30];
    int nbDispo=0,nbEmp=0;
    char CnbDispo[4],CnbEmp[4];
    strcpy(titre,gtk_widget_get_name((GtkWidget *) button));
    char nombre_empruntes[4];
    livre* donneesLivre=RechercheLivre(titre,liteLivre);// en va chercher d'abord ce livre et recupirer c'est informatios
    compteurLivres(donneesLivre->titre_livre,&nbEmp,&nbDispo,liteLivre);//compter le nombre des despo et des livre emprinter
    sprintf(CnbDispo,"%d",nbDispo);//caste de nombre des disponible a une chaine ce caractere
    sprintf(CnbEmp,"%d",nbEmp);//caste de nombre des emprunt a une chaine ce caractere
    gtk_label_set_label((GtkLabel *) app_wdgts->titre_liv_details, donneesLivre->titre_livre);//mettre le titre de livre dans le abel de titre
    gtk_label_set_label((GtkLabel *) app_wdgts->catregorie_liv_details, CATEGS[donneesLivre->ctegorie]);//mettre la categorie de livre dans le abel de la categorie
    gtk_label_set_label((GtkLabel *) app_wdgts->aut_nom_liv_details, donneesLivre->auteur_liv.nom_aut); //l'affichage de nom de l'auteur de livre
    gtk_label_set_label((GtkLabel *) app_wdgts->nb_empr_liv, CnbEmp);//affichage de nombre des exemplaire de livre emprunter
    gtk_label_set_label((GtkLabel *) app_wdgts->nb_desp_liv, CnbDispo);//affichage de nombre des exemplaire de livre desponible
    gtk_widget_hide(app_wdgts->container_table);//cacher le widget qui contient la tabe des livres
    gtk_widget_show(app_wdgts->box_detail);//afficher la zone qui va contient les detaille du livre
}
//l'affichage de toute la liste des livres
void afichageListeLivre(listeLivre *tete, app_widgets_livres *appWidgetsLivre){
    listeLivre *temp;
    GtkWidget *tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);// l'inialisation du tempBox
    GtkWidget *ltitre,*lcategorie,*butDetail;
    GList *children, *iter;
    char id[4];
    temp=listeUniqueLivre(tete);//recupiration d'un seul exemplaire des livre
    temp=listeOrdoneCteg(temp);// ordoner la liste par categorie
    //temp=tete;
    children = gtk_container_get_children(GTK_CONTAINER(appWidgetsLivre->box_table));//la recuperation des enfants de la container
    //desactiver tous les enfants de la container
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);//liberie la liste des enfants
    ltitre=gtk_label_new("Titre");// la creation d'un label pour le titre
    lcategorie=gtk_label_new("Categorie");//la creation d'un label pour la categorie
    gtk_box_pack_start((GtkBox *) tempBox, ltitre, 1, 0, 0);//la positionemeent de titre dans la table
    gtk_box_pack_start((GtkBox *) tempBox, lcategorie, 1, 0, 0);//la positionment de la categorie de livre dans la table
    gtk_box_pack_start((GtkBox *) tempBox, gtk_label_new("Actions"), 1, 1, 0);//la positionment de les actions dans la table
    gtk_box_set_homogeneous((GtkBox *) tempBox, 1);//to specify whether or not all children of the GtkBox are forced to get the same amount of space.
    gtk_box_pack_start((GtkBox *) appWidgetsLivre->box_table, tempBox, 0, 1, 0);//L’enfant est emballé après tout autre enfant emballé en référence au début de la box.
    //si la liste des lvres est vide
    if(tete==NULL){tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        ltitre=gtk_label_new("Vide");//remplaire par le mot vide
        gtk_box_pack_start((GtkBox *) tempBox, ltitre, 1, 1, 0);//positioner ce mot dans la table
        gtk_box_pack_start((GtkBox *) appWidgetsLivre->box_table, tempBox, 0, 1, 0);//posisione la tempbox dan la box de la table des livres
        printf("la liste est vide !!");
    }
    else{
        //parcorir la liste des livre
        while(temp!=NULL){
            afficherLivre(temp->info);//l'affichage dans le console

            tempBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);//l'initialisation de la tempbox
            ltitre = gtk_label_new(temp->info->titre_livre);//l'initialisation de label de titre avec le titre de l'element cournt de la liste des livres
            lcategorie = gtk_label_new(CATEGS[temp->info->ctegorie]);//l'initialisation de label de la categorie avec la categorie de l'element cournt de la liste des livres
            butDetail = gtk_button_new_with_mnemonic("détails");
            gtk_widget_set_name(butDetail, temp->info->titre_livre);//l'initialisation du bouton avec le titre de l'element cournt de la liste des livres
            g_signal_connect(GTK_WIDGET(butDetail), "clicked", G_CALLBACK(on_livre_detail_clicked),
                             appWidgetsLivre);//l'initialisation de signa de la bouton si on click sur le boutton cree
            gtk_box_pack_start((GtkBox *) tempBox, ltitre, 1, 0, 0);//Ajoute widget à la tembox, emballé avec référence au début de la tembox
            gtk_box_pack_start((GtkBox *) tempBox, lcategorie, 1, 0, 0);//Ajoute widget à la tembox, emballé avec référence au début de la tembox
            gtk_box_pack_start((GtkBox *) tempBox, butDetail, 1, 1, 0);//Ajoute widget à la tembox, emballé avec référence au début de la tembox
            gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
            gtk_box_pack_start((GtkBox *) appWidgetsLivre->box_table, tempBox, 0, 1, 0);//Ajoute widget à la tembox, emballé avec référence au début de la tembox
            temp = temp->suiv;
        }
    }
    gtk_widget_show_all(appWidgetsLivre->box_table);//afficher le widget qui contient la table
}
//affichage des dettaile d'un livre
void afficherUnLive(int num,app_widgets_livres *appWidgetsLivre){
    listeLivre * temp=liteLivre;
    GList *children, *iter;
    char id[4];
    GtkWidget *tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);//l'initia;lisation de la widget
    GtkWidget *ltitre,*lcategorie,*butDetail;
    children = gtk_container_get_children(GTK_CONTAINER(appWidgetsLivre->box_table));//la recuperation des enfants de la container
    //parcorire tous les children pour desormie
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);//liberie les pointeur qui point sur ses children
    ltitre=gtk_label_new("Titre");// creation de lable d'entete de la table
    lcategorie=gtk_label_new("Categorie");// creation de lable d'entete de la table
    gtk_box_pack_start((GtkBox *) tempBox, ltitre, 1, 0, 0);//Ajoute widget à la tembox, emballé avec référence au début de la tembox
    gtk_box_pack_start((GtkBox *) tempBox, lcategorie, 1, 0, 0);//Ajoute widget à la tembox, emballé avec référence au début de la tembox
    gtk_box_pack_start((GtkBox *) tempBox, gtk_label_new("Actions"), 1, 1, 0);//Ajoute widget à la tembox, emballé avec référence au début de la tembox
    gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
    gtk_box_pack_start((GtkBox *) appWidgetsLivre->box_table, tempBox, 0, 1, 0);//Ajoute widget à la tembox, emballé avec référence au début de la tembox
//    afficher les livres qui sont pas emprunter
    if (!liteLivre || num ==-1)
    {
        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        ltitre=gtk_label_new("Non Trouvé");
        gtk_box_pack_start((GtkBox *) tempBox, ltitre, 0, 1, 0);
        gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
        gtk_box_pack_start((GtkBox *) appWidgetsLivre->box_table, tempBox, 0, 1, 0);
        printf("Liste est vide\n");
    }
    //le parcoure de la liste des livres
    while (temp && num!=temp->info->num_liv)
        temp=temp->suiv;
    if (temp){
        afficherLivre(temp->info);//l'affichage dans le console

        tempBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        ltitre=gtk_label_new(temp->info->titre_livre);//initialisie le lable titre de livre
        lcategorie=gtk_label_new(CATEGS[temp->info->ctegorie]);//initialisie le lable categorie de livre
        butDetail = gtk_button_new_with_mnemonic("détails");//initialisie du bouton detaille de livre
        gtk_widget_set_name(butDetail,temp->info->titre_livre);//mettre un identifient du boutton cree
        g_signal_connect( GTK_WIDGET(butDetail), "clicked", G_CALLBACK( on_livre_detail_clicked ),appWidgetsLivre);//initialisie e signale de boutton detaille
        gtk_box_pack_start((GtkBox *) tempBox, ltitre, 1, 0, 0);//Ajoute widget à la tembox, emballé avec référence au début de la tembox
        gtk_box_pack_start((GtkBox *) tempBox, lcategorie, 1, 0, 0);//Ajoute widget à la tembox, emballé avec référence au début de la tembox
        gtk_box_pack_start((GtkBox *) tempBox, butDetail, 1, 1, 0);//Ajoute widget à la tembox, emballé avec référence au début de la tembox
        gtk_box_set_homogeneous((GtkBox *) tempBox, 1);
        gtk_box_pack_start((GtkBox *) appWidgetsLivre->box_table, tempBox, 0, 1, 0);//Ajoute widget à la tembox, emballé avec référence au début de la tembox
    }
    gtk_widget_show_all(appWidgetsLivre->box_table);//afficher tous les widget
}
//cette fonction ne appermet de retourner tous les livres de maniere non deplicable
listeLivre *listeUniqueLivre(listeLivre *liste){
    listeLivre *ptrLivre=liste;//on l'initialise avec la liste de tous les livre
    listeLivre *listeFinale=NULL;
    char titre[50];
    if (ptrLivre) {
        strcpy(titre, ptrLivre->info->titre_livre);
        listeFinale=insererEnOrdre(listeFinale,ptrLivre->info);//initialisation  de laliste avec une liste trie
        // parcours tous la liste des livres
        while (ptrLivre){
            if (strcmp(titre,ptrLivre->info->titre_livre)!=0){
                strcpy(titre, ptrLivre->info->titre_livre);
                listeFinale=insererEnOrdre(listeFinale,ptrLivre->info);//en insire dans la liste que on est entraine de cree qui contient bien sur les informations qui sont pas repeter
            }
            ptrLivre = ptrLivre->suiv;
        }
    }
    return listeFinale;//retourner la liste finale
}
//prepare une liste ordoner par categorie
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
//la liste des livres empruntes
listeLivre *listeEmprinter(listeLivre *liste){
    listeLivre *ptrLivre=liste;
    listeLivre *listeFinale=NULL;
    while (ptrLivre){
        //si l'emprunteur est defirent de -1 c-a-d  emprunter par un adherant
        if (ptrLivre->info->emprunteur_liv != -1){
            listeFinale=insererEnOrdre(listeFinale,ptrLivre->info);
        }
        ptrLivre = ptrLivre->suiv;
    }
    return listeFinale;//retourner la liste finale
}
//la liste des livres desponible dans le stock
listeLivre *listeDisponible(listeLivre* liste){
    listeLivre *ptrLivre=liste;
    listeLivre *listeFinale=NULL;
    while (ptrLivre){
        if (ptrLivre->info->emprunteur_liv == -1){
            listeFinale=insererEnOrdre(listeFinale,ptrLivre->info);//le remplissage de la liste finale c-a-d la liste des livres qui sont pas emprunter
        }
        ptrLivre = ptrLivre->suiv;
    }
    return listeFinale;//retourner la lister finale des livres desponible
}
//charger la liste de tous les livres que on est dans la base de donnees
listeLivre* chargerLivres(){
    FILE *f;
    livre* l=Malloc(livre);
    int i;
    liteLivre=NULL;
    f=fopen(livreFile,"rb");//ouvrir le ficher qui contient tous les livres que on est dans la base de donnee
    //si on est un  erreur d'ouverture de fichier
    if(f==NULL)printf("erreur d'ouvertur\n");
    else{
        //le parcoure de tous les livres qui sont engestrer dans le fichier pour remplire la liste des livres
        while (fread(l, sizeof(livre),1,f)==1){// la recuperation d'un seulle adh de fichier
            liteLivre=insererEnOrdre(liteLivre, l);//le remplisage des la liste des livres
            l=Malloc(livre);
        }
        fclose(f);//la fermeture de ficher des livres
    }
    return liteLivre;//retourner la liste des livres

}
//engestrement la liste des livres dans le fichier de la base de donnee
void sauvgarderLivres(){
    listeLivre *liste=liteLivre;
    FILE *f=fopen(livreFile,"wb");
    if (!f) perror("err");//si il ya un erreur d'ouverture
    else{
        //parcorir tous la liste des livres pour mettre il dans le fichier
        while (liste){
            fwrite(liste->info, sizeof(livre),1,f);
            liste=liste->suiv;
        }
    }
    fclose(f);//la fermeture de fichier
}
//si on click sur le boutton de validation d'ajout d'un nouvelle livres
void on_valider_ajouter_livre_clicked(GtkButton *button, app_widgets_livres *app_wdgts){
    gtk_dialog_response((GtkDialog *) app_wdgts->dialog_livre, GTK_RESPONSE_OK);

}
//si on anulle l'ajoute de l'aivre
void on_cancel_ajouter_livre_clicked(GtkButton *button, app_widgets_livres *app_wdgts){
    gtk_dialog_response((GtkDialog *) app_wdgts->dialog_livre, GTK_RESPONSE_DELETE_EVENT);
}
//si on click sur le botton ajouter un livre
void on_ajoute_livre_clicked(GtkButton *button,app_widgets_livres *appWidgets){
    int categ;
    auteur aut;
    int nb_exempliare;
    char id[10];
    int i;
    char titre[50];
    int nbDispo=0,nbEmpr=0;
    char CnbDispo[4];
    char categorie[4];
    livre* data=NULL;
    appWidgets->modify_or_add=1;// indiqui que c'est un ajoute
    appWidgets->dialog_livre= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "dialog_livre"));//appelle a la dialog
    appWidgets->valider_ajouter= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "valider_ajouter2"));//appele les botton de validation
    appWidgets->cancel_ajouter= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "cancel_ajouter2"));//l'appelle a la botton d'anullation
    g_signal_connect(GTK_WIDGET(appWidgets->valider_ajouter), "clicked", G_CALLBACK(on_valider_ajouter_livre_clicked ), appWidgets);//l'initialisation de signals du botton valider
    g_signal_connect(GTK_WIDGET(appWidgets->cancel_ajouter), "clicked", G_CALLBACK(on_cancel_ajouter_livre_clicked ), appWidgets);//l'initialisation de signals du botton d'anulation

    int resp=gtk_dialog_run(GTK_DIALOG (appWidgets->dialog_livre));//mettre enoeuvre le dialog
    switch (resp) {
        case GTK_RESPONSE_OK:
            strcpy(id,gtk_combo_box_get_active_id((GtkComboBox *) appWidgets->input_categorie_liv));//la recuperation de la categorie choisie
            strcpy(titre,gtk_entry_get_text((GtkEntry *) appWidgets->input_titre_liv));//la recuperation de titre cree
            categ=atoi(id);//la tronsformation de l'indice de la categorie a un entier
            strcpy(aut.nom_aut,gtk_entry_get_text((GtkEntry *) appWidgets->input_nom_autr));//la recupiration de nom de l'auteur que on est cree
            strcpy(aut.prenom_aut,gtk_entry_get_text((GtkEntry *) appWidgets->input_prenom_atr));//la recueration de prenom de l'auteur que on est cree
            strcpy(id,gtk_entry_get_text((GtkEntry *) appWidgets->nbr_exemplaire));//la recuperation de le nombre des exemplaire que on est entrer
            //vider les input
            gtk_entry_set_text((GtkEntry *) appWidgets->input_titre_liv, "");//vider la zone d'input de titre
            gtk_combo_box_set_active((GtkComboBox *) appWidgets->input_categorie_liv,0);//vider la zone d'input de categorie
            gtk_entry_set_text((GtkEntry *) appWidgets->input_nom_autr, "");//vider la zone d'input de nom de l'auteur
            gtk_entry_set_text((GtkEntry *) appWidgets->input_prenom_atr, "");//vider la zone d'input de prenom de l'auteur
            gtk_entry_set_text((GtkEntry *) appWidgets->nbr_exemplaire, "");//vider la zone d'input de nombre des exemplaire
            if(strlen(id) !=0){
                nb_exempliare=atoi(id);//la trensformation a un entier
                //la creation d'un livre avec les information que on est entrer dans le formulaire
                data = saisieLivre(titre, categ, aut.nom_aut, aut.prenom_aut);
                if (data) {
                    //on est entraine d'ajouter un nouvelle livre
                    //l'insertion de livre que on est cree par nombre des exemplaire que on est entrer
                    for (i = 0; i < nb_exempliare; ++i) {
                        liteLivre = insererEnOrdre(liteLivre, data);
                    }
                }
            }else{
                afficheErreurLivres("l'un des champs est vide, l'ajout a été annulé",globalWidgLivre);
            }
            afichageListeLivre(liteLivre, appWidgets);//l'apille a la fonction d'affichage de la liste des livres
            break;
        case GTK_RESPONSE_DELETE_EVENT:break;
        case GTK_RESPONSE_CLOSE:break;
    }
    gtk_widget_hide(appWidgets->dialog_livre);//cacher le dialog
}
//si on click sur le boutton retourner dans la partie qui affiche les detaille
void on_retourn_clicked(GtkButton *button,app_widgets_livres *appWidgets){
    gtk_widget_hide(appWidgets->box_detail);//cacher les zone qui va contient les information d'un livre
    gtk_widget_show(appWidgets->container_table);//afficher la table des livres
}
//si on click sur le boutton supprimer un livre
void on_supprime_livre_clicked(GtkButton *button,app_widgets_livres *appWidgets){
    int nbDispo=0,nbEmpr=0;
    char titre[50];
    livre* data=NULL;
    strcpy(titre,gtk_label_get_text((GtkLabel *) appWidgets->titre_liv_details));//la recuperation de titre de livre
    data=RechercheLivre(titre,liteLivre);//recupirer le livre
    compteurLivres(data->titre_livre,&nbEmpr,&nbDispo,liteLivre);//la recuperation de nombre des livre desponible et empruntes
    supprimerLivre(nbDispo,titre);//la supprision de livre
    afichageListeLivre(liteLivre,appWidgets);//la reaffichage de liste de livre
    gtk_widget_hide(appWidgets->box_detail);//cacher la zone des detaille
    gtk_widget_show(appWidgets->container_table);//afficher la zone qui contient la table des livres
}
//si on click sur le boutton modufier
void on_modifier_livre_clicked(GtkButton *button,app_widgets_livres *appWidgets){
    int categ;
    auteur aut;
    int nb_exempliare;
    char id[10];
    int i;
    char titre[50];
    int nbDispo=0,nbEmpr=0;
    char CnbDispo[4];
    char categorie[4];
    livre* data=NULL;
    appWidgets->modify_or_add=0;//on indique que c'est une modification
    strcpy(titre,gtk_label_get_text((GtkLabel *) appWidgets->titre_liv_details));//la recuperation de titre de livre
    data=RechercheLivre(titre,liteLivre);//rechercher ce livre
    appWidgets->dialog_livre= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "dialog_livre"));//appelle a dialog
    appWidgets->valider_ajouter= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "valider_ajouter2"));//l'appelle a la boutton de validation
    appWidgets->cancel_ajouter= GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "cancel_ajouter2"));//l'appelle a la boutton d'annulation
    g_signal_connect(GTK_WIDGET(appWidgets->valider_ajouter), "clicked", G_CALLBACK(on_valider_ajouter_livre_clicked ), appWidgets);//l'initialisation de signal de validation
    g_signal_connect(GTK_WIDGET(appWidgets->cancel_ajouter), "clicked", G_CALLBACK(on_cancel_ajouter_livre_clicked ), appWidgets);//linitialisation de signal d'anullation

    compteurLivres(data->titre_livre,&nbEmpr,&nbDispo,liteLivre);//la recuperation de nombre des exemplaire desponble et emprunter
    sprintf(CnbDispo,"%d",nbDispo);//le formatage de nombre des despo a une chaine de caractere
    sprintf(categorie,"%d",data->ctegorie);//le formatage de catregorie recuperer a une chaine de caractere
    strcpy(appWidgets->modify_titre,titre);//mettre le trer entrer dans un variable
    gtk_entry_set_text((GtkEntry *) appWidgets->input_titre_liv, data->titre_livre);//mettre le titrre dans la zone d'input de titre
    gtk_combo_box_set_active_id((GtkComboBox *) appWidgets->input_categorie_liv,categorie);//mettre le categorie dans la zoone de categorie de la formule
    gtk_entry_set_text((GtkEntry *) appWidgets->input_nom_autr, data->auteur_liv.nom_aut);//mettre le nom de l'auteur dans l'input de nom de l'auteur
    gtk_entry_set_text((GtkEntry *) appWidgets->input_prenom_atr, data->auteur_liv.prenom_aut);//mettre le prenom de l'auteur dans l'input de prenom de la dialog
    gtk_entry_set_text((GtkEntry *) appWidgets->nbr_exemplaire, CnbDispo);//remplaire le nombre des exemplaire par le nombre des despo

    int resp=gtk_dialog_run(GTK_DIALOG (appWidgets->dialog_livre));//demarer le dialog
    switch (resp) {
        case GTK_RESPONSE_OK:
            strcpy(id,gtk_combo_box_get_active_id((GtkComboBox *) appWidgets->input_categorie_liv));//la recuperation de la categorie choisie
            strcpy(titre,gtk_entry_get_text((GtkEntry *) appWidgets->input_titre_liv));//la recuperation de titre cree
            categ=atoi(id);//la tronsformation de l'indice de la categorie a un entier
            strcpy(aut.nom_aut,gtk_entry_get_text((GtkEntry *) appWidgets->input_nom_autr));//la recupiration de nom de l'auteur que on est cree
            strcpy(aut.prenom_aut,gtk_entry_get_text((GtkEntry *) appWidgets->input_prenom_atr));//la recueration de prenom de l'auteur que on est cree
            strcpy(id,gtk_entry_get_text((GtkEntry *) appWidgets->nbr_exemplaire));//la recuperation de le nombre des exemplaire que on est entrer
            //vider les input
            gtk_entry_set_text((GtkEntry *) appWidgets->input_titre_liv, "");//vider la zone d'input de titre
            gtk_combo_box_set_active((GtkComboBox *) appWidgets->input_categorie_liv,0);//vider la zone d'input de categorie
            gtk_entry_set_text((GtkEntry *) appWidgets->input_nom_autr, "");//vider la zone d'input de nom de l'auteur
            gtk_entry_set_text((GtkEntry *) appWidgets->input_prenom_atr, "");//vider la zone d'input de prenom de l'auteur
            gtk_entry_set_text((GtkEntry *) appWidgets->nbr_exemplaire, "");//vider la zone d'input de nombre des exemplaire
            if(strlen(id) !=0){
                nb_exempliare=atoi(id);//la trensformation a un entier
                //on est en traine de modofier les information d'un livre
                //si le nomb des livres despo superieur ou nombre que on est entrais dans l'input
                if (nbDispo > nb_exempliare)
                    supprimerLivre(nbDispo - nb_exempliare, appWidgets->modify_titre);
                else if (nbDispo < nb_exempliare) {
                    for (i = 0; i < nb_exempliare - nbDispo; ++i) {
                        liteLivre = insererEnOrdre(liteLivre, data);//inserer les nouvelle exemplaire de cette livres
                    }
                }
                modifierLivre(liteLivre, appWidgets->modify_titre, titre, categ,&aut);//l'appele a la fonction qui nous appermet de modifier u  livre donnee
            }else{
                afficheErreurLivres("l'un des champs est vide, l'ajout a été annulé",globalWidgLivre);
            }
            gtk_widget_hide(appWidgets->box_detail);//cacher la zmne de detaille
            gtk_widget_show(appWidgets->container_table);//afficher la table des livres
            afichageListeLivre(liteLivre, appWidgets);//l'apille a la fonction d'affichage de la liste des livres
            break;
        case GTK_RESPONSE_DELETE_EVENT:break;
        case GTK_RESPONSE_CLOSE:break;
    }
    gtk_widget_hide(appWidgets->dialog_livre);//cacher le dialoge
}
//si on click sur le boutton chercher
void on_recherche_livre_clicked(GtkButton *button,app_widgets_livres *appWidgets){
    char titre[50];
    livre *liv;
    gtk_widget_hide(appWidgets->input_chercher_livre);//cacher l'input de recherche
    gtk_widget_hide(appWidgets->chercher_livre);//cacher le boutton de validation de recherche
    gtk_widget_show(appWidgets->annuler_chercher_livre);//afficher la boutton de retoure de recherche
    strcpy(titre,gtk_entry_get_text((GtkEntry *) appWidgets->input_chercher_livre));//la recuperation de titre que on est entrer dans l'input de la zone de recherche
    //si il contient du texte
    if(strlen(titre)!=0){
        liv=RechercheLivre(titre,liteLivre);//appele a la fonctions qui fait la recherche d'un livre par titre
        //si il exsiste
        if(liv) {
            afficherUnLive(liv->num_liv, appWidgets);//on va afficher cette livre
            return;
        }
    }
    afficherUnLive(-1,appWidgets);
}
//si on click sur a botton retoure
void on_cancel_recherche_livre_clicked(GtkButton *button,app_widgets_livres *appWidgets){
    gtk_widget_show(appWidgets->input_chercher_livre);//reaficher la zone de recherche
    gtk_widget_show(appWidgets->chercher_livre);//reaficher le boutton  de recherche
    gtk_widget_hide(appWidgets->annuler_chercher_livre);//cacher le boutton de retoure
    afichageListeLivre(liteLivre,appWidgets);//reafficher la liste des livres

}
//la fonction qui gere les traitment sur les livres
void gestionLivres(app_widgets_home *appWidgetsHome){
    GList *children, *iter;
    livre *donnes=NULL;
    g_slist_free((GSList *) globalWidgLivre);
    app_widgets_livres *appWidgetsLivre=g_slice_new(app_widgets_livres);//l'initialisation de l'appwidget
    globalWidgLivre=appWidgetsLivre;
    appWidgetsLivre->builder=appWidgetsHome->builder;//l'initialisation de builder par le puilder de la page home

    appWidgetsLivre->box_detail=GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "detail_liv"));//l'initialisation de la box de detaille
    appWidgetsLivre->box_table= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "table_livres"));//l'initialisation de box de la table
    appWidgetsLivre->container_table= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "body_livre"));//l'initialisatton de la container de la table
    // pour le tool bar
    appWidgetsLivre->ajouter_livre= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "ajouter_livre"));//l''inittialisation de la boutton d'ajoute
    appWidgetsLivre->chercher_livre= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "recherche_livre"));//l'initialisation de la boutton de validation de recherche
    appWidgetsLivre->annuler_chercher_livre= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "annuler_recherche_livre"));//l'initialisation de la boutton d'anullation de recherche
    appWidgetsLivre->input_chercher_livre= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "input_chercher_livre"));//l'initialisation de l'input de la recherche
    appWidgetsLivre->categorie= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "categorie"));//l'initialisation des cambobox de categorie
    appWidgetsLivre->ajouter_livre= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "ajouter_livre"));
    //le traitment sur les detailes
    appWidgetsLivre->titre_liv_details= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "titre_liv_details"));//l'initialisation de lablel de titre
    appWidgetsLivre->catregorie_liv_details= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "catregorie_liv_details"));//l'initialisation de label de categorie
    appWidgetsLivre->aut_nom_liv_details= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "aut_nom_liv_details"));//l'initialisation de label de nom d'auteur
    appWidgetsLivre->aut_prenom_liv_details= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "aut_prenom_liv_details"));//l'initialisation de label de prenom de l'auteur de livre
    appWidgetsLivre->nb_empr_liv= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "nb_empr_liv"));//l'initialisation de lbel de nombre des exemplaire empaunter
    appWidgetsLivre->nb_desp_liv= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "nb_desp_liv"));//l'initialisaion de label de nombre des exemplaire desponibe
    appWidgetsLivre->retourner_livre= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "retourner_livre"));//l'initialisation de la boutton de retoure a la zone qui contient la table des lvres
    appWidgetsLivre->supprimer_liv= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "supprimer_liv"));//l'initialisation de la boutton de la supprission
    appWidgetsLivre->modifier_liv= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "modifier_liv"));//l'initialisation de la boutton de modifications
    // le traitement de la cambobox
    appWidgetsLivre->input_titre_liv= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "titre_liv"));//l'initialisation de l'input de titre
    appWidgetsLivre->input_categorie_liv= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "categorie_liv"));//l'initialisation cambobox des catecorie
    appWidgetsLivre->input_prenom_atr= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "prenom_atr"));//l'initialisation de l'input de pprenom
    appWidgetsLivre->input_nom_autr= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "nom_autr"));//l'initialisation de l'input de nom de auteur
    appWidgetsLivre->valider_ajouter= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "valider_ajouter_livre"));//l'initialisation de la boutton de validation
    appWidgetsLivre->cancel_ajouter= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "cancel_ajouter_livre"));//l'initialisation de la boutton d'annulation l'ajout
    appWidgetsLivre->dialog_livre= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "dialog_livre"));//l'initialisation de dialog de livre
    appWidgetsLivre->nbr_exemplaire= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "nbr_exemplaire"));//l'initialisation de la zone de nombre des exemplaire
    appWidgetsLivre->erreur_dialog= GTK_WIDGET(gtk_builder_get_object(appWidgetsLivre->builder, "dialog_erreur"));

    //l'initialisation des signaux
    g_signal_connect(GTK_WIDGET(appWidgetsLivre->ajouter_livre), "clicked", G_CALLBACK(on_ajoute_livre_clicked ), appWidgetsLivre);//l'initialisation de la signal de boutton d'ajout
    g_signal_connect(GTK_WIDGET(appWidgetsLivre->valider_ajouter), "clicked", G_CALLBACK(on_valider_ajouter_livre_clicked), appWidgetsLivre);//l'initialisation de signal de la boutton de validation
    g_signal_connect(GTK_WIDGET(appWidgetsLivre->cancel_ajouter), "clicked", G_CALLBACK(on_cancel_ajouter_livre_clicked), appWidgetsLivre);//l'initialisation de signal de boutton d'annulation
    g_signal_connect(GTK_WIDGET(appWidgetsLivre->retourner_livre), "clicked", G_CALLBACK(on_retourn_clicked ), appWidgetsLivre);//l'initiaisation du signal de boutton retoure
    g_signal_connect(GTK_WIDGET(appWidgetsLivre->supprimer_liv), "clicked", G_CALLBACK(on_supprime_livre_clicked ), appWidgetsLivre);//l'initialisation de signale de la boutton de supprisions
    g_signal_connect(GTK_WIDGET(appWidgetsLivre->modifier_liv), "clicked", G_CALLBACK(on_modifier_livre_clicked ), appWidgetsLivre);//l'initiaisatin de la boutton de la modifications
    g_signal_connect(GTK_WIDGET(appWidgetsLivre->chercher_livre), "clicked", G_CALLBACK(on_recherche_livre_clicked), appWidgetsLivre);//l'initialisation de signal de boutton de validation de recherche
    g_signal_connect(GTK_WIDGET(appWidgetsLivre->annuler_chercher_livre), "clicked", G_CALLBACK(on_cancel_recherche_livre_clicked ), appWidgetsLivre);//l'initialisation de la boutton de retoure e la recherche

    afichageListeLivre(liteLivre, appWidgetsLivre);//l'appelle a la fonction d'affichage des livres pour afficher la liste des livre
    gtk_builder_connect_signals(appWidgetsLivre->builder, appWidgetsLivre);//coonecter tous les signalle que on est initialisie
    gtk_widget_hide(appWidgetsLivre->annuler_chercher_livre);//masquer le boutton de retoure qui est affiche apprer le recherche d'une livre
    gtk_widget_hide(appWidgetsLivre->box_detail);//masquer la zone qui va contient les detaille d'un livre

}