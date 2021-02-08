cmake_minimum_required(VERSION 3.17)
project(Bibilo-ENSIAS)

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_CXX_FLAGS} -std=c99 -rdynamic")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0 )

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS} )

add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(BiblioENSIAS src/main.c "src/pages/gestion des adherents/gestion_adherent.h" "src/pages/gestion des adherents/gestion_adherent.c" "src/pages/gestion des livres/gestion_livre.h" "src/pages/gestion des livres/gestion_livre.c" "src/pages/gestion des empruntes/gestion_emprunte.h" "src/pages/gestion des empruntes/gestion_emprunte.c" "src/pages/page accueil/home.h" "src/pages/page accueil/home.c" src/headers/header.h)
target_link_libraries(BiblioENSIAS ${GTK3_LIBRARIES})
