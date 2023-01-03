#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonctions.h"
#define TAILLE_MAX 1000

int main(int argc, char * argv[])
{
    if(argc != 3)
    {
        printf("\nVeuillez passer en argument le chemain de votre fichier de logs puis le chemain du resultat\n");
    }
    else{

    FILE *fichier = NULL;
    ///fichier = fopen("./log2_hmailserver_train.log", "r+");
    fichier = fopen(argv[1], "r+");

    ///char* nomfichier = strrchr("./log2_hmailserver_train.log", '/') + 1; /// +1 pour éliminer le / comme ça on a le nom du fichier
    ///char* nomfichier = strrchr(argv[1], '/') + 1; /// +1 pour éliminer le / comme ça on a le nom du fichier
    char* nomfichier = argv[1];

    int nlignes = 0;                /// compteur du nombre de lignes
    int tcp = 0;                    /// compteur de protocole TCP
    int debug = 0;                  /// compteur de debug
    int error = 0;                  /// compteur d'erreurs
    int local = 0;                  /// compteur de local
    int smtpd = 0;                  /// compteur de protocole SMTPD
    int smtpc = 0;                  /// compteur de protocole SMTPC
    int pop3d = 0;                  /// compteur de protocole POP3D
    int imapd = 0;                  /// compteur de protocole IPAPD
    int application = 0;            /// compteur de protocole APPLICATION
    int nbsessions = 0;             /// compteur du nombre de sessions
    int nblignessessions = 0;       /// compteur du nombre de lignes de sessions
    int positionip = 0;             /// valeur qui va servir à retenir la position d'une adresse IP dans le tableaud d'adresses
    int sessions[5000];             /// tableau qui va contenir toutes les sessions
    int moyheures[50][12][31][24];  /// tableau qui va servir à savoir quel heure est présente dans le fichier et à quel jour
    int moyheuresip[24][TAILLE_MAX];  /// tableau qui va servir à compter le nombre de messages par heure pour chaque adresse
    int moyheuresipdiv[24][TAILLE_MAX];  /// tableau qui va servir à compter le nombre de messages par heure pour chaque adresse
    static int tempsip[50][12][31][24][TAILLE_MAX];  /// tableau qui va servir à vérifier si une heure est présente pour chaque adresse
    int nbmsgadresses[TAILLE_MAX];  /// tableau qui va contenir le nombre de messages envoyés par une adresse dans le même ordre
    int nbrecadresses[TAILLE_MAX];  /// que le tableau d'adresses

    char ip[18];                    /// chaine de caractères qui va stocker une adresse IP
    char str[TAILLE_MAX];           /// chaine de caractères qui va stocker le type de ligne
    char day[5];                    /// chaine de caractères qui va stocker le jour
    char month[5];                  /// chaine de caractères qui va stocker le mois
    char year[8];                   /// chaine de caractères qui va stocker l'année
    char hour[5];                   /// chaine de caractères qui va stocker l'heure
    char ligne[TAILLE_MAX];         /// chaine de caractères qui va stocker une ligne
    char *adresses[TAILLE_MAX];     /// tableau de string qui va stocker toutes les adresses
    char session[10];               /// chaine de caractères qui va contenir le numéro d'une session
    char sent[4];                   /// chaine de caractères qui va contenir le début d'une ligne pour voir si c'est un SENT ou un RECEIVED

    Heure stockageheure;            /// structure de données qui va servir au nombre de lignes par heure

    int i, j;
    for(i=0; i<24; i++)             /// initialisation de la structure de données et de tableaux
    {
        stockageheure.hour[i]=i+1;
        stockageheure.nb[i]=0;
        stockageheure.moy[i]=0;
        for(j=0; j<TAILLE_MAX; j++)
        {
            moyheuresip[i][j]=0;
            moyheuresipdiv[i][j]=0;
        }
    }

    initTab(adresses);              /// initialisation des tableaux
    initsessions(sessions);
    initTabInt(nbmsgadresses);

    while (fgets(ligne, TAILLE_MAX, fichier) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
    {
        sscanf(ligne, "%s%*s", str);

        if(strcmp(str,"\"TCPIP\"") == 0)  // Si la chaine de caracteres correspond à TCPIP
        {
            tcp = tcp + 1;
        }

        if(strcmp(str,"\"POP3D\"") == 0)
        {
            pop3d = pop3d + 1;
            sscanf( ligne, "%*s%*d%*d%*s%*s%s%*c%5s%*s", ip, sent-1);   // On récupère l'adresse IP et le début du contenu
            positionip = tabip(ip, adresses);
            if(strcmp(sent, "SENT") == 0)
                nbmsgadresses[positionip] ++;
            if(strcmp(sent, "RECE") == 0)
                nbrecadresses[positionip] ++;
            if(tempsip[atoi(year)-2001][atoi(month)-1][atoi(day)-1][atoi(hour)-1][positionip]==1)
            {
                moyheuresip[atoi(hour)-1][positionip] ++;
;            }
            else
            {
                tempsip[atoi(year)-2001][atoi(month)-1][atoi(day)-1][atoi(hour)-1][positionip]=1;
                moyheuresipdiv[atoi(hour)-1][positionip] ++;
                moyheuresip[atoi(hour)-1][positionip] ++;
            }
        }

        if(strcmp(str,"\"SMTPD\"") == 0)
        {
            smtpd = smtpd + 1;
            sscanf( ligne, "%*s%*d%*d%*s%*s%s%*c%5s%*s", ip, sent-1);   // On récupère l'adresse IP et le début du contenu
            positionip = tabip(ip, adresses);
            if(strcmp(sent, "SENT") == 0)
                nbmsgadresses[positionip] ++;
            if(strcmp(sent, "RECE") == 0)
                nbrecadresses[positionip] ++;
            if(tempsip[atoi(year)-2001][atoi(month)-1][atoi(day)-1][atoi(hour)-1][positionip]==1)
            {
                moyheuresip[atoi(hour)-1][positionip] ++;
            }
            else
            {
                tempsip[atoi(year)-2001][atoi(month)-1][atoi(day)-1][atoi(hour)-1][positionip]=1;
                moyheuresipdiv[atoi(hour)-1][positionip] ++;
                moyheuresip[atoi(hour)-1][positionip] ++;

            }
        }

        if(strcmp(str,"\"APPLICATION\"") == 0)
        {
            application ++;
        }

        if(strcmp(str,"\"DEBUG\"") == 0)
        {
            debug ++;
        }

        if(strcmp(str,"\"ERROR\"") == 0)
        {
           error ++;
        }

        if(strcmp(str,"\"LOCAL\"") == 0)
        {
            local ++;
        }

        if(strcmp(str,"\"IMAPD\"") == 0)
        {
            imapd ++;
            sscanf( ligne, "%*s%*d%*d%*s%*s%s%*c%5s%*s", ip, sent-1);   // On récupère l'adresse IP et le début du contenu
            positionip = tabip(ip, adresses);
            if(strcmp(sent, "SENT") == 0)
                nbmsgadresses[positionip] ++;
            if(strcmp(sent, "RECE") == 0)
                nbrecadresses[positionip] ++;
            if(tempsip[atoi(year)-2001][atoi(month)-1][atoi(day)-1][atoi(hour)-1][positionip]==1)
            {
                moyheuresip[atoi(hour)-1][positionip] ++;
            }
            else
            {
                tempsip[atoi(year)-2001][atoi(month)-1][atoi(day)-1][atoi(hour)-1][positionip]=1;
                moyheuresipdiv[atoi(hour)-1][positionip] ++;
                moyheuresip[atoi(hour)-1][positionip] ++;
            }
        }

        if(strcmp(str,"\"SMTPC\"") == 0)
        {
            smtpc ++;
            sscanf( ligne, "%*s%*d%*d%*s%*s%s%*c%5s%*s", ip, sent-1);   // On récupère l'adresse IP et le début du contenu
            positionip = tabip(ip, adresses);
            if(strcmp(sent, "SENT") == 0)
                nbmsgadresses[positionip] ++;
            if(strcmp(sent, "RECE") == 0)
                nbrecadresses[positionip] ++;
            if(tempsip[atoi(year)-2001][atoi(month)-1][atoi(day)-1][atoi(hour)-1][positionip]==1)
            {
                moyheuresip[atoi(hour)-1][positionip] ++;
            }
            else
            {
                tempsip[atoi(year)-2001][atoi(month)-1][atoi(day)-1][atoi(hour)-1][positionip]=1;
                moyheuresipdiv[atoi(hour)-1][positionip] ++;
                moyheuresip[atoi(hour)-1][positionip] ++;
            }

            moyheuresip[24][positionip] ++;
        }


        if((strcmp(str,"\"TCPIP\"")!=0)&&(strcmp(str,"\"APPLICATION\"")!=0)&&(strcmp(str,"\"DEBUG\"")!=0)&&(strcmp(str,"\"ERROR\"")!=0)&&strcmp(str,"\"LOCAL\""))  // Si la chaine de caracteres ne correspond pas à TCPIP ou APPLICATION
        {
            sscanf( ligne, "%*s%*d%s%*c%*c%4s%*c%2s%*c%2s%*c%2s", session, year, month, day, hour);
            stockageheure.nb[atoi(hour)-1] ++;
            nbsessions = remplirsessions(sessions, atoi(session), nbsessions);
            nblignessessions ++;
            if(verifheure(moyheures, atoi(year), atoi(month), atoi(day), atoi(hour))==1)
            {
                stockageheure.moy[atoi(hour)-1] ++;
            }

        }
        else
        {
            sscanf( ligne, "%*s%*d%*c%*c%4s%*c%2s%*c%2s%*c%2s", year, month, day, hour);
            stockageheure.nb[atoi(hour)] ++;
        }

        nlignes ++;
    }

    fclose( fichier );

    ///FILE    *html = fopen( "index.html", "w");
    FILE    *html = fopen( argv[2], "w");

    fprintf( html, "<!DOCTYPE html>\n");
    fprintf( html, "<html lang=\"fr\">\n");
    fprintf( html, "    <head>\n\
        <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n\
        <title> SAE15 </title>\n\
        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n\
        <script src=\"https://cdn.jsdelivr.net/npm/chart.js\"></script>\n");
    fprintf( html, "\
        <style>\n\
            @import url('https://fonts.googleapis.com/css2?family=Poppins:wght@300&display=swap'); \n\
            body{\n\
                font-family: Poppins;\n\
                background-color: #2e2e2e;\n\
                color: #f0f0f0;\n\
                text-align:center;\n\
                font-size: 15pt;\n\
            }\n\
            p{\n\
                margin: 30px;\n\
                margin-left: 30px;\n\
            }\n\
            footer{\n\
                margin: 20px;\n\
                margin-top: 40px;\n\
                margin-bottom: 30px;\n\
                font-size: 12pt;\n\
            }\n\
            h1{\n\
                margin:30px;\n\
                margin-left:10px;\n\
                font-size: 40pt;\n\
            }\n\
            h2{\n\
                margin-top:30px;\n\
                margin-bottom:20px;\n\
                margin-left:20px;\n\
                font-size: 30pt;\n\
            }\n\
            ul{\n\
            }\n\
            li.sommaire{\n\
                margin :10px;\n\
                font-size:20pt;\n\
            }\n\
            li {\n\
                color :rgba(255, 109, 31, 1);\n\
            }\n\
            li span{\n\
                color :#f0f0f0;\n\
            }\n\
            a{\n\
                text-decoration: none;\n\
                color: #f0f0f0;\n\
            }\n\
            canvas{\n\
                background-color: #f0f7ff;\n\
                color: #061424;\n\
                border-radius:7px;\n\
                padding-left: 0;\n\
                padding-right: 0;\n\
                margin-left: auto;\n\
                margin-right: auto;\n\
                margin-top: 30px;\n\
                margin-bottom :30px;\n\
                display: block;\n\
            }\n\
            .wrapper {\n\
                text-align: center;\n\
                padding-left: 43vw;\n\
            }\n\
            .wrapper ul {\n\
                margin: 0;\n\
                display: inlie-table;\n\
            }\n\
            .wrapper li {\n\
                text-align: left;\n\
            }\n\
            .title{\n\
                margin-left:-145px;\n\
            }\n\
            .sub{\n\
                margin-left:125px;\n\
            }\n\
        </style>\n\
    </head>\n");
    fprintf( html, "    <body>\n");
    fprintf( html, "        <h1>Analyse du fichier de logs : %s</h1> \n", nomfichier);
    fprintf( html, "        <h2>Sommaire</h2> \n");
    fprintf( html, "        <div class=\"wrapper\"><ul>\n\
                <li class=\"sommaire\"><span><a href=\"#struct\">Structure du log</a></span></li>\n\
                <li class=\"sommaire\"><span><a href=\"#lignes\">Lignes</a></span></li>\n\
                <li class=\"sommaire\"><span><a href=\"#protocoles\">Protocoles</a></span></li>\n\
                <li class=\"sommaire\"><span><a href=\"#sessions\">Sessions</a></span></li>\n\
                <li class=\"sommaire\"><span><a href=\"#adresses\">Adresses</a></span></li>\n\
            </ul></div>");
    fprintf( html, "        <h2><a name=\"struct\">Structure d'une ligne du fichier</a></h2>\n");
    fprintf( html, "        <p>Ce programme analyse des logs de serveurs hmail, voici comment est construite une ligne :</p>");
    fprintf( html, "        <p>\"POP3D\"	2780	40	\"2021-09-23 14:25:15.260\"	\"10.0.0.4\"	\"SENT: +OK 8 3944\"</p>");
    fprintf( html, "        <p>Chaque ligne commence par le nom du protocole, le PID qui l'a enclenchée, un numéro de session, la date au moment de la création du log, l'adresse IP utilisée et le contenu du message.</p>");
    fprintf( html, "        <h2><a name=\"lignes\">Analyse des lignes du fichier de logs</a></h2>\n");
    fprintf( html, "        <p>Ce fichier contient %d lignes</p>\n", nlignes);
    fprintf( html, "        <p>Moyenne des lignes par heure :</p>\n");

    fprintf( html, "        <div>\n\
            <canvas id=\"graph1\" width=\"1000px\" ></canvas>\n\
        </div>\n");
    fprintf( html, "        <h2><a name=\"protocoles\">Analyse des protocoles du fichier de logs</a></h2>\n");
    fprintf( html, "        <div class=\"wrapper\"><ul>\n\
            <li><span>TCPIP = %d</span></li>\n\
            <li><span>POP3D = %d</span></li>\n\
            <li><span>SMTPD = %d</span></li>\n\
            <li><span>APPLICATION = %d</span></li>\n\
            <li><span>IMAPD = %d</span></li>\n\
            <li><span>SMTPC = %d</span></li>\n\
            <li><span>DEBUG = %d</span></li>\n\
            <li><span>ERROR = %d</span></li>\n\
            <li><span>LOCAL = %d</span></li>\n\
        </ul></div>\n\
        <canvas id=\"graph2\" width=\"450px\"></canvas>\n", tcp, pop3d, smtpd, application, imapd, smtpc, error, debug, local);

    fprintf( html, "\
        <script>\n\
            const dataprotocoles = {\n\
                labels: ['TCPIP', 'POP3D', 'STMPD', 'APPLICATION', 'IMAPD', 'SMTPC'],\n\
                datasets: [{\n\
                    label: 'R\u00E9partition des protocoles',\n\
                    backgroundColor: [\n\
                        'rgba(255, 99, 132, 0.2)',\n\
                        'rgba(54, 162, 235, 0.2)',\n\
                        'rgba(255, 206, 86, 0.2)',\n\
                        'rgba(75, 192, 192, 0.2)',\n\
                        'rgba(153, 102, 255, 0.2)',\n\
                        'rgba(255, 159, 64, 0.2)',\n\
                    ],\n\
                    color: '#666',\n\
                    borderColor: [\n\
                        'rgba(255, 99, 132, 0.2)',\n\
                        'rgba(54, 162, 235, 0.2)',\n\
                        'rgba(255, 206, 86, 0.2)',\n\
                        'rgba(75, 192, 192, 0.2)',\n\
                        'rgba(153, 102, 255, 0.2)',\n\
                        'rgba(255, 159, 64, 0.2)',\n\
                    ],\n\
                    data: [%d, %d, %d, %d, %d, %d", tcp, pop3d, smtpd, application, imapd, smtpc);

    fprintf( html, "],\n\
                }]\n\
            };\n\
        const configprotocoles = {\n\
            type: 'doughnut',\n\
            data: dataprotocoles,\n\
            options: {\n\
                devicePixelRatio: 2,\n\
                responsive: false\n\
            }\n\
        };\n\
        const data = {\n\
            labels: ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24'],\n\
            datasets: [{\n\
                label: 'Nombre de lignes par heure en moyenne',\n\
                backgroundColor: 'rgba(255, 109, 31, 0.5)',\n\
                color: '#666',\n\
                borderColor: 'rgba(255, 109, 31, 0.7)',\n\
            data: [");
    for(i=0; i<23; i++)
    {
        if(stockageheure.moy[i]!=0)
        {
            fprintf( html, "%d, ", stockageheure.nb[i]/stockageheure.moy[i]);
        }
        else
        {
            fprintf( html, "0, ");
        }
    }
    if(stockageheure.moy[23]!=0)
        {
            fprintf( html, "%d", stockageheure.nb[23]/stockageheure.moy[23]);
        }
        else
        {
            fprintf( html, "0");
        }

    fprintf( html, "],\n\
                }]\n\
            };\n\
        const config = {\n\
            type: 'bar',\n\
            data: data,\n\
            options: {\n\
                responsive: false,\n\
                devicePixelRatio: 2\n\
            }\n\
        }\n\
        const graph2 = new Chart(document.getElementById('graph2'), configprotocoles);\n\
        const graph1 = new Chart(document.getElementById('graph1'), config);\n\
        </script>\n");

    fprintf( html, "        <h2><a name=\"sessions\">Analyse des sessions du fichier de logs</a></h2> \n");
    fprintf( html, "        <p>Il contient %d sessions</p>\n", nbsessions);
    fprintf( html, "        <p>Le nombre moyen de lignes par session est de %d</p>\n", nblignessessions/nbsessions);

    int nbadresses = 0;
    for(i = 0; i<TAILLE_MAX; i++)
    {
        if( strcmp(adresses[i], "") != 0)
            nbadresses ++;
    }

    fprintf( html, "        <h2><a name=\"adresses\">Analyse des adresses du fichier de logs</a></h2> \n");

    fprintf( html, "        <p>Il contient %d adresses :</p>\n", nbadresses);

    fprintf( html, "        <ul>\n");

    for(i = 0; i<nbadresses; i++)
    {
        adresses[i][strlen(adresses[i])-1] = '\0';  /// retirer le dernier char
        fprintf( html, "\
            <li class=\"title\"><span>%s : </span>\n\
                <div class=\"wrapper\"><ul>\n\
                    <li class=\"sub\"><span>%d messages envoy\u00E9s</span></li>\n\
                    <li class=\"sub\"><span>%d messages reçus</span></li>\n\
                </ul></div>\n\
            </li>\n", adresses[i]+1, nbmsgadresses[i], nbrecadresses[i]);  /// +1 pour retirer le 1er char
    }
    fprintf( html, "        </ul>\n\n\n");

    for(i = 0; i<TAILLE_MAX; i++)
    {
        if( strcmp(adresses[i], "") != 0)
        {
            fprintf( html, "\
        <canvas id=\"graph%d\" width=\"1200px\" height =\"500px\"></canvas>\n\n\
        <script>\n\
            const dataip%d = {\n\
                labels: ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24'],\n\
                datasets: [{\n\
                    label: 'Moyenne des lignes générées par heure par l\\'adresse %s',\n\
                    backgroundColor:\'rgba(255, 109, 31, 0.7)',\n\
                    color: '#666',\n\
                    borderColor:'rgba(255, 109, 31, 0.8)',\n\
                    data: [", i+10, i+10, adresses[i]+1);

    for(j=0; j<23; j++)
    {
        if(moyheuresipdiv[j][i]!=0)
        {
            fprintf( html, "%d, ", moyheuresip[j][i]/moyheuresipdiv[j][i]);
        }
        else
        {
            fprintf( html, "0, ");
        }
    }
    if(moyheuresip[j][i]!=0)
        {
            fprintf( html, "%d", moyheuresip[23][i]/moyheuresipdiv[23][i]);
        }
        else
        {
            fprintf( html, "0");
        }

    fprintf( html, "],\n\
                }]\n\
            };\n\
            const config%d = {\n\
                type: 'bar',\n\
                data: dataip%d,\n\
                options: {\n\
                    responsive: false,\n\
                    devicePixelRatio: 2\n\
                }\n\
            }\n\
            const graph%d = new Chart(document.getElementById('graph%d'), config%d);\n\
        </script>\n\n", i+10, i+10, i+10, i+10, i+10);
        }
    }

    fprintf( html, "    </body>\n");
    fprintf( html, "    <footer>\n\
        <p> Cr\u00E9\u00E9 par Schmitz Arthur et Belhadj Nassim</p>\n\
    </footer>\n");
    fprintf( html, "</html>");

    fclose( html );

    i = 232;
    printf("\nAnalyse compl\%cte\n", i);
    return 1;
    }
}

