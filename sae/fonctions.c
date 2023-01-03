#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonctions.h"
#define TAILLE_MAX 1000

void initTab(char **tab)
{
    int i;
    for(i = 0; i < TAILLE_MAX; i++)
    {
        tab[i] = "";
    }
}

void initTabInt(int tab[])
{
    int i;
    for(i = 0; i < TAILLE_MAX; i++)
    {
        tab[i] = 0;
    }
}

int tabip(char *adip, char **tab) /// v�rifie si une addresse est pr�sente dans le tableau et l'ajoute si non
{
    int i;
    for(i = 0; i < TAILLE_MAX; i++)
    {
        if(strcmp(adip, tab[i]) == 0)
        {
            return i;
        }
        if(strcmp(tab[i], "") == 0)
        {
            tab[i] = strdup(adip);
            return i;
        }
    }
    return 0;
}

void initsessions(int s[])
{
    int i;
    for(i = 0; i<5000; i++)
    {
        s[i]=0;
    }
}

int remplirsessions(int s[5000], int a, int nb)
{
    int i;
    for(i = 0; i<5000; i++)
    {
        if(s[i] == a)
        {
            return nb;
        }
        if(s[i] == 0)
        {
            s[i] = a;
            return nb+1;
        }
    }
    return nb;
}



int verifheure(int time[50][12][31][24], int a, int m, int j, int h)
{
    if(time[a-2001][m-1][j-1][h-1]==1)
    {
        return 0;
    }
    else
    {
        time[a-2001][m-1][j-1][h-1]=1;
        return 1;
    }

    return 0;
}
