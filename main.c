#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h>

#define READ 0
#define WRITE 1

int main(void) {
    int canal_1[2], canal_2[2], canal_3[2];
    int canal_pai1[2], canal_pai2[2], canal_pai3[2];

    char input[256], output[256];



    pipe(canal_1);
    pipe(canal_2);
    pipe(canal_3); 
    pipe(canal_pai1);
    pipe(canal_pai2);
    pipe(canal_pai3);

    pid_t pid_1 = fork();
    if (pid_1 == 0) {
      
        close(canal_pai1[READ]);
        close(canal_1[WRITE]);
        printf ("Processo 1 - Enviando msg pro pai...\n");
        
        strcpy(output, "Mensagem 1.");
      
        write(canal_pai1[WRITE], output, sizeof(output)+1);
      
        
      
      
        char mensagem[256];
        ssize_t t;
        while ((t = read(canal_1[READ], mensagem, sizeof(mensagem)+1)) <= 0){
          printf ("P1: Esperando msg do Pai...\n");
          
        }
      
      printf ("P1: Mensagem recebida do pai: %s\n", mensagem);
      
        exit(0);
    }

    pid_t pid_2 = fork();
    if (pid_2 == 0) {
        close(canal_pai2[READ]);
        close(canal_2[WRITE]);
      
        char mensagem[256];
        ssize_t t;
      
        while ((t = read(canal_2[READ], mensagem, sizeof(mensagem) +1)) <= 0){
        printf ("P2: Esperando msg do Pai...\n");
      }
      
        printf ("P2: Mensagem recebida do pai: %s\n", mensagem);


      printf ("Processo 2 - Enviando msg pro pai...\n");

      
      strcpy(output, "Mensagem 2.\n");
      write(canal_pai2[WRITE], output, sizeof(output)+1);
      
        exit(0);
    }

    pid_t pid_3 = fork();
    if (pid_3 == 0) {

      
        close(canal_pai3[READ]);
        close(canal_3[WRITE]);
      
        char mensagem[256];
        ssize_t t;
      
        while ((t = read(canal_3[READ], mensagem, sizeof(mensagem) +1)) <= 0){
        printf ("P3: Esperando msg do Pai...\n");
      }
      
      printf ("P3: Mensagem recebida do pai: %s\n", mensagem);
      
      printf ("Processo 3 - Enviando msg pro pai...\n");

      strcpy(output, "Mensagem 3.\n");
      write(canal_pai3[WRITE], output, sizeof(output)+1);
      
        exit(0);
    }
  
    close(canal_pai1[WRITE]);
    close(canal_1[READ]);
    
    
    char mensagem[256];
    ssize_t t;
    while ((t = read(canal_pai1[READ], mensagem, sizeof(mensagem) +1)) <= 0){
      printf ("PAI: Esperando msg do P1...\n");
      }

  
    close(canal_pai2[WRITE]);
    close(canal_2[READ]);
  
    strcpy(output, mensagem);
  
    write(canal_2[WRITE], output, sizeof(output) + 1);


    while ((t = read(canal_pai2[READ], mensagem, sizeof(mensagem) +1)) <= 0){
      printf ("PAI: Esperando msg do P2...\n");
      }

    close(canal_pai3[WRITE]);
    close(canal_3[READ]);
  
    strcpy(output, mensagem);
  
    write(canal_3[WRITE], output, sizeof(output) + 1);

    while ((t = read(canal_pai3[READ], mensagem, sizeof(mensagem) +1)) <= 0){
      printf ("PAI: Esperando msg do P3...\n");
      }

  
    strcpy(output, mensagem);
    write(canal_1[WRITE], output, sizeof(output) + 1);
  

    waitpid(pid_1, NULL, 0);
    waitpid(pid_2, NULL, 0);
    waitpid(pid_3, NULL, 0);
    printf ("Print final do pai.\n");

    return 0;
}
