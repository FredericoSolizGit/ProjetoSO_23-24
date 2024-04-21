#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>


void printProgressBar(int totalPoints, int verifiedPoints) {
    // Calcula a porcentagem de progresso
    float progress = (float)verifiedPoints / totalPoints * 100;

    // Imprime a barra de progresso
    printf("\rProgresso: [");
    int pos;
    for (pos = 0; pos < 50; ++pos) {
        if (pos < progress / 2)
            printf("=");
        else if (pos == progress / 2)
            printf(">");
        else
            printf(" ");
    }
    printf("] %.2f%%", progress);
    fflush(stdout); // Força a saída para ser exibida imediatamente
}

int requisito04() {
    int totalPoints = 1000000;  // Número total de pontos aleatórios gerados
    int verifiedPoints = 0;      // Número total de pontos verificados por todos os processos filho

    // Simula o processo de verificação de pontos
    while (verifiedPoints < totalPoints) {
        // Suponha que a cada iteração, um número aleatório de pontos seja verificado
        int randomVerified = rand() % 1000 + 1; // Simula verificação de 1 a 1000 pontos
        verifiedPoints += randomVerified;

        // Imprime e atualiza a barra de progresso
        printProgressBar(totalPoints, verifiedPoints);

        // Espera um curto período de tempo para simular o progresso
        usleep(100000); // Espera por 100ms
    }

    printf("\nVerificação concluída!\n");

    return 0;
}
