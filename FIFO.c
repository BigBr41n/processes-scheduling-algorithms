#include <stdio.h>
#include <stdlib.h>
#define MAX 100



typedef struct
{
    int pid; // the pid of the process
    int arrivale  ; // the arivale time for each process 
    int excut_time; // excution time of each 
    int waiting_time; // the waiting time of the process before lunch 
    int total_spent; // how the process takes time to lunch 
} Process;

void print_gantt_chart(Process p[], int n); // the prototype of the function that allow to display the execution process chart






int main()
{
    Process p[MAX];
    int i, j, n;
    int sum_waiting_time = 0, sum_total_spent;
    printf("Entrer le nombre de processus: ");
    scanf("%d", &n);
    printf("Enter le temps d'execution de chaque processus:\n");
    for(i=0; i<n; i++) {
        p[i].pid = i+1;
        printf("P[%d] : ", i+1);
        scanf("%d", &p[i].excut_time);
    }
    printf("Enter le temps d'arrive de chaque processus:\n");
    for(i=0; i<n; i++) {
        p[i].pid = i+1;
        printf("P[%d] : ", i+1);
        scanf("%d", &p[i].arrivale);
    }
    
    

    // calculate waiting time and turnaround time
    p[0].waiting_time = 0 ; 
    p[0].total_spent = p[0].excut_time;

    for(i=1; i<n; i++) {
        p[i].waiting_time = p[i-1].total_spent-p[i].arrivale;
        p[i].total_spent = p[i].waiting_time + p[i].excut_time;
    }



    // calculate sum of waiting time and sum of turnaround time
    for(i=0; i<n; i++) {
            sum_waiting_time += p[i].waiting_time;
            sum_total_spent += p[i].total_spent;
    }
    
    

    // print waiting time and average waiting time
    puts("");
    puts("");
    printf("Temps d'attente total      : %2d\n", sum_waiting_time);
    printf("Temps d'attente moyenne    : %.2f\n", (double)sum_waiting_time / (double) n);


    // print  execution chart
    puts(""); 
    puts("          Diagramme d'execution          ");
    puts("          *********************          ");
    print_gantt_chart(p, n);
    return 0;
}






void print_gantt_chart(Process p[], int n)
{
    int i, j;
    // print top bar
    printf(" ");
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].excut_time; j++) printf("--");
        printf(" ");
    }
    printf("\n|");

    // printing process id in the middle
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].excut_time - 1; j++) printf(" ");
        printf("P%d", p[i].pid);
        for(j=0; j<p[i].excut_time - 1; j++) printf(" ");
        printf("|");
    }
    printf("\n ");
    // printing bottom bar
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].excut_time; j++) printf("--");
        printf(" ");
    }
    printf("\n");

    // printing the time line
    printf("0");
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].excut_time; j++) printf("  ");
        if(p[i].total_spent > 9) printf("\b"); // backspace : remove 1 space
        printf("%d", p[i].total_spent);

    }
    printf("\n");

}
