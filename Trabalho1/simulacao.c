#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>


double en_final;
double ew_final;
double lambda;
double erro_little;

// Abrir o arquivo CSV para escrita
FILE *fp;

void inicia_arqv(){  
    fp = fopen("dados_simulador.csv", "w"); 
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
    }

    // Escreva o cabecalho do arquivo CSV (opcional, mas recomendado)
    fprintf(fp, "Marcador,Ocupação,E[N],E[W],Erro,Lambda\n");

    // Fechar o arquivo
    fclose(fp);

    printf("Arquivo CSV criado com sucesso.\n");

}

void escreve(double marcador, double ocupacao, double en, double ew, double erro, double lambda){

    fp = fopen("dados_simulador.csv", "a");

    if (fp == NULL) {
            printf("Erro ao abrir o arquivo.\n");
        }

    // Simulando dados (aqui como exemplo estamos apenas gerando números simples)
        int tempo = 1;
        float valor1 = 1.5;
        float valor2 = 2.5;

        // Escrever os dados no arquivo CSV
        fprintf(fp, "%.2f,%.4f,%.4f,%.4f,%.4f,%.4f\n",marcador, ocupacao, en, ew, erro, lambda);

        // Fechar o arquivo após cada iteração
        fclose(fp);

    
}

typedef struct {
    unsigned long int num_eventos;
    double tempo_anterior;
    double soma_areas;
} little;

double uniforme(){
    double u = rand() / ((double) RAND_MAX + 1);
    u = 1.0 - u;
    return u;

}

double gera_tempo(double parametro_chegada){

    return (-1.0/parametro_chegada) * log(uniforme());

}

// arrumar pra fazer o minimo entre 3 numeros
double min(double n1, double n2, double n3){

    if(n1 < n2 && n1 < n3){
        return n1;
    }else{
        if(n2 < n3){
            return n2;
        }
    }
    return n3;

}

void inicia_little(little *n){
    n->num_eventos = 0;
    n->soma_areas = 0.0;
    n->tempo_anterior = 0.0;
}

int main(){
    inicia_arqv();
    srand(time(NULL));
    double parametro_chegada = 1.0/10;
    // printf("Informe o tempo medio de chegada: ");
    // scanf("%lF", &parametro_chegada);
    // parametro_chegada = 1.0/parametro_chegada;

    double parametro_saida = 1.0/8.5;
    // printf("Informe o tempo medio de atendimento: ");
    // scanf("%lF", &parametro_saida);
    // parametro_saida = 1.0/parametro_saida;

    double tempo_simulacao = 100000;
    // printf("informe o tempo de simulacao: ");
    // scanf("%lF",&tempo_simulacao);

    double tempo_decorrido = 0.0;
    double tempo_chegada = gera_tempo(1);
    double tempo_saida = DBL_MAX;
    double tempo_aux;

    unsigned long int fila = 0;
    unsigned long int fila_max = 0;

    double soma_ocupacao = 0.0;
    double marcador = 100.0;

    little en;
    little ew_chegadas;
    little ew_saidas;

    inicia_little(&ew_chegadas);
    inicia_little(&ew_saidas);
    inicia_little(&en);

    while(tempo_decorrido <= tempo_simulacao){
        // Adicionando o marcador na funcao minimo
        tempo_decorrido = min(tempo_chegada,tempo_saida, marcador);
        // Evento da coleta
        if(tempo_decorrido == marcador){

            printf("Marcador: %f\n", marcador/100);
            en_final = en.soma_areas/tempo_decorrido;
            ew_final = (ew_chegadas.soma_areas - ew_saidas.soma_areas) / ew_chegadas.num_eventos;
            lambda = ew_chegadas.num_eventos/tempo_decorrido;
            erro_little = en_final - (lambda*ew_final);

            printf("Tamanho de ocupacao: %lF\n",soma_ocupacao/tempo_decorrido);
            printf("E[N]: %lF\n",en_final);
            printf("E[W]: %lF\n",ew_final);
            printf("Erro de Little: %lF\n", erro_little);
            printf("Lambda: %lF\n", lambda);
            escreve(marcador/100,soma_ocupacao/tempo_decorrido,en_final,ew_final,erro_little,lambda);

            marcador += 100;
        
        }
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

            en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.num_eventos;
            en.num_eventos++;
            en.tempo_anterior = tempo_decorrido;

            ew_chegadas.soma_areas += (tempo_decorrido - ew_chegadas.tempo_anterior) * ew_chegadas.num_eventos;
            ew_chegadas.num_eventos++;
            ew_chegadas.tempo_anterior = tempo_decorrido;

        }
        // saindo do sistema
        if(tempo_decorrido == tempo_saida){
            fila--; // o individuo saiu do banco
            tempo_saida = DBL_MAX;  
            // discreto, n simula eventos q n interessa, ent vai pular p prox evento
            if(fila){
                tempo_saida = tempo_decorrido + gera_tempo(parametro_saida);
                
                soma_ocupacao += tempo_saida - tempo_decorrido;
            }//tinha um else aq

            en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.num_eventos;
            en.num_eventos--;
            en.tempo_anterior = tempo_decorrido;

            ew_saidas.soma_areas += (tempo_decorrido - ew_saidas.tempo_anterior) * ew_saidas.num_eventos;
            ew_saidas.num_eventos++;
            ew_saidas.tempo_anterior = tempo_decorrido;

        }
             
        }

	//tranformar isso numa funcao
    en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.num_eventos;
    ew_chegadas.soma_areas += (tempo_decorrido - ew_chegadas.tempo_anterior) * ew_chegadas.num_eventos;
    ew_saidas.soma_areas += (tempo_decorrido - ew_saidas.tempo_anterior) * ew_saidas.num_eventos;

    printf("Tamanho maximo da fila: %ld\n",fila_max);
    printf("Tamanho de ocupacao: %lF\n",soma_ocupacao/tempo_decorrido);

    double en_final = en.soma_areas/tempo_decorrido;
    double ew_final = (ew_chegadas.soma_areas - ew_saidas.soma_areas) / ew_chegadas.num_eventos;
    double lambda = ew_chegadas.num_eventos/tempo_decorrido;
    double erro_little = en_final - (lambda*ew_final);

    printf("E[N]: %lF\n",en_final);
    printf("E[W]: %lF\n",ew_final);
    printf("Erro de Little: %lF\n", erro_little);
    printf("Lambda: %lF\n", lambda);


    return 0;
}
