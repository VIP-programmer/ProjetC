//
// Created by iliass on 1/14/21.
//
#include "home.h"
void on_window_main_destroy(){
    sauvgarderAdherent();//la sauvegarde de la liste des adhérents avant quitte l'application
    sauvgarderLivres();
    gtk_main_quit();
}
void gerer_pages (GtkNotebook *notebook,GtkWidget *page,guint page_num,app_widgets_home *appWidgets){

    switch (page_num) { //le teste sur le numéro de la page qui en est entrain d'afficher
        case 0:
            gtk_window_set_title((GtkWindow *) appWidgets->window, "Bibliothèque ENSIAS-accueil");
            break;
        case 1:
            sauvgarderLivres();
            sauvgarderAdherent();
            gtk_window_set_title((GtkWindow *) appWidgets->window, "Bibliothèque ENSIAS-gestion des empruntes");
            gestionEmprents(appWidgets);break;//l'appelle a la fonction gestionEmpruntes qui prepare tous les traitements applicable sur les empruntes
        case 2:

            sauvgarderLivres();
            sauvgarderAdherent();
            gtk_window_set_title((GtkWindow *) appWidgets->window, "Bibliothèque ENSIAS-gestion des livres");
            gestionLivres(appWidgets);break;//l'appelle a la fonction gestionLivres qui prepare tous les traitements applicable sur les livres
        case 3:
            sauvgarderLivres();
            sauvgarderAdherent();
            gtk_window_set_title((GtkWindow *) appWidgets->window, "Bibliothèque ENSIAS-gestion des adherents");
            gestionAdherents(appWidgets);break;//l'appelle a la fonction gestionAdherent qui prepare tous les traitements applicable sur les adhérents
    }
}void startHome(int argc, char *argv[]){
    app_widgets_home *appWidgets=g_slice_new(app_widgets_home);
    gtk_init(&argc, &argv);
    chargerAdherents();
    chargerLivres();
    appWidgets->builder = gtk_builder_new();
    appWidgets->styleProvider=gtk_css_provider_new();
    gtk_css_provider_load_from_path(appWidgets->styleProvider,  "../CSS/style.css", NULL);
    gtk_builder_add_from_file (appWidgets->builder, "../glade/home.glade", NULL);
    appWidgets->window = GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "ges_home"));//l'initialisation de la windows
    appWidgets->notebook = GTK_WIDGET(gtk_builder_get_object(appWidgets->builder, "notebook"));
    g_signal_connect( GTK_WIDGET(appWidgets->notebook), "switch-page", G_CALLBACK( gerer_pages ), appWidgets );//le signale envoyé si en click sur un des éléments de la notebook
    g_signal_connect( GTK_WIDGET(appWidgets->window), "destroy", G_CALLBACK( on_window_main_destroy ),NULL );//le signale envoyé si en click sur le button destroy
    gtk_widget_show(appWidgets->window);//affichage de la windows
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(appWidgets->styleProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_main();//Une fois que les signaux sont connectés, il faut lancer la boucle évènementielle en appelant cette fonction
    g_slice_free(app_widgets_home, appWidgets);//libérer un bloc de mémoire qui a été alloué
}

