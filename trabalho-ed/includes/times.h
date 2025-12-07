#ifndef TIMES_H
#define TIMES_H

#define TAMANHO_NOME_TIME 30//define o nome do time com máximo de caracteres


typedef struct //Definição da estrutura times
{
    char nome_do_time[TAMANHO_NOME_TIME];
    int gols_marcados;
    int gols_sofridos;
    int vitorias;
    int empates;
    int derrotas;
    int pontuacao;
    int saldo_de_gols;
    int ID;
} Times;

// Adiciona gols sofridos pela equipe
void somarGolsSofridos(Times *equipe, int quantidade);

// Adiciona gols marcados pela equipe
void somarGolsMarcados(Times *equipe, int quantidade);

// Calcula pontos das partidas jogadas
void atualizarPontuacao(Times *equipe);

// Verifica prefixo do nome do time
int conferePrefixoTime(const char *nome_equipe, const char *busca);

// Retorna saldo de gols das partidas
int saldoDeGols(Times *time, int id);

// Retorna pontos conquistados nas partidas
int pontuacaoTime(Times *time, int id);

// Mostra informacoes do time
void exibirTime(Times *equipe);

// Cria time no vetor
Times *construirTime(int codigo, const char *nome);

#endif