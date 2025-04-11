#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_PROCESSOS 1000
#define MAX_LINE 256
#define MAX_VALORES 10 

typedef struct {
    long long id;
    char numero[21];
    char data_ajuizamento[25];
    int id_classe[MAX_VALORES];
    int num_classes; 
    int id_assunto[MAX_VALORES];
    int num_assuntos; 
    int ano_eleicao;
} Processo;

Processo* criar_processo(long long id, const char* numero, const char* data, int* id_classe, int num_classes, int* id_assunto, int num_assuntos, int ano_eleicao);
void destruir_processo(Processo* p);

void ordenar_por_id(Processo* processos[], int n, const char* arquivo_saida);
void ordenar_por_data_decrescente(Processo* processos[], int n, const char* arquivo_saida);
int contar_processos_por_classe(Processo* processos[], int n, int id_classe);
int contar_assuntos_distintos(Processo* processos[], int n);
void listar_processos_multi_assunto(Processo* processos[], int n);
void calcular_dias_tramitacao(Processo* processos[], int n);
void listar_processos_por_classe(Processo** processos, int n, int id_classe);

#endif