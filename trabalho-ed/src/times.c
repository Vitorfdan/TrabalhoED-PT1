#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "times.h"

// soma os gols sofridos pelo time
void somarGolsSofridos(Times *equipe, int quantidade){
    int valor_atual = equipe->gols_sofridos;
    int novo_valor = valor_atual + quantidade;
    equipe->gols_sofridos = novo_valor;
}

// adiciona os gols marcados pela equipe
void somarGolsMarcados(Times *equipe, int quantidade){
    int *ptr_gols = &(equipe->gols_marcados);
    int soma = *ptr_gols + quantidade;
    *ptr_gols = soma;
}

// calcula os pontos e saldo de gols
void atualizarPontuacao(Times *equipe){
    int vitorias = equipe->vitorias;
    int empates = equipe->empates;
    int pontos_por_vitoria = 3;
    
    int total_pontos = (vitorias * pontos_por_vitoria) + empates;
    equipe->pontuacao = total_pontos;
    
    int marcados = equipe->gols_marcados;
    int sofridos = equipe->gols_sofridos;
    int diferenca = marcados - sofridos;
    equipe->saldo_de_gols = diferenca;
}

// analisa prefixo do nome do time
int conferePrefixoTime(const char *nome_equipe, const char *busca){
    if (nome_equipe == NULL || busca == NULL) {
        return 0;
    }

    int posicao = 0;
    int continuar = 1;
    
    while (continuar != 0) {
        char letra_busca = busca[posicao];
        
        if (letra_busca == '\0') {
            break;
        }
        
        char letra_nome = nome_equipe[posicao];
        
        if (letra_busca != letra_nome) {
            return 0;
        }
        
        posicao++;
    }
    
    return 1;
}

// mostra o time e as estatísticas
void exibirTime(Times *equipe){
    struct {
        int id;
        char nome[TAMANHO_NOME_TIME];
        int v, e, d, gm, gs, sg, pg;
    } dados_impressao;
    
    dados_impressao.id = equipe->ID;
    strcpy(dados_impressao.nome, equipe->nome_do_time);
    dados_impressao.v = equipe->vitorias;
    dados_impressao.e = equipe->empates;
    dados_impressao.d = equipe->derrotas;
    dados_impressao.gm = equipe->gols_marcados;
    dados_impressao.gs = equipe->gols_sofridos;
    dados_impressao.sg = equipe->saldo_de_gols;
    dados_impressao.pg = equipe->pontuacao;
    
    printf("| %-2d | %-14s | %1d | %1d | %1d | %2d | %2d | %3d | %2d |\n", 
           dados_impressao.id, dados_impressao.nome, dados_impressao.v, 
           dados_impressao.e, dados_impressao.d, dados_impressao.gm, 
           dados_impressao.gs, dados_impressao.sg, dados_impressao.pg);
}

// cria novo time no vetor
Times *construirTime(int codigo, const char *nome){
    Times *nova_equipe = malloc(sizeof(Times));
    
    if (nova_equipe == NULL) {
        perror("Falha ao alocar time\n");
        exit(1);
    }

    int *ptr_id = &(nova_equipe->ID);
    *ptr_id = codigo;
    
    char *destino = nova_equipe->nome_do_time;
    int limite = TAMANHO_NOME_TIME - 1;
    
    int i = 0;
    while (i < limite && nome[i] != '\0') {
        destino[i] = nome[i];
        i++;
    }
    destino[i] = '\0';
    
    nova_equipe->vitorias = 0;
    nova_equipe->empates = 0;
    nova_equipe->derrotas = 0;
    nova_equipe->gols_marcados = 0;
    nova_equipe->gols_sofridos = 0;
    nova_equipe->pontuacao = 0;
    nova_equipe->saldo_de_gols = 0;

    return nova_equipe;
}