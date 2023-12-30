
/*

Remarque : Dans ce programme, j'ai un problème avec le dessin d'un diagramme de Gantt comme je l'ai fait dans FIFO et je le dessine comme ceci :

   0 -> P1 <- 4 -> p2 <- 8 -> ...........
   
   Je pense que c'est un moyen simple pour vous, madame, et merci

*/
#include<stdio.h>

struct TempsProcessus
{
    int p, arrivee, duree, attente, turnaround, reste; // Structure pour stocker les informations sur les processus
};

void trierArrivee(struct TempsProcessus a[], int nbProcessus)
{
    int i, j;
    struct TempsProcessus temp;
    for(i = 0; i < nbProcessus; i++)
    {
        for(j = i + 1; j < nbProcessus; j++)
        {
            if(a[i].arrivee > a[j].arrivee) // Trie les processus par ordre d'arrivée croissant
            {
                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }
    }
    return;
}

int main()
{
    int i, j, nbProcessus, temps, restants, flag = 0, quantum;
    struct TempsProcessus processus[100];
    float avgAttente = 0, avgTurnaround = 0;

    printf("Algorithme d'ordonnancement Round Robin\n");
    printf("Note :\n1. L'heure d'arrivée d'au moins un processus doit être 0\n2. Le processeur ne doit jamais être inactif\n");
    
    printf("Entrer le nombre de processus : ");
    scanf("%d", &nbProcessus);
    
    restants = nbProcessus;
    for(i = 0; i < nbProcessus; i++)
    {
        printf("Entrer l'heure d'arrivée et la durée du processus P%d : ", i);
        scanf("%d%d", &processus[i].arrivee, &processus[i].duree);
        processus[i].p = i;
        processus[i].reste = processus[i].duree; // Initialise les variables pour chaque processus
    }
    
    trierArrivee(processus, nbProcessus); // Trie les processus par ordre d'arrivée
    
    printf("Entrer la taille de la tranche de temps (quantum) : ");
    scanf("%d", &quantum);
    
    printf("\n***************************************\n");
    printf("Diagramme de Gantt\n");
    printf("0");
    
    for(temps = 0, i = 0; restants != 0;) // Exécute le planificateur jusqu'à ce que tous les processus soient terminés
    {
        if(processus[i].reste <= quantum && processus[i].reste > 0) // Si le processus peut s'exécuter pour une tranche de temps complète
        {
            temps = temps + processus[i].reste;
            printf(" -> [P%d] <- %d", processus[i].p, temps); // Affiche le processus en cours d'exécution
            processus[i].reste = 0;
            flag = 1;
        }
        else if(processus[i].reste > 0) // Si le processus a encore du temps d'exécution restant
        {
            processus[i].reste = processus[i].reste - quantum;
            temps = temps + quantum;
            printf(" -> [P%d] <- %d", processus[i].p, temps); // Affiche le processus en cours d'exécution
        }
        if(processus[i].reste == 0 && flag == 1) // Si le processus est terminé
        {
            restants--;
            processus[i].turnaround = temps - processus[i].arrivee; // Calcul du temps de turnaround
            processus[i].attente = temps - processus[i].arrivee - processus[i].duree; // Calcul du temps d'attente
            avgAttente = avgAttente + temps - processus[i].arrivee - processus[i].duree;
            avgTurnaround = avgTurnaround + temps - processus[i].arrivee;
            flag = 0;
        }
        if(i == nbProcessus - 1)
            i = 0;
        else if(processus[i + 1].arrivee <= temps)
            i++;
        else
            i = 0;
    }
    
    printf("\n\n");

    
    printf("***************************************\n");
    avgAttente = avgAttente / nbProcessus; // Calcul du temps d'attente moyen
    avgTurnaround = avgTurnaround / nbProcessus; // Calcul du temps de turnaround moyen
    printf("Temps d'attente moyen : %.2f\n", avgAttente);
    printf("Temps de turnaround moyen : %.2f\n", avgTurnaround);
    
    return 0;
}

