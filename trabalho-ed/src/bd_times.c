#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "times.h"
#include "bd_partidas.h"
#include "bd_times.h"

void liberarAlocacaoTimes(BancoTimes *bd) { // Desaloca memoria dos times (alocada dinamicamente)
    int quantidade = bd->qntd;
    while (quantidade > 0) {
        quantidade--;
        Times *time_atual = bd->times[quantidade];
        free(time_atual);
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
    
    return nova_estrutura;
}

// Insere time no banco/vetor
void inserirTime(BancoTimes *bd, int id, const char *nome_do_time){
    int espaco_disponivel = LIMITE_TIMES - bd->qntd;
    
    if (espaco_disponivel <= 0){
        printf("Limite de times atingido\n");
        exit(1);
    }
    
    Times *novo_time = construirTime(id, nome_do_time);
    int posicao_insercao = bd->qntd;
    bd->times[posicao_insercao] = novo_time;
    
    int *ptr_quantidade = &(bd->qntd);
    *ptr_quantidade = *ptr_quantidade + 1;
}

// Le o arquivo de times
void importarTimes(const char *bd_times, BancoTimes *dados){
    FILE *arq = fopen(bd_times, "r");
    
    if (arq == NULL){
        printf("Arquivo %s nao encontrado\n", bd_times);
        exit(1);
    }

    char buffer[LIMITE_TIMES];
    int info[2];
    char nome_equipe[TAMANHO_NOME_TIME];

    fgets(buffer, sizeof(buffer), arq);
    dados->qntd = 0;

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

// atualiza estatisticas dos times no vetor
void processarEstatisticas(BD_Partidas *dados_das_partidas, BancoTimes *dados_dos_times){
    int indice = 0;
    int limite = dados_das_partidas->qntd;
    
    while (indice < limite) {
        Partidas *jogo = dados_das_partidas->partidas[indice];
        
        Times *time_casa = dados_dos_times->times[jogo->time1];
        Times *time_fora = dados_dos_times->times[jogo->time2];

        int gols_mandante = jogo->gols_time1;
        int gols_visitante = jogo->gols_time2;
        
        // Soma os gols marcados e sofridos pelo time
        int *ptr_gols_marcados_casa = &(time_casa->gols_marcados);
        *ptr_gols_marcados_casa = *ptr_gols_marcados_casa + gols_mandante;
        
        int valor_atual_sofridos = time_casa->gols_sofridos;
        time_casa->gols_sofridos = valor_atual_sofridos + gols_visitante;
        
        time_fora->gols_marcados = time_fora->gols_marcados + gols_visitante;
        time_fora->gols_sofridos = time_fora->gols_sofridos + gols_mandante;

        // Verifica se a partida deu empate
        if (gols_mandante == gols_visitante) {
            int empates_casa = time_casa->empates;
            int empates_fora = time_fora->empates;
            time_casa->empates = empates_casa + 1;
            time_fora->empates = empates_fora + 1;
        } else {
            definirVencedor(jogo, time_casa, time_fora);
        }

        // Atualiza pontuacao dos times da partida
        int vitorias_casa = time_casa->vitorias;
        int empates_calc_casa = time_casa->empates;
        time_casa->pontuacao = (vitorias_casa * 3) + empates_calc_casa;
        
        int marcados_casa = time_casa->gols_marcados;
        int sofridos_casa = time_casa->gols_sofridos;
        time_casa->saldo_de_gols = marcados_casa - sofridos_casa;
        
        int vitorias_fora = time_fora->vitorias;
        int empates_calc_fora = time_fora->empates;
        time_fora->pontuacao = (vitorias_fora * 3) + empates_calc_fora;
        
        int marcados_fora = time_fora->gols_marcados;
        int sofridos_fora = time_fora->gols_sofridos;
        time_fora->saldo_de_gols = marcados_fora - sofridos_fora;
        
        indice++;
    }
}

// Exibe tabela de classificacao
void mostrarTabelaClassificacao(BancoTimes *dados_times){
    printf("\n+----+----------------+---+---+---+----+----+-----+----+\n");
    printf("| ID | Time           | V | E | D | GM | GS | S   | PG |\n");
    printf("+----+----------------+---+---+---+----+----+-----+----+\n");
    
    int i = 0;
    int total_times = dados_times->qntd;
    
    while (i < total_times) {
        Times *time_atual = dados_times->times[i];
        exibirTime(time_atual);
        i++;
    }
    
    printf("+----+----------------+---+---+---+----+----+-----+----+\n");
}

// Busca times por nome
void buscarTimes(BancoTimes *dados){
    char termo[LIMITE_TIMES];
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

    int idx = 0;
    while (idx < dados->qntd) {
        Times *time_verificado = dados->times[idx];
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
        
        idx++;
    }
    
    printf("+----+----------------+---+---+---+----+----+-----+----+\n");

    if (quantidade_encontrada == 0) {
        printf("\nNenhum time localizado.\n");
    }
}
