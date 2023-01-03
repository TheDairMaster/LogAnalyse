#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAILLE_MAX 1000

void initTab(char **tab);
/// entree : tableau de chaines de caract�res
/// sortie : aucune
/// traitement : remplit un tableau de str de TAILLE_MAX avec des ""

void initTabInt(int tab[]);
/// entree : tableau d'entiers de TAILLE_MAX
/// sortie : aucune
/// traitement : remplit un tableau de INT de TAILLE_MAX avec des 0

int tabip(char *adip, char **tab);
/// entree : chaine de caract�res et tableau de chaines de caracteres
/// sortie : position de la chaine de la caractere dans le tableau sous forme d'entier
/// traitement : verifie si la chaine est presente et sinon ajoute la chaine a la fin

void initsessions(int s[]);
/// entree : tableau d'entiers
/// sortie : aucune
/// traitement : remplit un tableau de taille 5000 de 0

int remplirsessions(int s[5000], int a, int nb);
/// entree : tableau
/// sortie : aucune
/// traitement : verifie si un num de sessions est deja present et le met a la fin si non

int verifheure(int time[50][12][31][24], int a, int m, int j, int h);
/// entree : tableau de dimension 4 et 4 entiers
/// sortie : renvoie 1 si la date est presente et 0 sinon
/// traitement : verifie si un num de sessions est deja present et le met a la fin si non

typedef struct Heure Heure; /// pour stocker le nombre d'elements par heure et faire une moyenne par heure
struct Heure                    /// en prenant en compte tous les jours
{
    int nb[24]; /// total de lignes pour chaque heure
    int hour[24]; /// tableau allant de 1 a 24
    int moy[24]; /// nombre de jours ou l'heure apparait
};
