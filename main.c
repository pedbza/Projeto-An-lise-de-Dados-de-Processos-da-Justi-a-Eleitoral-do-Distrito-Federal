#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "processo.h"

#define MAX_PROCESSOS 1000
#define MAX_LINE 256
#define MAX_VALORES 10

void exibir_menu() {
    printf("\n=== Menu de Operacoes ===\n");
    printf("1. Ordenar processos por ID (crescente)\n");
    printf("2. Ordenar processos por data de ajuizamento (decrescente)\n");
    printf("3. Contar processos por ID de classe\n");
    printf("4. Contar assuntos distintos\n");
    printf("5. Listar processos com mais de um assunto\n");
    printf("6. Calcular dias em tramitacao\n");
    printf("7. Sair\n");
    printf("Escolha uma opcao (1-7): ");
}

int parse_multivalor(const char* str, int* valores) {
    char temp[256];
    strncpy(temp, str, 255);
    temp[255] = '\0';
    
    char* src = temp;
    char* dst = temp;
    while (*src) {
        if (*src != '"') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
    
    char* start = temp;
    if (*start == '{') start++;
    
    char* end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) end--;
    if (*end == '}') *end = '\0';
    
    int count = 0;
    char* token = strtok(start, ",");
    while (token && count < MAX_VALORES) {
        while (*token && isspace(*token)) token++;
        char* token_end = token + strlen(token) - 1;
        while (token_end > token && isspace(*token_end)) *token_end-- = '\0';
        
        if (*token) {
            valores[count++] = atoi(token);
        }
        token = strtok(NULL, ",");
    }
    
    return count;
}

int main() {
    Processo* processos[MAX_PROCESSOS];
    int n = 0;
    FILE* fp = fopen("processo_043_202409032338.csv", "r");
    if (!fp) {
        printf("Erro ao abrir o arquivo de entrada!\n");
        return 1;
    }

    char linha[MAX_LINE];
    fgets(linha, MAX_LINE, fp);
    while (fgets(linha, MAX_LINE, fp) && n < MAX_PROCESSOS) {
        char field1[100], field2[100], field3[100], field4[256], field5[256], field6[100];
        
        int field_start = 0;
        int line_pos = 0;
        int inside_quotes = 0;
        int field_num = 0;
        char* current_field = NULL;
        
        while (linha[line_pos] != '\0' && field_num < 6) {
            switch (field_num) {
                case 0: current_field = field1; break;
                case 1: current_field = field2; break;
                case 2: current_field = field3; break;
                case 3: current_field = field4; break;
                case 4: current_field = field5; break;
                case 5: current_field = field6; break;
            }
            
            int max_size = (field_num == 3 || field_num == 4) ? 255 : 99;
            
            if (linha[line_pos] == '"') {
                inside_quotes = !inside_quotes;
            } else if (linha[line_pos] == ',' && !inside_quotes) {
                strncpy(current_field, linha + field_start, line_pos - field_start);
                current_field[line_pos - field_start] = '\0';
                field_start = line_pos + 1;
                field_num++;
            }
            
            line_pos++;
        }
        
        if (field_num == 5) {
            strncpy(field6, linha + field_start, 99);
            field6[99] = '\0';
        }
        
        long long id = atoll(field1);

        char numero[21];
        char* num_start = field2;
        if (*num_start == '"') num_start++;
        strncpy(numero, num_start, 20);
        numero[20] = '\0';
        size_t len = strlen(numero);
        if (len > 0 && numero[len - 1] == '"') numero[len - 1] = '\0';

        char data[25];
        strncpy(data, field3, 24);
        data[24] = '\0';

        int id_classe[MAX_VALORES] = {0};
        int num_classes = parse_multivalor(field4, id_classe);

        int id_assunto[MAX_VALORES] = {0};
        int num_assuntos = parse_multivalor(field5, id_assunto);

        char* ano_str = field6;
        while (*ano_str && isspace(*ano_str)) ano_str++;
        char* ano_end = ano_str + strlen(ano_str) - 1;
        while (ano_end > ano_str && isspace(*ano_end)) *ano_end-- = '\0';
        int ano_eleicao = atoi(ano_str);

        processos[n] = criar_processo(id, numero, data, id_classe, num_classes, id_assunto, num_assuntos, ano_eleicao);
        n++;
    }
    fclose(fp);

    int opcao;
    do {
        exibir_menu();
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao) {
            case 1:
                ordenar_por_id(processos, n, "ordenado_id.csv");
                printf("Processos ordenados por ID e salvos em 'ordenado_id.csv'.\n");
                break;
            case 2:
                ordenar_por_data_decrescente(processos, n, "ordenado_data.csv");
                printf("Processos ordenados por data (decrescente) e salvos em 'ordenado_data.csv'.\n");
                break;
            case 3: {
                int id_classe;
                printf("Digite o ID da classe para contar processos: ");
                scanf("%d", &id_classe);
                int count = contar_processos_por_classe(processos, n, id_classe);
                printf("Processos com id_classe %d: %d\n", id_classe, count);
                
                listar_processos_por_classe(processos, n, id_classe);
                break;
            }
            case 4: {
                int count = contar_assuntos_distintos(processos, n);
                printf("Quantidade de assuntos distintos: %d\n", count);
                break;
            }
            case 5:
                listar_processos_multi_assunto(processos, n);
                break;
            case 6:
                calcular_dias_tramitacao(processos, n);
                break;
            case 7:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 7);

    for (int i = 0; i < n; i++) {
        destruir_processo(processos[i]);
    }
    return 0;
}