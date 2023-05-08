#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h>

#define READ 0
#define WRITE 1

int main(void) {
    //int canal_12[2], canal_23[2], canal_31[2];
    int canal_pai1[2], canal_pai2[2], canal_pai3[2];

    char input[256], output[256];

    sem_t sem1, sem2, sem3;
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
    sem_init(&sem3, 0, 0);

    //pipe(canal_12);
    //pipe(canal_23);
    //pipe(canal_31); 
    pipe(canal_pai1);
    pipe(canal_pai2);
    pipe(canal_pai3);

    pid_t pid_1 = fork();
    if (pid_1 == 0) {
        printf ("Processo 1 - Enviando msg pro pai...\n");
        strcpy(output, "Mensagem 1.");
        write(canal_pai1[WRITE], output, sizeof(output) + 1);

        sem_post(&sem1);

        printf ("Processo 1 esperando...\n");
        sem_wait(&sem3);
      
        printf("Processo 1 - Terminou \n");

        exit(0);
    }

    pid_t pid_2 = fork();
    if (pid_2 == 0) {
        sem_wait(&sem2);
        read(canal_pai2[READ], input, sizeof(input));
        printf("Processo 2 - Mensagem recebida do pai: %s\n", input);
      
        strcpy(output, "Mensagem 2.");
      
        write(canal_pai2[WRITE], output, sizeof(output) + 1);
        sem_post(&sem3);

        exit(0);
    }

    pid_t pid_3 = fork();
    if (pid_3 == 0) {
        exit(0);
    }

    sem_wait(&sem1);

    read(canal_pai1[READ], input, sizeof(input));
    strcpy(output, input);
    write(canal_pai2[WRITE], output, sizeof(output) + 1);
    sem_post(&sem2);

    waitpid(pid_1, NULL, 0);
    waitpid(pid_2, NULL, 0);
    waitpid(pid_3, NULL, 0);
    printf ("Print final do pai.\n");

    return 0;
}
