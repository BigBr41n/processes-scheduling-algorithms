#include <stdio.h>
#include <stdlib.h>

typedef struct processus
{
    int id;
    int temps_arrivee;
    int temps_execution;
    int temps_restant;
    int temps_attente;
    int dernier_temps_execution;
    struct processus *suivant;
} Processus;

typedef struct file
{
    int quantum;
    int taille;
    struct processus *tete;
    struct processus *queue;
} File;

void initialiserFile(File *file, int quantum)
{
    file->quantum = quantum;
    file->taille = 0;
    file->tete = NULL;
    file->queue = NULL;
}

int fileEstVide(File *file)
{
    return file->taille == 0;
}

void enfiler(File *file, Processus *processus)
{
    if (file->queue == NULL)
    {
        file->tete = processus;
        file->queue = processus;
    }
    else
    {
        file->queue->suivant = processus;
        file->queue = processus;
    }
    processus->suivant = NULL;
    file->taille++;
}

Processus *defiler(File *file)
{
    if (file->tete == NULL)
    {
        return NULL;
    }
    Processus *processus = file->tete;
    file->tete = processus->suivant;
    if (file->tete == NULL)
    {
        file->queue = NULL;
    }
    file->taille--;
    return processus;
}

typedef struct
{
    int id_processus;
    int temps_debut;
    int temps_fin;
} DiagrammeGantt;

void afficherDiagrammeGantt(DiagrammeGantt *diagrammeGantt, int n)
{
    printf("+");
    for (int i = 0; i < n; i++)
    {
        printf("--------+");
    }
    printf("\n|");

    for (int i = 0; i < n; i++)
    {
        printf("   P%d   ", diagrammeGantt[i].id_processus);
        printf("|");
    }

    printf("\n+");

    for (int i = 0; i < n; i++)
    {
        printf("--------+");
    }

    printf("\n%-9d", diagrammeGantt[0].temps_debut);

    for (int i = 0; i < n; i++)
    {
        printf("%-9d", diagrammeGantt[i].temps_fin);
    }

    printf("\n");
}

int main()
{
    // Lire le nombre de files
    int nb_files;
    printf("Entrez le nombre de files : ");
    scanf("%d", &nb_files);

    // Initialiser les files et lire le quantum pour chaque file
    File *files = malloc(nb_files * sizeof(File));
    for (int i = 0; i < nb_files; i++)
    {
        int quantum;
        printf("Entrez le quantum pour la file %d : ", i + 1);
        scanf("%d", &quantum);
        initialiserFile(&files[i], quantum);
    }

    // Lire le nombre de processus
    int nb_processus;
    printf("Entrez le nombre de processus : ");
    scanf("%d", &nb_processus);

    // Lire le temps d'arrivée et le temps d'exécution de chaque processus après l'avoir initialisé
    Processus *processus = malloc(nb_processus * sizeof(Processus));
    for (int i = 0; i < nb_processus; i++)
    {
        printf("Entrez le temps d'arrivée et le temps d'exécution pour le processus %d : ", i + 1);
        scanf("%d %d", &processus[i].temps_arrivee, &processus[i].temps_execution);
        processus[i].id = i + 1;
        processus[i].temps_restant = processus[i].temps_execution;
        processus[i].temps_attente = -processus[i].temps_arrivee;
        processus[i].dernier_temps_execution = 0;
    }

    printf("\n\n+*****+***************+****************+\n");
    printf("| PID | Temps d'arrivée | Temps d'exécution |\n");
    printf("+*****+***************+****************+\n");

    for (int i = 0; i < nb_processus; i++)
    {
        Processus processusCourant = processus[i];
        printf("| P%-2d | %-14d | %-16d |\n", processusCourant.id, processusCourant.temps_arrivee, processusCourant.temps_execution);
        printf("+-----+----------------+----------------+\n");
    }

    // Trier les processus par temps d'arrivée
    for (int i = 0; i < nb_processus; i++)
    {
        for (int j = i + 1; j < nb_processus; j++)
        {
            if (processus[i].temps_arrivee > processus[j].temps_arrivee)
            {
                Processus temp = processus[i];
                processus[i] = processus[j];
                processus[j] = temp;
            }
        }
    }

    // Enfiler les processus dans la première file
    for (int i = 0; i < nb_processus; i++)
        enfiler(&files[0], &processus[i]);

    // Allouer et initialiser le diagramme de Gantt
    DiagrammeGantt *diagrammesGantt = (DiagrammeGantt *)malloc(nb_files * nb_processus * sizeof(DiagrammeGantt *));
    int indexDiagrammeGantt = 0;

    int tempsActuel = 0;

    // Exécuter les processus dans chaque file jusqu'à ce que toutes les files soient vides
    for (int i = 0; i < nb_files; i++)
    {
        while (!fileEstVide(&files[i]))
        {
            // Défiler le processus de la file
            Processus *processusCourant = defiler(&files[i]);

            // Mettre à jour le temps de début du processus
            diagrammesGantt[indexDiagrammeGantt].id_processus = processusCourant->id;
            diagrammesGantt[indexDiagrammeGantt].temps_debut = tempsActuel;

            // Exécuter le processus pendant le quantum ou jusqu'à ce qu'il se termine
            int quantum = files[i].quantum;
            if (processusCourant->temps_restant < quantum)
                quantum = processusCourant->temps_restant;
            tempsActuel += quantum;
            processusCourant->temps_restant -= quantum;

            // Mettre à jour le temps de fin du processus
            diagrammesGantt[indexDiagrammeGantt].temps_fin = tempsActuel;
            indexDiagrammeGantt++;

            processusCourant->temps_attente += (tempsActuel - quantum) - processusCourant->dernier_temps_execution;
            processusCourant->dernier_temps_execution = tempsActuel;

            // Enfiler le processus dans la file suivante s'il n'est pas terminé
            if (processusCourant->temps_restant > 0 && i < nb_files - 1)
                enfiler(&files[i + 1], processusCourant);
        }
    }

    // Afficher le diagramme de Gantt pour chaque file
    printf("\n\nDiagramme de Gantt :\n");
    afficherDiagrammeGantt(diagrammesGantt, indexDiagrammeGantt);

    // Afficher le temps d'attente pour chaque processus
    printf("\n\nTemps d'attente pour chaque processus :\n");
    for (int i = 0; i < nb_processus; i++)
        printf("P%d: %d   ", processus[i].id, processus[i].temps_attente);

    // Calculer le temps d'attente moyen
    float temps_attente_moyen = 0;
    for (int i = 0; i < nb_processus; i++)
        temps_attente_moyen += processus[i].temps_attente;
    temps_attente_moyen /= nb_processus;
    printf("\n\nTemps d'attente moyen : %.2f\n", temps_attente_moyen);

    // Libérer toute la mémoire allouée
    free(diagrammesGantt);
    free(processus);
    free(files);

    return 0;
}

