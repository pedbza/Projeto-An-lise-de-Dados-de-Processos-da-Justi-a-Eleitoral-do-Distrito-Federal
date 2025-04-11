#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processo.h"

Processo* criar_processo(long long id, const char* numero, const char* data, int* id_classe, int num_classes, int* id_assunto, int num_assuntos, int ano_eleicao) {
    Processo* p = (Processo*)malloc(sizeof(Processo));
    p->id = id;
    strncpy(p->numero, numero, 20);
    p->numero[20] = '\0';
    strncpy(p->data_ajuizamento, data, 24);
    p->data_ajuizamento[24] = '\0';
    p->num_classes = num_classes > MAX_VALORES ? MAX_VALORES : num_classes;
    p->num_assuntos = num_assuntos > MAX_VALORES ? MAX_VALORES : num_assuntos;
    for (int i = 0; i < p->num_classes; i++) p->id_classe[i] = id_classe[i];
    for (int i = 0; i < p->num_assuntos; i++) p->id_assunto[i] = id_assunto[i];
    p->ano_eleicao = ano_eleicao;
    return p;
}

void destruir_processo(Processo* p) {
    free(p);
}

void ordenar_por_id(Processo* processos[], int n, const char* arquivo_saida) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processos[j]->id > processos[j + 1]->id) {
                Processo* temp = processos[j];
                processos[j] = processos[j + 1];
                processos[j + 1] = temp;
            }
        }
    }
    FILE* fp = fopen(arquivo_saida, "w");
    fprintf(fp, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%lld,\"%s\",%s,{", processos[i]->id, processos[i]->numero, processos[i]->data_ajuizamento);
        for (int j = 0; j < processos[i]->num_classes; j++) {
            fprintf(fp, "%d%s", processos[i]->id_classe[j], j < processos[i]->num_classes - 1 ? "," : "");
        }
        fprintf(fp, "},{");
        for (int j = 0; j < processos[i]->num_assuntos; j++) {
            fprintf(fp, "%d%s", processos[i]->id_assunto[j], j < processos[i]->num_assuntos - 1 ? "," : "");
        }
        fprintf(fp, "},%d\n", processos[i]->ano_eleicao);
    }
    fclose(fp);
}

void ordenar_por_data_decrescente(Processo* processos[], int n, const char* arquivo_saida) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(processos[j]->data_ajuizamento, processos[j + 1]->data_ajuizamento) < 0) {
                Processo* temp = processos[j];
                processos[j] = processos[j + 1];
                processos[j + 1] = temp;
            }
        }
    }
    FILE* fp = fopen(arquivo_saida, "w");
    fprintf(fp, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%lld,\"%s\",%s,{", processos[i]->id, processos[i]->numero, processos[i]->data_ajuizamento);
        for (int j = 0; j < processos[i]->num_classes; j++) {
            fprintf(fp, "%d%s", processos[i]->id_classe[j], j < processos[i]->num_classes - 1 ? "," : "");
        }
        fprintf(fp, "},{");
        for (int j = 0; j < processos[i]->num_assuntos; j++) {
            fprintf(fp, "%d%s", processos[i]->id_assunto[j], j < processos[i]->num_assuntos - 1 ? "," : "");
        }
        fprintf(fp, "},%d\n", processos[i]->ano_eleicao);
    }
    fclose(fp);
}

int contar_processos_por_classe(Processo** processos, int n, int id_classe) {
    int count = 0;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i]->num_classes; j++) {
            if (processos[i]->id_classe[j] == id_classe) {
                count++;
                break;
            }
        }
    }
    
    return count;
}

int contar_assuntos_distintos(Processo* processos[], int n) {
    int assuntos[10000] = {0};
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i]->num_assuntos; j++) {
            if (!assuntos[processos[i]->id_assunto[j]]) {
                assuntos[processos[i]->id_assunto[j]] = 1;
                count++;
            }
        }
    }
    return count;
}

void listar_processos_multi_assunto(Processo* processos[], int n) {
    printf("Processos com mais de um assunto:\n");
    for (int i = 0; i < n; i++) {
        if (processos[i]->num_assuntos > 1) {
            printf("ID: %lld, Assuntos: {", processos[i]->id);
            for (int j = 0; j < processos[i]->num_assuntos; j++) {
                printf("%d%s", processos[i]->id_assunto[j], j < processos[i]->num_assuntos - 1 ? "," : "");
            }
            printf("}\n");
        }
    }
}

void calcular_dias_tramitacao(Processo* processos[], int n) {
    int dia_atual = 7, mes_atual = 4, ano_atual = 2025;
    for (int i = 0; i < n; i++) {
        int dia, mes, ano;
        sscanf(processos[i]->data_ajuizamento, "%d-%d-%d", &ano, &mes, &dia);
        int dias = (ano_atual - ano) * 365 + (mes_atual - mes) * 30 + (dia_atual - dia);
        printf("Processo %lld: %d dias em tramitacao\n", processos[i]->id, dias);
    }
}

void listar_processos_por_classe(Processo** processos, int n, int id_classe) {
    printf("Processos com id_classe %d:\n", id_classe);
    int found = 0;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i]->num_classes; j++) {
            if (processos[i]->id_classe[j] == id_classe) {
                printf("ID: %lld, Numero: %s\n", processos[i]->id, processos[i]->numero);
                printf("  Classes: {");
                for (int k = 0; k < processos[i]->num_classes; k++) {
                    printf("%d", processos[i]->id_classe[k]);
                    if (k < processos[i]->num_classes - 1) printf(", ");
                }
                printf("}\n");
                found++;
                break;
            }
        }
    }
    
    printf("Total de processos encontrados: %d\n", found);
}