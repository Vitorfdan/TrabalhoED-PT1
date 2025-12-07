#ifndef BD_TIMES_H
#define BD_TIMES_H

#include "times.h"
#include "bd_partidas.h"

// Nó da lista encadeada de times
typedef struct NoTime {
    Times *time;
    struct NoTime *proximo;
} NoTime;

typedef struct BancoTimes
{
    int qntd;
    NoTime *primeiro;
    NoTime *ultimo;
} BancoTimes;

// Busca time por prefixo ou nome
void buscarTimes(BancoTimes *dados);

// Exibe tabela da clasificação
void mostrarTabelaClassificacao(BancoTimes *dados_times);

// Processa/atualiza estatisticas dos times
void processarEstatisticas(BD_Partidas *dados_das_partidas, BancoTimes *dados_dos_times);

// libera memoria alocada dinamicamente 
void liberarAlocacaoTimes(BancoTimes *bd);

// Cria banco de times
BancoTimes *inicializarBancoTimes();

// Verifica existencia do time
int existeTime(BancoTimes *bd, char *nome_do_time);

// Busca time por ID
Times* buscarTimePorID(BancoTimes *bd, int id);

// Adiciona time no vetor
void inserirTime(BancoTimes *bd, int id, const char *nome_do_time);

// Carrega arquivo de times
void importarTimes(const char *bd_times, BancoTimes *dados);

// Define vencedor da partida
void definirVencedor(Partidas *jogo, Times *equipe1, Times *equipe2);

// Zera estatísticas dos times
void zerarEstatisticasTimes(BancoTimes *banco);

#endif