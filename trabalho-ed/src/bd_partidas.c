#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bd_partidas.h"
#include "bd_times.h"

// Função que Desaloca memória 
void liberarAlocacaoDados(BD_Partidas *bd) {
    int total = bd->qntd;
    for (int idx = total - 1; idx >= 0; idx--) {
        Partidas *item = bd->partidas[idx];
        free(item);
    }
    free(bd);
}

// Função para exibir a Tela principal
void exibirTelaInicial(){
    printf("*** SISTEMA DE JOGOS ***\n");
    printf("\n");
    printf("(1) Buscar time\n");
    printf("(2) Buscar partida\n");
    printf("(6) Visualizar classificacao\n");
    printf("(Q) Finalizar programa\n");
    printf("Digite sua escolha: ");
}

// Inicializa banco de dados
BD_Partidas *inicializarBanco(){
    BD_Partidas *novo_banco = malloc(sizeof(BD_Partidas));
    if (!novo_banco) {
        printf("Falha ao alocar memoria\n");
        exit(1);
    }
    return novo_banco;
}

// Cria nova partida no vetor
Partidas *construirPartida(int codigo, int equipe_a, int equipe_b, int placar_a, int placar_b){
    Partidas *nova_partida = malloc(sizeof(Partidas));
    
    if (nova_partida == NULL) {
        perror("falha ao alocar\n");
        exit(1);
    }
    
    nova_partida->gols_time1 = placar_a;
    nova_partida->gols_time2 = placar_b;
    nova_partida->time1 = equipe_a; //Inicializando os ponteiros/valores dentro da estrutura
    nova_partida->time2 = equipe_b;
    nova_partida->id = codigo;

    return nova_partida;
}

// Registra nova partida no vetor
void inserirNovaPartida(BD_Partidas *bd, int id, int time1, int time2, int gols_time1, int gols_time2){
    int espacos_disponiveis = LIMITE_JOGOS - bd->qntd;
    
    if (espacos_disponiveis <= 0){
        printf("Capacidade maxima atingida\n");
        exit(1);
    }
    
    Partidas *registro = construirPartida(id, time1, time2, gols_time1, gols_time2);
    int *contador = &(bd->qntd);
    bd->partidas[*contador] = registro;
    *contador = *contador + 1;
}

// Mostra menu de busca 
void mostrarOpcoesDeBusca(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    printf("*** OPCOES DE BUSCA ***\n");
    printf("(1) Time como mandante\n");
    printf("(2) Time como visitante\n");
    printf("(3) Qualquer participacao\n");
    printf("(4) Retornar\n");
}

// Verifica se time participa da partida (como mandante, visitante ou ambos)
int verificarTimeNaPartida(const char *nome_busca, Partidas *jogo, struct BancoTimes *dados, int tipo){
    int posicao_time;
    
    // tipo: 1=mandante, 2=visitante, 3=qualquer
    if (tipo == 1) {
        posicao_time = jogo->time1;
    } else if (tipo == 2) {
        posicao_time = jogo->time2;
    } else {
        // Checa ambos
        int achou_mandante = verificarTimeNaPartida(nome_busca, jogo, dados, 1);
        int achou_visitante = verificarTimeNaPartida(nome_busca, jogo, dados, 2);
        return (achou_mandante || achou_visitante) ? 1 : 0;
    }
    
    char *nome_time = dados->times[posicao_time]->nome_do_time;
    
    // Compara prefixo do nome dos times
    int i = 0;
    while (nome_busca[i] != '\0') {
        if (nome_time[i] != nome_busca[i]) {
            return 0;
        }
        i++;
    }
    
    return 1;
}

// Realiza busca de partidas dentro do vetor
void buscarPartidas(BD_Partidas *dados_partidas, struct BancoTimes *dados_times){
    int selecao;
    char termo_busca[100];

    mostrarOpcoesDeBusca();
    scanf("%d", &selecao);

    if (selecao == 4) {
        printf("Voltando ao menu...\n");
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
        return;
    }

    int opcao_valida = (selecao >= 1 && selecao <= 3);
    if (!opcao_valida) {
        printf("Opcao invalida\n");
        return;
    }

    printf("\nDigite o nome ou prefixo do time: ");
    scanf("%s", termo_busca);

    int contador_resultados = 0;
    
    printf("\n+-----+--------------------------------------------------+\n");
    printf("| ID  | Partida                                          |\n");
    printf("+-----+--------------------------------------------------+\n");
    
    for (int idx = 0; idx < dados_partidas->qntd; idx++) {
        Partidas *jogo_atual = dados_partidas->partidas[idx];
        
        // Verifica se o time participa da partida
        int encontrou = verificarTimeNaPartida(termo_busca, jogo_atual, dados_times, selecao);

        if (encontrou != 0) {

            char *nome_casa = dados_times->times[jogo_atual->time1]->nome_do_time;
            char *nome_fora = dados_times->times[jogo_atual->time2]->nome_do_time;
            
            printf("| %-3d | %-20s %2d x %-2d %-20s |\n", 
                   jogo_atual->id, nome_casa, jogo_atual->gols_time1,
                   jogo_atual->gols_time2, nome_fora);
            
            contador_resultados++;
        }
    }
    
    printf("+-----+--------------------------------------------------+\n");
    
    if (contador_resultados == 0) {
        printf("\nNenhuma partida encontrada.\n");
    }
}

// importa dados do arquivo
void importarDadosArquivo(const char *bd_partidas, BD_Partidas *dados){
    FILE *arquivo_dados = fopen(bd_partidas, "r");
    
    if (arquivo_dados == NULL){
        printf("Nao foi possivel abrir %s\n", bd_partidas);
        exit(1);
    }

    char linha_lida[LIMITE_JOGOS];
    int valores[5];

    fgets(linha_lida, sizeof(linha_lida), arquivo_dados);
    dados->qntd = 0;

    while (1) {
        char *resultado_leitura = fgets(linha_lida, sizeof(linha_lida), arquivo_dados);
        
        if (resultado_leitura == NULL) {
            break;
        }
        
        int campos_lidos = sscanf(linha_lida, "%d, %d, %d, %d, %d", 
                                  &valores[0], &valores[1], &valores[2], &valores[3], &valores[4]);
        
        if (campos_lidos == 5) {
            inserirNovaPartida(dados, valores[0], valores[1], valores[2], valores[3], valores[4]);
        }
    }

    fclose(arquivo_dados);
}
