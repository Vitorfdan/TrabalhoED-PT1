#ifndef BD_PARTIDAS_H
#define BD_PARTIDAS_H

#include "times.h"

struct BancoTimes;

#define LIMITE_JOGOS 100 

// Definicao da struct Partidas
typedef struct 
{
    int gols_time1;
    int gols_time2;
    int time1; //inicialização das variáveis como int
    int time2;
    int id;
} Partidas;

typedef struct BD_Partidas
{
    Partidas *partidas[LIMITE_JOGOS];
    int qntd;
} BD_Partidas;

// Função para consultar os jogos
void buscarPartidas(BD_Partidas *dados_partidas, struct BancoTimes *dados_times);

// Apresenta opcoes de consulta
void mostrarOpcoesDeBusca();

// Mostra tela principal
void exibirTelaInicial();

// Registra novo jogo no vetor
void inserirNovaPartida(BD_Partidas *bd, int id, int time1, int time2, int gols_time1, int gols_time2);

// Carrega arquivo
void importarDadosArquivo(const char *bd_partidas, BD_Partidas *dados);

// Libera memória alocada dinamicamente
void liberarAlocacaoDados(BD_Partidas *bd);

// Inicializa as partidas no vetor dinamico
BD_Partidas *inicializarBanco();

// Cria partida
Partidas *construirPartida(int codigo, int equipe_a, int equipe_b, int placar_a, int placar_b);

// Verifica time na partida
int verificarTimeNaPartida(const char *nome_busca, Partidas *jogo, struct BancoTimes *dados, int tipo);

#endif