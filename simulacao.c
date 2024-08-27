#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>

double uniforme(){
    double u = rand() / ((double) RAND_MAX + 1);
    u = 1.0 - u;
    return u;

}

double gera_tempo(double parametro_chegada){

    return (-1.0/parametro_chegada) * log(uniforme());

}

double min(double n1, double n2){

    if(n1 < n2) return n1;
    return n2;

}

int main(){
    srand(time(NULL));
    double parametro_chegada;
    printf("Informe o tempo medio de chegada: ");
    scanf("%lF", &parametro_chegada);
    parametro_chegada = 1.0/parametro_chegada;

    double parametro_saida;
    printf("Informe o tempo medio de atendimento: ");
    scanf("%lF", &parametro_saida);
    parametro_saida = 1.0/parametro_saida;

    double tempo_simulacao;
    printf("informe o tempo de simulacao: ");
    scanf("%lF",&tempo_simulacao);

    double tempo_decorrido = 0.0;
    double tempo_chegada = gera_tempo(1);
    double tempo_saida = DBL_MAX;

    unsigned long int fila = 0;
    unsigned long int fila_max = 0;

    double soma_ocupacao = 0.0;

    while(tempo_decorrido <= tempo_simulacao){
        tempo_decorrido = min(tempo_chegada,tempo_saida);
        // chegando no sistema
        if(tempo_decorrido == tempo_chegada){
            if(!fila){
                tempo_saida = tempo_decorrido + gera_tempo(parametro_saida);

                soma_ocupacao += tempo_saida - tempo_decorrido;
            }
            fila++;
            if(fila > fila_max){
                fila_max = fila;
            }
            tempo_chegada = tempo_decorrido + gera_tempo(parametro_chegada);

        }else{
            // saindo do sistema
            fila--; // o individuo saiu do banco
            tempo_saida = DBL_MAX;  
            // discreto, n simula eventos q n interessa, ent vai pular p prox evento
            if(fila){
                tempo_saida = tempo_decorrido + gera_tempo(parametro_saida);
                
                
                soma_ocupacao += tempo_saida - tempo_decorrido;
            }else{
              
            }

            
        }
        // printf("Tamanho da fila: %ld",fila);
        

    }
    printf("Tamanho maximo da fila: %ld\n",fila_max);
    printf("Tamanho de ocupacao: %lF\n",soma_ocupacao/tempo_decorrido);



    return 0;
}