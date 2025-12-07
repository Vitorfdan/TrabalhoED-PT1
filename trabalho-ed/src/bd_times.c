#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "times.h"
#include "bd_partidas.h"
#include "bd_times.h"

void liberarAlocacaoTimes(BancoTimes *bd) { // Desaloca memoria dos times (alocada dinamicamente)
    NoTime *atual = bd->primeiro;
    
    while (atual != NULL) {
        NoTime *proximo = atual->proximo;
        free(atual->time);
        free(atual);
        atual = proximo;
    }
    
    free(bd);
}

// Aloca memoria dinamicamente da estrutura de times
BancoTimes *inicializarBancoTimes() {
    BancoTimes *nova_estrutura = malloc(sizeof(BancoTimes));
    int falhou = (nova_estrutura == NULL);
    
    if (falhou) {
        printf("Falha ao alocar estrutura\n");
        exit(1);
    }
    
    nova_estrutura->qntd = 0;
    nova_estrutura->primeiro = NULL;
    nova_estrutura->ultimo = NULL;
    
    return nova_estrutura;
}

// Insere time no banco usando lista encadeada
void inserirTime(BancoTimes *bd, int id, const char *nome_do_time){
    // Cria o novo time
    Times *novo_time = construirTime(id, nome_do_time);
    
    // Cria o novo nó
    NoTime *novo_no = malloc(sizeof(NoTime));
    if (novo_no == NULL) {
        printf("Falha ao alocar nó para time\n");
        exit(1);
    }
    
    novo_no->time = novo_time;
    novo_no->proximo = NULL;
    
    // Insere no final da lista
    if (bd->primeiro == NULL) {
        // Lista vazia
        bd->primeiro = novo_no;
        bd->ultimo = novo_no;
    } else {
        // Adiciona no final
        bd->ultimo->proximo = novo_no;
        bd->ultimo = novo_no;
    }
    
    bd->qntd++;
}

// Le o arquivo de times
void importarTimes(const char *bd_times, BancoTimes *dados){
    FILE *arq = fopen(bd_times, "r");
    
    if (arq == NULL){
        printf("Arquivo %s nao encontrado\n", bd_times);
        exit(1);
    }

    char buffer[256];
    int info[2];
    char nome_equipe[TAMANHO_NOME_TIME];

    fgets(buffer, sizeof(buffer), arq);

    int continuar = 1;
    while (continuar != 0) {
        char *linha_lida = fgets(buffer, sizeof(buffer), arq);
        
        if (linha_lida == NULL) {
            continuar = 0;
            continue;
        }
        
        int resultado_scan = sscanf(buffer, "%d,%99[^\n]", &info[0], nome_equipe);
        
        if (resultado_scan == 2) {
            inserirTime(dados, info[0], nome_equipe);
        }
    }

    fclose(arq);
}

//Define o vencedor da partida baseado nos gols
void definirVencedor(Partidas *jogo, Times *equipe1, Times *equipe2){ 
    int diferenca_gols = jogo->gols_time1 - jogo->gols_time2;
    
    if (diferenca_gols > 0){
        int vitorias_atual = equipe1->vitorias;
        int derrotas_atual = equipe2->derrotas;
        equipe1->vitorias = vitorias_atual + 1;
        equipe2->derrotas = derrotas_atual + 1;
    } else {
        int vitorias_visitante = equipe2->vitorias;
        int derrotas_mandante = equipe1->derrotas;
        equipe2->vitorias = vitorias_visitante + 1;
        equipe1->derrotas = derrotas_mandante + 1;
    }
}

// Busca time por ID na lista encadeada
Times* buscarTimePorID(BancoTimes *bd, int id) {
    NoTime *atual = bd->primeiro;
    
    while (atual != NULL) {
        if (atual->time->ID == id) {
            return atual->time;
        }
        atual = atual->proximo;
    }
    
    return NULL;
}

// atualiza estatisticas dos times no vetor
void processarEstatisticas(BD_Partidas *dados_das_partidas, BancoTimes *dados_dos_times){
    int indice = 0;
    int limite = dados_das_partidas->qntd;
    
    while (indice < limite) {
        Partidas *jogo = dados_das_partidas->partidas[indice];
        
        Times *time_casa = buscarTimePorID(dados_dos_times, jogo->time1);
        Times *time_fora = buscarTimePorID(dados_dos_times, jogo->time2);

        int gols_mandante = jogo->gols_time1;
        int gols_visitante = jogo->gols_time2;
        
        // Soma os gols marcados e sofridos pelo time
        time_casa->gols_marcados += gols_mandante;
        time_casa->gols_sofridos += gols_visitante;
        
        time_fora->gols_marcados += gols_visitante;
        time_fora->gols_sofridos += gols_mandante;

        // Verifica se a partida deu empate
        if (gols_mandante == gols_visitante) {
            time_casa->empates++;
            time_fora->empates++;
        } else {
            definirVencedor(jogo, time_casa, time_fora);
        }
        
        indice++;
    }
    
    // Após processar todas as partidas, calcula pontuação e saldo de gols para todos os times
    NoTime *atual = dados_dos_times->primeiro;
    while (atual != NULL) {
        Times *time = atual->time;
        time->pontuacao = (time->vitorias * 3) + time->empates;
        time->saldo_de_gols = time->gols_marcados - time->gols_sofridos;
        atual = atual->proximo;
    }
}

