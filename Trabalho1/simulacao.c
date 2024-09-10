#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>

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
double min(double n1, double n2){

    if(n1 < n2) return n1;
    return n2;

}

void inicia_little(little *n){
    n->num_eventos = 0;
    n->soma_areas = 0.0;
    n->tempo_anterior = 0.0;
}

int main(){
    srand(771327);
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
    double marcador = 100.0;

    little en;
    little ew_chegadas;
    little ew_saidas;

    inicia_little(&ew_chegadas);
    inicia_little(&ew_saidas);
    inicia_little(&en);


    while(tempo_decorrido <= tempo_simulacao){
        tempo_decorrido = min(tempo_chegada,tempo_saida);
        printf("Tempo Decorrido = %f\n", tempo_decorrido);
        // chegando no sistema
        if(tempo_decorrido == marcador){
        marcador += 100
        
        
        }
        
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

            en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.num_eventos;
            en.num_eventos--;
            en.tempo_anterior = tempo_decorrido;

            ew_saidas.soma_areas += (tempo_decorrido - ew_saidas.tempo_anterior) * ew_saidas.num_eventos;
            ew_saidas.num_eventos++;
            ew_saidas.tempo_anterior = tempo_decorrido;

            
        }
        

    }

	//tranformar isso numa funcao
    //en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.num_eventos;
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





    return 0;
}