// Compara dois times para ordenação (critérios: pontos, saldo de gols, gols marcados)
int compararTimes(Times *time1, Times *time2) {
    // Primeiro critério: pontuação (maior é melhor)
    if (time1->pontuacao != time2->pontuacao) {
        return time1->pontuacao - time2->pontuacao;
    }
    
    // Segundo critério: saldo de gols (maior é melhor)
    if (time1->saldo_de_gols != time2->saldo_de_gols) {
        return time1->saldo_de_gols - time2->saldo_de_gols;
    }
    
    // Terceiro critério: gols marcados (maior é melhor)
    return time1->gols_marcados - time2->gols_marcados;
}

// Exibe tabela de classificacao
void mostrarTabelaClassificacao(BancoTimes *dados_times){
    // Cria array temporário para ordenação
    Times **times_ordenados = malloc(dados_times->qntd * sizeof(Times*));
    if (times_ordenados == NULL) {
        printf("Erro ao alocar memória para ordenação\n");
        return;
    }
    
    // Copia ponteiros da lista para o array
    NoTime *atual = dados_times->primeiro;
    int i = 0;
    while (atual != NULL) {
        times_ordenados[i] = atual->time;
        i++;
        atual = atual->proximo;
    }
    
    // Ordena usando bubble sort
    for (int i = 0; i < dados_times->qntd - 1; i++) {
        for (int j = 0; j < dados_times->qntd - i - 1; j++) {
            if (compararTimes(times_ordenados[j], times_ordenados[j + 1]) < 0) {
                Times *temp = times_ordenados[j];
                times_ordenados[j] = times_ordenados[j + 1];
                times_ordenados[j + 1] = temp;
            }
        }
    }
    
    printf("\n+----+----------------+---+---+---+----+----+-----+----+\n");
    printf("| ID | Time           | V | E | D | GM | GS | S   | PG |\n");
    printf("+----+----------------+---+---+---+----+----+-----+----+\n");
    
    for (int i = 0; i < dados_times->qntd; i++) {
        exibirTime(times_ordenados[i]);
    }
    
    printf("+----+----------------+---+---+---+----+----+-----+----+\n");
    
    free(times_ordenados);
}

// Busca times por nome
void buscarTimes(BancoTimes *dados){
    char termo[256];
    int quantidade_encontrada = 0;
    
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("Digite o nome ou prefixo do time: ");
    scanf("%s", termo);
    
    printf("\n+----+----------------+---+---+---+----+----+-----+----+\n");
    printf("| ID | Time           | V | E | D | GM | GS | S   | PG |\n");
    printf("+----+----------------+---+---+---+----+----+-----+----+\n");

    NoTime *atual = dados->primeiro;
    while (atual != NULL) {
        Times *time_verificado = atual->time;
        char *nome_time = time_verificado->nome_do_time;
        
        int posicao = 0;
        int corresponde = 1;
        
        while (termo[posicao] != '\0' && corresponde == 1) {
            if (nome_time[posicao] != termo[posicao]) {
                corresponde = 0;
            }
            posicao++;
        }
        
        if (corresponde != 0) {
            exibirTime(time_verificado);
            quantidade_encontrada++;
        }
        
        atual = atual->proximo;
    }
    
    printf("+----+----------------+---+---+---+----+----+-----+----+\n");

    if (quantidade_encontrada == 0) {
        printf("\nNenhum time localizado.\n");
    }
}

// Zera todas as estatísticas dos times
void zerarEstatisticasTimes(BancoTimes *banco) {
    NoTime *atual = banco->primeiro;
    
    while (atual != NULL) {
        atual->time->pontuacao = 0;
        atual->time->vitorias = 0;
        atual->time->empates = 0;
        atual->time->derrotas = 0;
        atual->time->gols_marcados = 0;
        atual->time->gols_sofridos = 0;
        atual->time->saldo_de_gols = 0;
        
        atual = atual->proximo;
    }
}
